#include "FilePathProcessing.h"

FileName FilePath_GetFileName(const FilePath& path)
{
	static const char delim = '\\';

	size_t index = path.rfind(delim);
	if(index == std::string::npos || index == path.length())
		return path;
	return path.substr(index + 1);
}

FileNameNoExt FilePath_GetFileNameNoExt(const FilePath& path)
{
	static const char delim = '.';

	FileName nameWithExt = FilePath_GetFileName(path);
	size_t index = nameWithExt.rfind(delim);
	if(index == std::string::npos || index == 0)
		return nameWithExt;
	return nameWithExt.substr(0, index);
}