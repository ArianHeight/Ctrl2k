#include "FilePathProcessing.h"

#include "Core/Monument/Monument.h"

using namespace gbt;

FilePath::FilePath(std::string&& pathstring)
{
	move(std::move(pathstring));
}

FilePath::FilePath(const std::string& pathstring)
{
	copy(pathstring);
}

FilePath::FilePath(FilePath&& newPath) noexcept
{
	move(std::move(newPath));
}

FilePath::FilePath(const FilePath& oldPath)
{
	copy(oldPath);
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

FilePath& FilePath::operator=(const FilePath& other)
{
	copy(other);
	return *this;
}

FilePath& FilePath::operator=(FilePath&& other) noexcept
{
	move(std::move(other));
	return *this;
}

void FilePath::copy(const std::string& pathstring)
{
	m_path = pathstring;
	generateViews();
}

void FilePath::move(std::string&& pathstring)
{
	m_path = std::move(pathstring);
	generateViews();
}

void FilePath::copy(const FilePath& other)
{
	m_path = other.m_path;
	generateViews();
}

void FilePath::move(FilePath&& other)
{
	m_path = std::move(other.m_path);
	m_folderPath = std::move(other.m_folderPath);
	m_fileName = std::move(other.m_fileName);
	m_fileNameNoExt = std::move(other.m_fileNameNoExt);
	m_fileExt = std::move(other.m_fileExt);

#ifdef _DEBUG
	assert(m_folderPath.data() == nullptr || (m_folderPath.data() >= m_path.data() && m_folderPath.data() < m_path.data() + m_path.size()));
	assert(m_fileName.data() == nullptr || (m_fileName.data() >= m_path.data() && m_fileName.data() < m_path.data() + m_path.size()));
	assert(m_fileNameNoExt.data() == nullptr || (m_fileNameNoExt.data() >= m_path.data() && m_fileNameNoExt.data() < m_path.data() + m_path.size()));
	assert(m_fileExt.data() == nullptr || (m_fileExt.data() >= m_path.data() && m_fileExt.data() < m_path.data() + m_path.size()));
#endif
}

void FilePath::generateViews()
{
	const std::string_view pathView = std::string_view(m_path);

	const size_t pathLen = m_path.length();
	// TODO implement with min/max
	size_t dotPos = pathView.rfind(FILE_EXT_DELIMITER);
	const bool hadInvalidDot = dotPos == std::string_view::npos;
	dotPos = hadInvalidDot ? pathLen : dotPos;
	const size_t dotPosShifted = hadInvalidDot ? pathLen : dotPos + 1;

	size_t slashPos = pathView.find_last_of(FOLDER_DELIMITERS);
	const bool hadInvalidSlash = slashPos == std::string_view::npos;
	const size_t slashPosShifted = hadInvalidDot ? pathLen : (hadInvalidSlash ? 0 : slashPos + 1);
	slashPos = hadInvalidDot ? pathLen : (hadInvalidSlash ? 0 : slashPos);

	const bool slashReasonable = slashPosShifted < pathLen;

	m_folderPath = pathView.substr(0, slashPos);
	m_fileName = slashReasonable ? pathView.substr(slashPosShifted) : std::string_view();
	m_fileNameNoExt = slashReasonable ? pathView.substr(slashPosShifted, dotPos - slashPosShifted) : std::string_view();
	m_fileExt = pathView.substr(dotPosShifted);
}