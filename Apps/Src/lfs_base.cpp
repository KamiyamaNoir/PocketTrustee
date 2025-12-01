#include "lfs_base.h"
#include "bsp_flash.h"


// 将一个Sector作为一个Block供LFS使用，对于LFS来说一共512个Block
#define W25Q16_SECTOR_SIZE      4096
#define W25Q16_PAGE_SIZE        256

static int flash_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    const uint32_t addr = block * c->block_size + off;
    bsp_flash::read_data(addr, static_cast<uint8_t*>(buffer), size);
    return LFS_ERR_OK;
}

static int flash_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t addr = block * c->block_size + off;
    const auto* data = static_cast<const uint8_t*>(buffer);

    while (size > 0) {
        uint16_t write_len = size;
        uint32_t page_remain = W25Q16_PAGE_SIZE - (addr % W25Q16_PAGE_SIZE);

        if (write_len > page_remain) {
            write_len = page_remain;
        }
        bsp_flash::page_program(addr, data, write_len);
        while (bsp_flash::read_busy()) {}
        addr += write_len;
        data += write_len;
        size -= write_len;
    }

    return LFS_ERR_OK;
}

static int flash_erase(const struct lfs_config *c, lfs_block_t block)
{
    const uint32_t addr = block * c->block_size;
    bsp_flash::sector_erase(addr);
    while (bsp_flash::read_busy()) {}
    return LFS_ERR_OK;
}

static int flash_sync(const struct lfs_config *c)
{
    UNUSED(c);
    return LFS_ERR_OK;
}

static lfs_t fs_instance;

// __attribute__((section("._user_graphic_ram"))) static uint8_t prog_cache[256];
// __attribute__((section("._user_graphic_ram"))) static uint8_t read_cache[256];
// __attribute__((section("._user_graphic_ram"))) static uint8_t lookahead_cache[32];
// __attribute__((section("._user_graphic_ram"))) static uint8_t open_cache[256];
__aligned(4) static uint8_t prog_cache[256];
__aligned(4) static uint8_t read_cache[256];
__aligned(4) static uint8_t lookahead_cache[32];
__aligned(4) static uint8_t open_cache[256];

constexpr struct lfs_config config = {
    .read = flash_read,
    .prog = flash_write,
    .erase = flash_erase,
    .sync = flash_sync,

    .read_size = 1,
    .prog_size = 1,
    .block_size = 4096,
    .block_count = 512,
    .block_cycles = 1000,
    .cache_size = 256,
    .lookahead_size = 32,

    .read_buffer = read_cache,
    .prog_buffer = prog_cache,
    .lookahead_buffer = lookahead_cache,

    .name_max = 32,
    .file_max = 0x1FFFFF,
};

constexpr lfs_file_config open_config = {
    .buffer = open_cache,
};


int LittleFS::init()
{
    bsp_flash::reset_device();
    HAL_Delay(10);
    while (bsp_flash::read_busy()) {}
    int err = lfs_mount(&fs_instance, &config);
    if (err)
    {
        lfs_format(&fs_instance, &config);
    }
    return lfs_mount(&fs_instance, &config);
}

int LittleFS::fs_format()
{
    int err = lfs_format(&fs_instance, &config);
    if (err < 0) return err;
    return lfs_mount(&fs_instance, &config);
}

int LittleFS::fs_rename(const char* path, const char* new_path)
{
    return lfs_rename(&fs_instance, path, new_path);
}

int LittleFS::fs_remove(const char* path)
{
    return lfs_remove(&fs_instance, path);
}

int LittleFS::fs_read(const char* path, uint8_t* buffer, lfs_size_t size)
{
    lfs_file_t file;
    // int err = lfs_file_open(&fs_instance, &file, path, LFS_O_RDONLY);
    int err = lfs_file_opencfg(&fs_instance, &file, path, LFS_O_RDONLY, &open_config);
    if (err < 0) return err;
    err = lfs_file_read(&fs_instance, &file, buffer, size);
    lfs_file_close(&fs_instance, &file);
    return err;
}

