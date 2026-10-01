#pragma once
#include <atomic>

static inline constexpr const char* ANY_ASSERT_REGEX = "Assertion failed.*";

struct MoveTester
{
    int val;

private:
    unsigned int id;
    static inline std::atomic_uint current_id = 0;

public:
    MoveTester() : val(0), id(++current_id) {}
    MoveTester(int v) : val(v), id(++current_id) {}
    MoveTester(const MoveTester& other) : val(other.val), id(++current_id) {}
    MoveTester(MoveTester&& other) noexcept : val(other.val), id(other.id) { other.val = 0; other.id = ++current_id; }

    inline MoveTester& operator=(int v)
    {
        val = v;
        return *this;
    }

    inline MoveTester& operator=(const MoveTester& other)
    {
        if(this != &other)
        {
            val = other.val;
        }
        return *this;
    }

    inline MoveTester& operator=(MoveTester&& other) noexcept
    {
        if(this != &other)
        {
            val = other.val;
            id = other.id;
            other.val = 0;
            other.id = ++current_id;
        }
        return *this;
    }

    inline bool operator==(const MoveTester& other) const
    {
        return val == other.val;
    }

    inline unsigned int get_id() const { return id; }
};
