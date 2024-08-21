#include "LinuxDirectory.h"

namespace MAGE
{
    b8 LinuxDirectory::Exists(const String& path)
    {
        struct stat stats;
        const char* file_path = path.c_str();
        
        if (stat(file_path,&stats) == 0 && S_ISDIR(stats.st_mode)) {
            return true;
        }

        return false;
    }

    b8 LinuxDirectory::Create(const String& path)
    {
        
        if (!Exists(path)) {
            if (mkdir(path.c_str(),0755)) {
                return true;
            }
        }

        return false;
    }

    b8 LinuxDirectory::Delete(const String& path)
    {
        if (!Exists(path)) {
            if (rmdir(path.c_str()) == 0) {
                return true;
            }
        }

        return false;
    }

    b8 LinuxDirectory::Copy(const String& source, const String& destination)
    {
        if (!Exists(source) || Exists(destination)) {
            return false;
        }
        int src = open(source.c_str(),O_RDONLY);
        if (src == -1) {
            perror("opening source dir");
            return false;
        }
        int dst = open(destination.c_str(),O_WRONLY | O_CREAT,0755);
        if (dst == -1) {
            perror("opening/creating destination dir");
            close(src);
            return false;
        }
        char buffer[BYTE_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(src,buffer,sizeof(buffer))) > 0) {
            ssize_t bytes_written = write(dst,buffer,bytes_read);
            if(bytes_read != bytes_written) {
                perror("Write error");
                close(src);
                close(dst);
                return false;
            }
        }
        if (bytes_read == -1) {
            close(src);
            close(dst);
            return false;
        }

        close(src);
        close(dst);
        return true;
    }

    b8 LinuxDirectory::Move(const String& source, const String& destination)
    {
        if (!Exists(source) || Exists(destination)) {
            return false;
        }
        const char* src = source.c_str();
        const char* dst = destination.c_str();
        if (rename(src,dst) == 0) {
            return true;
        }
        return false;
    }

    b8 LinuxDirectory::Rename(const String& source, const String& destination)
    {
        Move(source,destination);
    }

    b8 LinuxDirectory::GetFiles(const String& path, Vector<String>& files)
    {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != NULL) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_REG)
                files.push_back(d->d_name);
        }
        closedir(dir);
        return true;
    }

    b8 LinuxDirectory::GetDirectories(const String& path, Vector<String>& directories)
    {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != NULL) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_DIR)
                directories.push_back(d->d_name);
        }
        closedir(dir);
        return true;
    }

    b8 LinuxDirectory::GetFilesWithExtension(const String& path, const String& extension, Vector<String>& files)
    {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return false;
        }
        struct dirent *d;
        while ((d = readdir(dir)) != NULL) {
            if (d->d_name[0] == '.') continue;
            if (d->d_type == DT_REG && strncmp(extension.c_str(), d->d_name,sizeof(extension.c_str())) == 0)
                files.push_back(d->d_name);
        }
        closedir(dir);
        return true;
    }
}