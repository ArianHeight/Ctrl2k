#include "FilePathProcessing.h"

#include "Core/Petra/basicmath.h"

using namespace gbt;

static FilePathMeta generateFilePathMeta(const file_path_viewtype& path)
{
	FilePathMeta meta;
	const size_t nameStart = path.find_last_of(FOLDER_DELIMITERS);
	meta.filenameStart = nameStart == INVALID_SIZE_T ? 0 : static_cast<uint16_t>(rqm::min(nameStart + 1, path.length()));

	const size_t extPos = path.find_last_of(FILE_EXT_DELIMITER);
	meta.extPos = extPos == INVALID_SIZE_T || extPos < meta.filenameStart ?
		static_cast<uint16_t>(path.length()) : static_cast<uint16_t>(rqm::min(extPos, path.length()));

	meta.flags.hasError = meta.extPos < meta.filenameStart || path.length() > FILE_PATH_MAX_LEN || path.empty();
	meta.flags.isFolder = meta.extPos == path.length() && meta.filenameStart == path.length();
	meta.flags.hasExt = meta.extPos + 1 < path.length();
	meta.flags.hasFolder = meta.filenameStart > 0; // can have a file start with /

	return meta;
}

bool FilePathView::setView(c_string path, size_t len)
{
	m_view.set(path, len);
	m_meta = generateFilePathMeta(m_view);
	return m_meta.flags.hasError;
}

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