uint32_t LittleFS::fs_getUsed()
{
    long int err = lfs_fs_size(&fs_instance);
    if (err < 0) return 0;
    return err;
}

uint32_t LittleFS::fs_getTotal()
{
    return config.block_count;
}

LittleFS::fs_file_handler::fs_file_handler(const char* path) : path(path)
{
    // lfs_file_open(&fs_instance, &file, path, LFS_O_CREAT | LFS_O_RDWR);
    lfs_file_opencfg(&fs_instance, &file, path, LFS_O_CREAT | LFS_O_RDWR, &open_config);
}

LittleFS::fs_file_handler::fs_file_handler(const char* path, int flags, int* err) : path(path)
{
    // lfs_file_open(&fs_instance, &file, path, flags);
    *err = lfs_file_opencfg(&fs_instance, &file, path, flags, &open_config);
}

LittleFS::fs_file_handler::~fs_file_handler()
{
    lfs_file_close(&fs_instance, &file);
}

int LittleFS::fs_file_handler::read(uint8_t* data, uint16_t size)
{
    return lfs_file_read(&fs_instance, &file, data, size);
}

int LittleFS::fs_file_handler::write(const uint8_t* data, uint16_t size)
{
    int err = lfs_file_write(&fs_instance, &file, data, size);
    if (err < 0) return err;
    lfs_file_sync(&fs_instance, &file);
    return err;
}

int LittleFS::fs_file_handler::seek(lfs_soff_t off)
{
    return lfs_file_seek(&fs_instance, &file, off, LFS_SEEK_SET);
}

int LittleFS::fs_file_handler::size()
{
    return lfs_file_size(&fs_instance, &file);
}

lfs_info LittleFS::fs_file_handler::stat() const
{
    lfs_info info {};
    lfs_stat(&fs_instance, path, &info);
    return info;
}

LittleFS::fs_dir_handler::fs_dir_handler(const char* path) : path(path)
{
    lfs_dir_open(&fs_instance, &dir, path);
}

LittleFS::fs_dir_handler::~fs_dir_handler()
{
    lfs_dir_close(&fs_instance, &dir);
}

int LittleFS::fs_dir_handler::rewind()
{
    return lfs_dir_rewind(&fs_instance, &dir);
}

uint32_t LittleFS::fs_dir_handler::tell()
{
    return lfs_dir_tell(&fs_instance, &dir);
}

int LittleFS::fs_dir_handler::seek(uint32_t off)
{
    return lfs_dir_seek(&fs_instance, &dir, off);
}

int LittleFS::fs_dir_handler::next(lfs_info* info)
{
    return lfs_dir_read(&fs_instance, &dir, info);
}

int LittleFS::fs_dir_handler::count()
{
    uint32_t current = tell();
    rewind();
    lfs_info info {};
    int count = 0;
    for (;;)
    {
        int err = lfs_dir_read(&fs_instance, &dir, &info);
        if (err > 0)
            count++;
        else if (err == 0)
        {
            seek(current);
            return ++count;
        }
        else
        {
            seek(current);
            return err;
        }
    }
}


int LittleFS::fs_dir_handler::list_str(char* dst, int max_size)
{
    dst[0] = '\0';
    lfs_info info {};
    for (;;)
    {
        int err = lfs_dir_read(&fs_instance, &dir, &info);
        if (err < 0) return err;
        if (err == 0) return 0;

        int size = static_cast<int>(strlen(info.name) + 2);
        if (max_size < size)
        {
            return 0;
        }
        if (8 <= max_size && max_size < size + 8)
        {
            strcat(dst, "\n(more)");
            return 0;
        }
        strcat(dst, info.name);
        if (info.type == LFS_TYPE_DIR)
        {
            strcat(dst, "/");
        }
        strcat(dst, "\n");
    }
}

int LittleFS::fs_dir_handler::mkdir(const char* path)
{
    return lfs_mkdir(&fs_instance, path);
}



