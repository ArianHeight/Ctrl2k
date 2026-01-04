#include "obn.h"

using namespace obn;

static const string_pool_chartype* try_add_string_to_bucket(
    data_bucket<string_pool_chartype>& block,
    const string_pool_chartype* str,
    size_t len)
{
    if(block.size() + len + 1 <= block.capacity())
    {
        string_pool_chartype* retVal = &block[block.size()];
        if(mem_copy(retVal, str, sizeof(string_pool_chartype) * len) == 0)
        {
            block[block.size() + len] = 0;
            block.size() += len + 1;
            return retVal;
        }
    }
    return nullptr;
}

dyn::string_pool::string_pool(size_t mem_block_size) :
    m_size(0),
    m_capacity(mem_block_size)
{
    m_mem_block.push_back(data_bucket<string_pool_chartype>(mem_block_size));
    mem_set_zero(m_mem_block[0].data(), m_mem_block[0].capacity());
}

const string_pool_chartype* dyn::string_pool::add(const string_pool_chartype* str, size_t len)
{
    if(!str)
    {
        return nullptr;
    }

    for(size_t i = 0; i < m_mem_block.size(); ++i)
    {
        const string_pool_chartype* retVal = try_add_string_to_bucket(m_mem_block[i], str, len);
        if(retVal)
        {
            m_size += len + 1;
            return retVal;
        }
    }

    size_t next_capacity = STRING_POOL_STARTING_CAPACITY;
    if(!m_mem_block.empty())
    {
        next_capacity = m_mem_block[m_mem_block.size() - 1].capacity() << 1;
    }
    if(len + 1 > next_capacity)
    {
        next_capacity = len + 1;
    }
    m_mem_block.push_back(data_bucket<string_pool_chartype>(next_capacity));
    mem_set_zero(m_mem_block[m_mem_block.size() - 1].data(), next_capacity);
    m_capacity += next_capacity;

    const string_pool_chartype* retVal = try_add_string_to_bucket(m_mem_block[m_mem_block.size() - 1], str, len);
    if(retVal)
    {
        m_size += len + 1;
        return retVal;
    }

    return nullptr;
}

string_registry_id dyn::string_registry::register_string(const string_pool_chartype* str, size_t len)
{
    assert(len < UINT32_MAX);
    if(!str)
    {
        return INVALID_STRING_REGISTRY_ID;
    }

    const hash64_t hash = checksum64_string(str, len);
    size_t idx = binary_search_nearest(m_views, hash);

    for(; idx < m_views.size() && m_views[idx].hash == hash; ++idx)
    {
        if(m_views[idx].len == len && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
        {
            return m_views[idx].id;
        }
    }

    const string_pool_chartype* ptr = m_pool.add(str, len);
    if(!ptr)
    {
        return INVALID_STRING_REGISTRY_ID;
    }

    if(idx > m_views.size())
    {
        idx = m_views.size(); // could have returned invalid
    }
    const string_registry_id new_id = (string_registry_id)m_id_map.size();
    m_views.insert(m_views.begin() + idx, { hash, new_id, (uint32_t)len });
    m_id_map.push_back(ptr);

    return new_id;
}

string_registry_id dyn::string_registry::find_registered_string(const string_pool_chartype* str, size_t len) const
{
    assert(len < UINT32_MAX);
    if(!str || m_views.empty())
    {
        return INVALID_STRING_REGISTRY_ID;
    }

    const hash64_t hash = checksum64_string(str, len);
    size_t idx = binary_search_nearest(m_views.data(), m_views.size(), hash);

    for(; idx < m_views.size() && m_views[idx].hash == hash; ++idx)
    {
        if(m_views[idx].len == len && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
        {
            return m_views[idx].id;
        }
    }

    return INVALID_STRING_REGISTRY_ID;
}

const string_pool_chartype* dyn::string_registry::get_string(const string_registry_id id) const
{
    return m_id_map[id];
}

size_t dyn::string_registry::num_strings() const
{
    return m_views.size();
}
