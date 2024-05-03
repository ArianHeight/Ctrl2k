#pragma once
#include <ostream>
#include <string>

#include "Core/Monument/basictypes.h"

namespace gbt
{
typedef std::string FileExt;
typedef std::string FileName;
typedef std::string FileNameNoExt;
typedef std::string FolderPath;

typedef size_t LineNumber;
constexpr LineNumber INVALID_LINE_NUMBER = SIZE_MAX;

constexpr const char* FILE_EXT_DELIMITER = ".";
constexpr const char* FOLDER_DELIMITERS = "/\\";

class FilePath
{
public:
    FilePath() : m_path(""), m_nameStartPos(0), m_nameEndPos(0), m_extStartPos(0) {}
    FilePath(std::string&& pathstring);
    FilePath(const std::string& pathstring);
    FilePath(c_string c_str);
    
    FilePath& operator=(const std::string& pathstring); // copy
    FilePath& operator=(std::string&& pathstring) noexcept; // move
    FilePath& operator=(c_string c_str); // copy

    bool isEmpty() const { return m_path.empty(); }

    const std::string& path() const { return m_path; }

    // use views if possible, but the data can go out of scope for these
    const std::string_view folderPathView() const { return { m_path.data(), m_nameStartPos }; }
    const std::string_view fileNameView() const { return { &(m_path[m_nameStartPos]), m_path.length() - m_nameStartPos }; }
    const std::string_view fileNameNoExtView() const { return { &(m_path[m_nameStartPos]), m_nameEndPos - m_nameStartPos }; }
    const std::string_view fileExtView() const { return { &(m_path[m_extStartPos]), m_path.length() - m_extStartPos }; }

    // copied strings
    FolderPath folderPath() const { return m_path.substr(0, m_nameStartPos); }
    FileName fileName() const { return m_path.substr(m_nameStartPos); }
    FileNameNoExt fileNameNoExt() const { return m_path.substr(m_nameStartPos, m_nameEndPos - m_nameStartPos); }
    FileExt fileExt() const { return m_path.substr(m_extStartPos); }

    inline bool operator==(const FilePath& other) const { return m_path == other.m_path; }

private:
    void copy(const std::string& pathstring);
    void move(std::string&& pathstring);
    void generatePos();

    std::string m_path;
    size_t m_nameStartPos;
    size_t m_nameEndPos;
    size_t m_extStartPos;
};
inline std::ostream& operator<<(std::ostream& os, const FilePath& path) { os << path.path(); return os; }
}