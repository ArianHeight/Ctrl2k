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

bool FilePath::set(c_string path, size_t len)
{
	m_path.copy(path, len);
	m_meta = generateFilePathMeta(m_path.view());
	return m_meta.flags.hasError;
}
