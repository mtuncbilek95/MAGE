#include "LinuxDirectory.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define BYTE_SIZE 65536

namespace MAGE {
    b8 LinuxDirectory::Exists(const String &path) {
        struct stat stats{};
        if (const char *filePath = path.c_str(); stat(filePath, &stats) == 0 && S_ISDIR(stats.st_mode))
            return true;

        return false;
    }

    b8 LinuxDirectory::Create(const String &path) {
        if (!Exists(path)) {
            if (mkdir(path.c_str(), 0755)) {
                return true;
            }
        }

        return false;
    }

    b8 LinuxDirectory::Delete(const String &path) {
        if (!Exists(path)) {
            if (rmdir(path.c_str()) == 0) {
                return true;
            }
        }

        return false;
    }

    b8 LinuxDirectory::Copy(const String &source, const String &destination) {
        if (!Exists(source) || Exists(destination))
            return false;

        const i32 src = open(source.c_str(),O_RDONLY);
        if (src == -1) {
            CORE_LOG(M_ERROR, "Failed to open source directory");
            return false;
        }

        const i32 dst = open(destination.c_str(),O_WRONLY | O_CREAT, 0755);
        if (dst == -1) {
            CORE_LOG(M_ERROR, "Failed to open destination directory");
            close(src);
            return false;
        }

        i8 buffer[BYTE_SIZE];
        i64 bytesRead;
        while ((bytesRead = read(src, buffer, sizeof(buffer))) > 0) {
            if (const i64 bytesWritten = write(dst, buffer, bytesRead); bytesRead != bytesWritten) {
                CORE_LOG(M_ERROR, "Failed to copy directory");
                close(src);
                close(dst);
                return false;
            }
        }
        if (bytesRead == -1) {
            close(src);
            close(dst);
            return false;
        }

        close(src);
        close(dst);
        return true;
    }

    b8 LinuxDirectory::Move(const String &source, const String &destination) {
        if (!Exists(source) || Exists(destination)) {
            return false;
        }
        const char *src = source.c_str();
        if (const char *dst = destination.c_str(); rename(src, dst) == 0) {
            return true;
        }
        return false;
    }

    b8 LinuxDirectory::Rename(const String &source, const String &destination) {
        if (!Exists(source) || Exists(destination)) {
            return false;
        }
        const char *src = source.c_str();
        if (const char *dst = destination.c_str(); rename(src, dst) == 0) {
            return true;
        }
        return false;
    }

    b8 LinuxDirectory::GetFiles(const String &path, Vector<String> &files) {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != nullptr) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_REG)
                files.emplace_back(d->d_name);
        }
        closedir(dir);
        return true;
    }

    b8 LinuxDirectory::GetDirectories(const String &path, Vector<String> &directories) {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != nullptr) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_DIR)
                directories.emplace_back(d->d_name);
        }
        closedir(dir);
        return true;
    }

    b8 LinuxDirectory::GetFilesWithExtension(const String &path, const String &extension, Vector<String> &files) {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != nullptr) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_REG && strncmp(extension.c_str(), d->d_name, sizeof(extension.c_str())) == 0)
                files.emplace_back(d->d_name);
        }
        closedir(dir);
        return true;
    }
}
