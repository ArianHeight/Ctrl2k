#pragma once
#include <ostream>
#include <string>

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
    FilePath() = default;
    FilePath(std::string&& pathstring);
    FilePath(const std::string& pathstring);
    FilePath(FilePath&& newPath) noexcept;
    FilePath(const FilePath& oldPath);
    
    FilePath& operator=(const std::string& pathstring); // copy
    FilePath& operator=(std::string&& pathstring) noexcept; // move
    FilePath& operator=(const FilePath& other); // copy
    FilePath& operator=(FilePath&& other) noexcept; // move

    bool isEmpty() const { return m_path.empty(); }

    const std::string& path() const { return m_path; }
    const std::string_view& folderPathView() const { return m_folderPath; }
    const std::string_view& fileNameView() const { return m_fileName; }
    const std::string_view& fileNameNoExtView() const { return m_fileNameNoExt; }
    const std::string_view& fileExtView() const { return m_fileExt; }
    FolderPath folderPath() const { return std::string(m_folderPath); }
    FileName fileName() const { return std::string(m_fileName); }
    FileNameNoExt fileNameNoExt() const { return std::string(m_fileNameNoExt); }
    FileExt fileExt() const { return std::string(m_fileExt); }

private:
    void copy(const std::string& pathstring);
    void move(std::string&& pathstring);
    void copy(const FilePath& other);
    void move(FilePath&& other);
    void generateViews();

    std::string m_path;
    std::string_view m_folderPath;
    std::string_view m_fileName;
    std::string_view m_fileNameNoExt;
    std::string_view m_fileExt;
};
inline std::ostream& operator<<(std::ostream& os, const FilePath& path) { os << path.path(); return os; }
}