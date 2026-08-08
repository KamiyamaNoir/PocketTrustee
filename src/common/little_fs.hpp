#ifndef POCKETTRUSTEE_LITTLE_FS_H
#define POCKETTRUSTEE_LITTLE_FS_H

#include "main.h"
#include "lfs.h"

class CoreLfs
{
    CoreLfs() = default;
    ~CoreLfs() = default;
public:
    CoreLfs(CoreLfs&) = delete;
    CoreLfs& operator=(CoreLfs&) = delete;

    static lfs_t * getInstance();

    static const char* interpret_error(int err)
    {
        /*
        * enum lfs_error {
            LFS_ERR_OK          = 0,    // No error
            LFS_ERR_IO          = -5,   // Error during device operation
            LFS_ERR_CORRUPT     = -84,  // Corrupted
            LFS_ERR_NOENT       = -2,   // No directory entry
            LFS_ERR_EXIST       = -17,  // Entry already exists
            LFS_ERR_NOTDIR      = -20,  // Entry is not a dir
            LFS_ERR_ISDIR       = -21,  // Entry is a dir
            LFS_ERR_NOTEMPTY    = -39,  // Dir is not empty
            LFS_ERR_BADF        = -9,   // Bad file number
            LFS_ERR_FBIG        = -27,  // File too large
            LFS_ERR_INVAL       = -22,  // Invalid parameter
            LFS_ERR_NOSPC       = -28,  // No space left on device
            LFS_ERR_NOMEM       = -12,  // No more memory available
            LFS_ERR_NOATTR      = -61,  // No data/attr available
            LFS_ERR_NAMETOOLONG = -36,  // File name too long
        };
         */
        switch (err)
        {
        case LFS_ERR_IO:
            return "Error during device operation";
        case LFS_ERR_CORRUPT:
            return "Corrupted";
        case LFS_ERR_NOENT:
            return "Not a directory";
        case LFS_ERR_EXIST:
            return "Entry already exists";
        case LFS_ERR_NOTDIR:
            return "Entry is not a directory";
        case LFS_ERR_ISDIR:
            return "Entry is a directory";
        case LFS_ERR_NOTEMPTY:
            return "Dir is not empty";
        case LFS_ERR_BADF:
            return "Bad file number";
        case LFS_ERR_FBIG:
            return "File too large";
        case LFS_ERR_INVAL:
            return "Invalid argument";
        case LFS_ERR_NOSPC:
            return "No space left on device";
        case LFS_ERR_NOMEM:
            return "Out of memory";
        case LFS_ERR_NOATTR:
            return "No attribute set";
        case LFS_ERR_NAMETOOLONG:
            return "Name too long";
        default:
            return "Unknown error";
        }
    }
};

class LfsFileGuard
{
public:
    ~LfsFileGuard()
    {
        if (opened_)
            lfs_file_close(lfs_, &instance);
    }

    int open(lfs_t * lfs, const char* path, int flags, const lfs_file_config* config)
    {
        lfs_ = lfs;
        int err = lfs_file_opencfg(lfs, &instance, path, flags, config);
        if (err >= 0)
            opened_ = true;
        return err;
    }

    lfs_file_t* operator->() {
        return &instance;
    }

    lfs_file_t operator*() const {
        return instance;
    }

    lfs_file_t instance {};
private:
    lfs_t * lfs_ {};
    bool opened_ = false;
};

class LfsDirectoryGuard
{
public:
    ~LfsDirectoryGuard()
    {
        if (opened_)
            lfs_dir_close(lfs_, &instance);
    }

    int open(lfs_t * lfs, const char* path)
    {
        lfs_ = lfs;
        int err = lfs_dir_open(lfs, &instance, path);
        if (err >= 0)
            opened_ = true;
        return err;
    }

    int count()
    {
        uint32_t current = lfs_dir_tell(lfs_, &instance);
        lfs_dir_rewind(lfs_, &instance);
        lfs_info info {};
        int count = 0;
        for (;;)
        {
            int err = lfs_dir_read(lfs_, &instance, &info);
            if (err > 0)
                count++;
            else if (err == 0)
            {
                lfs_dir_seek(lfs_, &instance, current);
                return ++count;
            }
            else
            {
                lfs_dir_seek(lfs_, &instance, current);
                return err;
            }
        }
    }

    lfs_dir_t instance {};
private:
    lfs_t * lfs_ {};
    bool opened_ = false;
};

#endif