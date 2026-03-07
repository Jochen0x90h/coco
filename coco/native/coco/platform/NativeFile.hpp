#pragma once

#include <coco/enum.hpp>
#include <filesystem>
#ifdef _WIN32
#include "WindowsDef.hpp"
#include <windows.h>
#include "WindowsUndef.hpp"
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif


namespace coco {

namespace fs = std::filesystem;

/// @brief Simple file wrapper only for internal use such as in emulator drivers. Otherwise use coco-file.
///
class NativeFile {
public:
    enum class Mode : uint32_t {
#ifdef _WIN32
        // open existing file in read-only mode
        OPEN_READ = OPEN_EXISTING | GENERIC_READ,

        // open existing file in read/write mode
        OPEN = OPEN_EXISTING | GENERIC_READ | GENERIC_WRITE,

        // truncate existing file and open in read/write mode
        TRUNCATE = TRUNCATE_EXISTING | GENERIC_READ | GENERIC_WRITE,

        // create file if it does not exist or open existing file in read/write mode
        CREATE_OR_OPEN = OPEN_ALWAYS | GENERIC_READ | GENERIC_WRITE,

        // create file if it does not exist or truncate existing file and open in read/write mode
        CREATE_OR_TRUNCATE = CREATE_ALWAYS | GENERIC_READ | GENERIC_WRITE,

        // create file if it does not exist of fail if it exists and open in read/write mode
        CREATE_OR_FAIL = CREATE_NEW | GENERIC_READ | GENERIC_WRITE,
#else
        // open existing file in read-only mode
        OPEN_READ = O_RDONLY,

        // open existing file in read/write mode
        OPEN = O_RDWR,

        // truncate existing file and open in read/write mode
        TRUNCATE = O_TRUNC | O_RDWR,

        // create file if it does not exist or open in read/write mode
        CREATE_OR_OPEN = O_CREAT | O_RDWR,

        // create file if it does not exist or truncate existing file and open in read/write mode
        CREATE_OR_TRUNCATE = O_CREAT | O_TRUNC | O_RDWR,

        // create file if it does not exist of fail if it exists and open in read/write mode
        CREATE_OR_FAIL = O_CREAT | O_EXCL | O_RDWR,
#endif
    };


    /// @brief Constructor opens a file.
    /// @param name file name
    /// @param mode combination of Mode elements
    NativeFile(const fs::path &filename, Mode mode) {
#ifdef _WIN32
    file_ = CreateFileW(filename.c_str(),
        int(mode) & (GENERIC_READ | GENERIC_WRITE),
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, // security
        int(mode) & 7,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);
#else
    file_ = open(filename.c_str(), int(mode), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
#endif
    }

    /// @brief Destructor.
    ///
    ~NativeFile() {
#ifdef _WIN32
        CloseHandle(file_);
#else
        close(file_);
#endif
    }

    bool isOpen() {
#ifdef _WIN32
        return file_ != INVALID_HANDLE_VALUE;
#else
        return file_ != -1;
#endif
    }

    /// @brief Get size of file.
    /// @return file size
    int64_t size() {
#ifdef _WIN32
        // https://learn.microsoft.com/de-de/windows/win32/api/winbase/nf-winbase-getfileinformationbyhandleex
        FILE_STANDARD_INFO fileInfo;
        if (!GetFileInformationByHandleEx(
                file_,
                FileStandardInfo,
                &fileInfo,
                sizeof(fileInfo)))
        {
            //int error = GetLastError();
            return -1;
        }
        return fileInfo.EndOfFile.QuadPart;
#else
        struct stat st;
        if (fstat(file_, &st) < 0) {
            //int error = errno;
            return -1;
        }
        return st.st_size;
#endif
    }

    /// @brief Set size of file.
    /// @param value initial value
    void resize(int64_t size, uint8_t value = 0) {
#ifdef _WIN32
        // https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfileinformationbyhandle
        FILE_END_OF_FILE_INFO eofInfo;
        eofInfo.EndOfFile.QuadPart = size;
        SetFileInformationByHandle(
            file_,
            FileEndOfFileInfo,
            &eofInfo,
            sizeof(eofInfo));
#else
        ftruncate(file_, size);
#endif
    }

    int read(int64_t offset, void *data, int length) {
#ifdef _WIN32
        DWORD numRead = -1;
        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(overlapped));
        *(int64_t *)(&overlapped.Offset) = offset;
        ReadFile(file_, data, length, &numRead, &overlapped);
        return numRead;
#else
        return pread(file_, data, length, offset);
#endif
    }

    int write(int64_t offset, const void *data, int length) {
#ifdef _WIN32
        DWORD numWritten = -1;
        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(overlapped));
        *(int64_t *)(&overlapped.Offset) = offset;
        WriteFile(file_, data, length, &numWritten, &overlapped);
        return numWritten;
#else
        return pwrite(file_, data, length, offset);
#endif
    }

protected:
#ifdef _WIN32
    HANDLE file_;
#else
    int file_;
#endif
};

} // namespace coco
