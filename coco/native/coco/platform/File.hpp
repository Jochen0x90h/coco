#pragma once

#include <coco/enum.hpp>
#include <filesystem>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#undef interface
#undef INTERFACE
#undef IN
#undef OUT
#undef READ_ATTRIBUTES
#undef ERROR
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif


namespace coco {

namespace fs = std::filesystem;

/**
	Simple file wrapper only for internal use such as in emulator drivers. Otherwise use coco-file
*/
class File {
public:
	enum class Mode : uint32_t {
#ifdef _WIN32
		READ = GENERIC_READ,
		WRITE = GENERIC_WRITE,
		READ_WRITE = READ | WRITE,

		TRUNCATE = 1
#else
		READ = O_RDONLY,
		WRITE = O_WRONLY,
		READ_WRITE = O_RDWR,

		TRUNCATE = O_TRUNC
#endif
	};

	/**
	 * Constructor opens a file
	 * @param name file name
	 * @param mode combination of Mode elements
	 */
	File(const fs::path &filename, Mode mode);

	/**
	 * Destructor
	 */
	~File() {
#ifdef _WIN32
		CloseHandle(this->file);
#else
		close(this->file);
#endif
	}

	bool isOpen() {
#ifdef _WIN32
		return this->file != nullptr;
#else
		return this->file != -1;
#endif
	}

	/**
	 * Get size of file
	 * @return file size
	 */
	int getSize() {
#ifdef _WIN32
		return GetFileSize(this->file, nullptr);
#else
		struct stat stat;
		fstat(this->file, &stat);
		return stat.st_size;
#endif
	}

	/**
	 * Set size of file
	 * @param value initial value
	 */
	void resize(int size, uint8_t value = 0) {
		if (value != 0) {
			int currentSize = getSize();
			if (currentSize <= size) {
				fill(currentSize, value, size - currentSize);
				return;
			}
		}
#ifdef _WIN32
		SetFilePointer(this->file, size, nullptr, FILE_BEGIN);
		SetEndOfFile(this->file);
#else
		ftruncate(this->file, size);
#endif
	}

	int read(int offset, void *data, int length) {
#ifdef _WIN32
		DWORD numRead;
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.Offset = offset;
		ReadFile(this->file, data, length, &numRead, &overlapped);
		return numRead;
#else
		return pread(this->file, data, length, offset);
#endif
	}

	int write(int offset, const void *data, int length) {
#ifdef _WIN32
		DWORD numWritten;
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.Offset = offset;
		WriteFile(this->file, data, length, &numWritten, &overlapped);
		return numWritten;
#else
		return pwrite(this->file, data, length, offset);
#endif
	}

	void fill(int offset, uint8_t value, int length) {
		uint8_t buffer[16] = {value, value, value, value, value, value, value, value, value, value, value, value, value, value, value, value};
		int count = length >> 4;
		for (int i = 0; i < count; ++i)
			write(offset + i * 16, buffer, 16);
		int remaining = length & 15;
		if (remaining > 0)
			write(offset + count * 16, buffer, remaining);
	}

protected:
#ifdef _WIN32
	HANDLE file;
#else
	int file;
#endif
};

COCO_ENUM(File::Mode)

inline File::File(const fs::path &filename, Mode mode) {
#ifdef _WIN32
	this->file = CreateFileW(filename.c_str(),
		int(mode & Mode::READ_WRITE),
		FILE_SHARE_READ,
		nullptr, // security
		(mode & Mode::TRUNCATE) != 0 ? CREATE_ALWAYS : OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
#else
	this->file = open(filename.c_str(), O_CREAT | int(mode), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
#endif
}

} // namespace coco
