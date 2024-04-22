#pragma once
#include <string>

typedef std::string FilePath;
typedef std::string FileName;
typedef std::string FileNameNoExt;
typedef unsigned int LineNumber;

FileName FilePath_GetFileName(const FilePath& path);
FileNameNoExt FilePath_GetFileNameNoExt(const FilePath &path);