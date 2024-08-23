#include "LinuxFile.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

constexpr s64 BUFFER_SIZE = 8192;

namespace MAGE {
    b8 LinuxFile::Exists(const String &path) {
        return (access(path.c_str(), F_OK) == 0);
    }

    b8 LinuxFile::Create(const String &path) {
        if (const i32 fd = open(path.c_str(), O_CREAT | O_WRONLY, 0644); fd != -1) {
            close(fd);
            return true;
        }
        return false;
    }

    b8 LinuxFile::Delete(const String &path) {
        return (unlink(path.c_str()) == 0);
    }

    b8 LinuxFile::Write(const String &path, const String &data, const u64 offset) {
        // Open the file with write permissions
        const i32 fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            return false;
        }

        // Seek to the specified offset
        if (lseek(fd, offset, SEEK_SET) == -1) {
            // NOLINT(*-narrowing-conversions)
            close(fd);
            return false;
        }

        // Write the data to the file
        if (const s64 bytesWritten = write(fd, data.c_str(), data.size()); bytesWritten == -1) {
            close(fd);
            return false;
        }

        // Close the file
        close(fd);
        return true;
    }

    b8 LinuxFile::Write(const String &path, const MemoryBuffer &buffer, const u64 offset) {
        // Open the file with write permissions
        const i32 fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            return false;
        }

        // Seek to the specified offset
        if (lseek(fd, offset, SEEK_SET) == -1) {
            close(fd);
            return false;
        }

        // Write the data to the file
        if (const s64 bytesWritten = write(fd, buffer.GetData(), buffer.GetSize()); bytesWritten == -1) {
            close(fd);
            return false;
        }

        // Close the file
        close(fd);
        return true;
    }

    b8 LinuxFile::Read(const String &path, String &contentOut, const u64 startByte, const u64 endByte) {

        const i32 fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            return false;
        }

        // Seek to the start byte
        if (lseek(fd, startByte, SEEK_SET) == -1) {
            close(fd);
            return false;
        }

        // Determine the file size
        s64 fileSize = lseek(fd, 0, SEEK_END);
        if (fileSize == -1) {
            close(fd);
            return false;
        }

        // Seek back to the start byte
        if (lseek(fd, startByte, SEEK_SET) == -1) {
            close(fd);
            return false;
        }

        // Calculate the number of bytes to read
        const u64 bytesToRead = static_cast<u64>(fileSize) - startByte;
        if (bytesToRead <= 0) {
            close(fd);
            return false;
        }

        // Allocate buffer for reading
        const auto buffer = new i8[bytesToRead];
        const s64 bytesRead = read(fd, buffer, bytesToRead);
        if (bytesRead == -1) {
            printf("Fourth\n");
            delete[] buffer;
            close(fd);
            return false;
        }

        // Resize the contentOut string to the amount of data read
        contentOut.assign(buffer, bytesRead);

        close(fd);
        return true;
    }

    b8 LinuxFile::Read(const String &path, MemoryOwnedBuffer &view, const u64 startByte, const u64 endByte) {
        const i32 fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            return false;
        }

        // Seek to the start byte
        if (lseek(fd, startByte, SEEK_SET) == -1) {
            close(fd);
            return false;
        }

        // Calculate the number of bytes to read
        const u64 bytesToRead = endByte - startByte;
        if (bytesToRead <= 0) {
            close(fd);
            return false;
        }

        // Allocate buffer for reading
        const auto buffer = new i8[bytesToRead];
        const s64 bytesRead = read(fd, buffer, bytesToRead);
        if (bytesRead == -1) {
            delete[] buffer;
            close(fd);
            return false;
        }

        // Resize the contentOut string to the amount of data read
        view = MemoryOwnedBuffer(buffer, bytesRead);

        // Clean up
        delete[] buffer;
        close(fd);
        return true;
    }

    b8 LinuxFile::Copy(const String &source, const String &destination) {
        // Open the source file with read-only permissions
        const i32 srcFd = open(source.c_str(), O_RDONLY);
        if (srcFd == -1) {
            return false;
        }

        // Open the destination file with write permissions, create it if it doesn't exist, and truncate it if it does
        const i32 destFd = open(destination.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destFd == -1) {
            close(srcFd);
            return false;
        }

        // Buffer for reading and writing data
        i8 buffer[BUFFER_SIZE];
        s64 bytesRead;

        // Copy data from source to destination
        while ((bytesRead = read(srcFd, buffer, BUFFER_SIZE)) > 0) {
            if (const s64 bytesWritten = write(destFd, buffer, bytesRead); bytesWritten != bytesRead) {
                close(srcFd);
                close(destFd);
                return false;
            }
        }

        // Check for read error
        if (bytesRead == -1) {
            return false;
        }

        // Clean up
        close(srcFd);
        close(destFd);

        return true;
    }

    b8 LinuxFile::Move(const String &source, const String &destination) {
        if (rename(source.c_str(), destination.c_str()) != 0) {
            return false;
        }
        return true;
    }

    b8 LinuxFile::Rename(const String &source, const String &destination) {
        if (rename(source.c_str(), destination.c_str()) != 0) {
            return false;
        }
        return true;
    }

    b8 LinuxFile::GetSize(const String &path, u64 &sizeOut) {
        struct stat fileInfo{};

        // Retrieve file information
        if (stat(path.c_str(), &fileInfo) != 0) {
            return false;
        }

        // Check if it is a regular file
        if (!S_ISREG(fileInfo.st_mode)) {
            return false;
        }

        // Get file size
        sizeOut = static_cast<u64>(fileInfo.st_size);
        return true;
    }

    b8 LinuxFile::GetName(const String &path, String &nameOut) {
        // Copy the path to a writable buffer
        const auto pathCopy = new i8[path.size() + 1];
        std::strcpy(pathCopy, path.c_str());

        // Get the base name (file name) from the path
        const i8 *baseName = basename(pathCopy);

        // Check if basename returned a valid name
        if (baseName == nullptr) {
            delete[] pathCopy;
            return false;
        }

        // Assign the base name to the output string
        nameOut = baseName;

        // Clean up
        delete[] pathCopy;

        return true;
    }

    b8 LinuxFile::GetExtension(const String &path, String &extensionOut) {
        // If a period is found, and it's not at the beginning of the path
        if (const s64 dotPos = path.find_last_of('.'); dotPos != std::string::npos && dotPos != path.find_last_of('/')) {
            // Extract the extension from the dot position to the end of the string
            extensionOut = path.substr(dotPos + 1);
            return true;
        } else {
            // No valid extension found
            extensionOut.clear();
            return false;
        }
    }

    b8 LinuxFile::GetDirectory(const String &path, String &directoryOut) {
        // If a slash is found, and it's not at the beginning of the path
        if (const s64 lastSlashPos = path.find_last_of('/'); lastSlashPos != std::string::npos && lastSlashPos != 0) {
            // Extract the directory part from the start to the last slash
            directoryOut = path.substr(0, lastSlashPos);
            return true;
        } else if (lastSlashPos == 0) {
            // If the path starts with a slash, the directory is the root
            directoryOut = "/";
            return true;
        } else {
            // No directory part found (e.g., the path is a single file name)
            directoryOut.clear();
            return false;
        }
    }
} // MAGE
