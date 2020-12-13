#pragma once
#include <string>

typedef std::string FilePath;
typedef std::string FileName;
typedef unsigned int LineNumber;

FileName FilePath_GetFileName(const FilePath& path);