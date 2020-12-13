#include "FilePathProcessing.h"

FileName FilePath_GetFileName(const FilePath& path)
{
	static const char delim = '\\';

	size_t index = path.rfind(delim);
	if(index == std::string::npos || index == path.length())
		return path;
	return path.substr(index + 1);
}