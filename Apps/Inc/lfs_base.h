#ifndef APP_LFS_BASE_H
#define APP_LFS_BASE_H

#include "lfs.h"

namespace LittleFS
{
    class fs_file_handler
    {
    public:
        explicit fs_file_handler(const char* path);
        fs_file_handler(const char* path, int flags, int* err);
        ~fs_file_handler();

        int write(const uint8_t* data, uint16_t size);
        int read(uint8_t* data, uint16_t size);
        int size();
        lfs_info stat() const;
        int seek(lfs_soff_t off);

        lfs_file_t file {};
        const char* path;
    };

    class fs_dir_handler
    {
    public:
        explicit fs_dir_handler(const char* path);
        ~fs_dir_handler();

        int rewind();
        uint32_t tell();
        int seek(uint32_t off);
        int next(lfs_info* info);

        int count();

        int list_str(char* dst, int max_size);
        static int mkdir(const char* path);

        lfs_dir_t dir {};
        const char* path;
    };

    int init();
    int fs_format();
    int fs_rename(const char* path, const char* new_path);
    int fs_remove(const char* path);
    int fs_read(const char* path, uint8_t* buffer, lfs_size_t size);
    uint32_t fs_getUsed();
    uint32_t fs_getTotal();
}

#endif //APP_LFS_BASE_H
