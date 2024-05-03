#include "FilePathProcessing.h"

#include "Core/Petra/basicmath.h"

using namespace gbt;

FilePath::FilePath(std::string&& pathstring)
{
	move(std::move(pathstring));
}

FilePath::FilePath(const std::string& pathstring)
{
	copy(pathstring);
}

FilePath::FilePath(c_string c_str)
{
	*this = std::string(c_str);
}

FilePath& FilePath::operator=(const std::string& pathstring)
{
	copy(pathstring);
	return *this;
}

FilePath& FilePath::operator=(std::string&& pathstring) noexcept
{
	move(std::move(pathstring));
	return *this;
}

FilePath& FilePath::operator=(c_string c_str)
{
	*this = std::string(c_str);
	return *this;
}

void FilePath::copy(const std::string& pathstring)
{
	m_path = pathstring;
	generatePos();
}

void FilePath::move(std::string&& pathstring)
{
	m_path = std::move(pathstring);
	generatePos();
}

void FilePath::generatePos()
{
	const size_t pathLen = m_path.length();
	m_nameStartPos = m_path.find_last_of(FOLDER_DELIMITERS);
	m_nameStartPos = m_nameStartPos == std::string::npos ? 0 : rqm::min( m_nameStartPos + 1, pathLen );
	const size_t nameLen = pathLen - m_nameStartPos;

	const std::string_view nameView = std::string_view(&(m_path[m_nameStartPos]), nameLen);
	m_nameEndPos = rqm::min(nameView.rfind(FILE_EXT_DELIMITER), nameLen) + m_nameStartPos;
	m_extStartPos = rqm::min(m_nameEndPos + 1, pathLen);
}