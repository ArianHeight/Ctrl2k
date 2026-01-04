#pragma once
#include <ostream>
#include <string>

#include "Core/Monument/monument.h"
#include "Core/OracleBone/obn.h"

namespace gbt
{
// using the smallest value for max file path length(which is on old windows)
constexpr size_t FILE_PATH_MAX_LEN = 260;
constexpr size_t FILE_PATH_MAX_BUF_LEN = FILE_PATH_MAX_LEN + 1;

typedef std::string FileExt;
typedef std::string FileName;
typedef std::string FileNameNoExt;
typedef std::string FolderPath;

typedef size_t LineNumber;
constexpr LineNumber INVALID_LINE_NUMBER = SIZE_MAX;

constexpr c_string FILE_EXT_DELIMITER = ".";
constexpr c_string FOLDER_DELIMITERS = "/\\";

template <typename chartype>
using generic_file_path_data = obn::generic_stack_string<chartype, FILE_PATH_MAX_BUF_LEN>;
using file_path_data = generic_file_path_data<char>;
using wfile_path_data = generic_file_path_data<wchar_t>;

typedef obn::view_string file_path_viewtype;
typedef obn::view_wstring wfile_path_viewtype;

union FilePathFlags
{
    struct
    {
        bool hasError : 1;
        bool isFolder : 1;
        bool hasFolder : 1;
        bool hasExt : 1;
    };
    uint16_t packed;

    FilePathFlags() : packed(0) { hasError = true; }
};

struct FilePathMeta
{
    FilePathFlags flags;
    uint16_t filenameStart;
    uint16_t extPos;
};

class FilePath;

class FilePathView
{
private:
    friend class FilePath;

    file_path_viewtype m_view;
    FilePathMeta m_meta;

    // This is only for initialization from the FilePath class!
    FilePathView(const file_path_viewtype& view, FilePathMeta meta) : m_view(view), m_meta(meta) {}

public:
    FilePathView() = default;
    FilePathView(c_string path, size_t len) { setView(path, len); };
    FilePathView(c_string path) { setView(path, obn::string_len(path)); }
    FilePathView(const file_path_data& path) { setView(path.c_str(), path.length()); }
    FilePathView(const file_path_viewtype& path) { setView(path.c_str(), path.length()); }

    bool setView(c_string path, size_t len);
    
    inline bool hasError() const { return m_meta.flags.hasError; }
    inline const FilePathFlags& getFlags() const { return m_meta.flags; }
    inline bool isEmpty() const { return m_view.empty(); }

    inline file_path_viewtype folderPathView() const { return m_view.substring(m_meta.filenameStart); }
    inline file_path_viewtype fileNameView() const { return m_view.substring(m_meta.filenameStart, m_view.length() - m_meta.filenameStart); }
    inline file_path_viewtype fileNameNoExtView() const { return m_view.substring(m_meta.filenameStart, m_meta.extPos - m_meta.filenameStart); }
    inline file_path_viewtype fileExtView() const { return m_meta.flags.hasExt ? m_view.substring(m_meta.extPos + 1, m_view.length() - m_meta.extPos - 1) : m_view.substring(0, 0); }
    inline const file_path_viewtype& pathView() const { return m_view; }

    inline file_path_data folderPath() const { return folderPathView(); }
    inline file_path_data fileName() const { return fileNameView(); }
    inline file_path_data fileNameNoExt() const { return fileNameNoExtView(); }
    inline file_path_data fileExt() const { return fileExtView(); }
    inline file_path_data path() const { return m_view; }

    // just copy memory, everything is flat anyways
    FilePathView& operator=(const FilePathView& other) = default;
    inline FilePathView& operator=(c_string path) { setView(path, obn::string_len(path)); return *this; }
    inline FilePathView& operator=(const file_path_data& path) { setView(path.c_str(), path.length()); return *this; }
    inline FilePathView& operator=(const file_path_viewtype& path) { setView(path.c_str(), path.length()); return *this; }
    
    inline bool operator==(const FilePathView& other) const { return m_view == other.m_view; }
};
inline std::ostream& operator<<(std::ostream& os, const FilePathView& path) { os << path.pathView(); return os; }

class FilePath
{
private:
    file_path_data m_path;
    FilePathMeta m_meta;

public:
    FilePath() = default;
    FilePath(c_string path, size_t len) { set(path, len); };
    FilePath(c_string path) { set(path, obn::string_len(path)); }
    FilePath(const file_path_data& path) { set(path.c_str(), path.length()); }
    FilePath(const file_path_viewtype& path) { set(path.c_str(), path.length()); }
    FilePath(const FilePathView& view) { set(view.pathView().c_str(), view.pathView().length()); }
    
    bool set(c_string path, size_t len);

    inline FilePathView view() const { return FilePathView(m_path.view(), m_meta); }

    inline bool hasError() const { return m_meta.flags.hasError; }
    inline const FilePathFlags& getFlags() const { return m_meta.flags; }
    inline bool isEmpty() const { return m_path.empty(); }

    inline file_path_viewtype folderPathView() const { return m_path.subview(m_meta.filenameStart); }
    inline file_path_viewtype fileNameView() const { return m_path.subview(m_meta.filenameStart, m_path.length() - m_meta.filenameStart); }
    inline file_path_viewtype fileNameNoExtView() const { return m_path.subview(m_meta.filenameStart, m_meta.extPos - m_meta.filenameStart); }
    inline file_path_viewtype fileExtView() const { return m_meta.flags.hasExt ? m_path.subview(m_meta.extPos + 1, m_path.length() - m_meta.extPos - 1) : m_path.subview(0, 0); }
    inline file_path_viewtype pathView() const { return m_path.view(); }

    inline file_path_data folderPath() const { return m_path.substring(m_meta.filenameStart); }
    inline file_path_data fileName() const { return m_path.substring(m_meta.filenameStart, m_path.length() - m_meta.filenameStart); }
    inline file_path_data fileNameNoExt() const { return m_path.substring(m_meta.filenameStart, m_meta.extPos - m_meta.filenameStart); }
    inline file_path_data fileExt() const { return m_meta.flags.hasExt ? m_path.substring(m_meta.extPos + 1, m_path.length() - m_meta.extPos - 1) : m_path.substring(0, 0); }
    inline const file_path_data& path() const { return m_path; }

    // just copy mem, it's literally flat
    FilePath& operator=(const FilePath& other) = default;
    inline FilePath& operator=(const FilePathView& other) { set(other.pathView().c_str(), other.pathView().length()); return *this; };
    inline FilePath& operator=(c_string path) { set(path, obn::string_len(path)); return *this; }
    inline FilePath& operator=(const file_path_data& path) { set(path.c_str(), path.length()); return *this; }
    inline FilePath& operator=(const file_path_viewtype& path) { set(path.c_str(), path.length()); return *this; }

    inline bool operator==(const FilePath& other) const { return m_path == other.m_path; }
};
inline std::ostream& operator<<(std::ostream& os, const FilePath& path) { os << path.path(); return os; }
}