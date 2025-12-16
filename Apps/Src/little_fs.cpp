#include "little_fs.hpp"
#include "bsp_flash.h"

// 将一个Sector作为一个Block供LFS使用，对于LFS来说一共512个Block
#define W25Q16_SECTOR_SIZE      4096
#define W25Q16_PAGE_SIZE        256
#define W25Q16_FS_CACHE_SIZE    256
#define W25Q16_FS_LOOKAHEAD_SIZE 32

static int w25q16_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    const uint32_t addr = block * c->block_size + off;
    bsp_flash::read_data(addr, static_cast<uint8_t*>(buffer), size);
    return LFS_ERR_OK;
}

static int w25q16_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
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

static int w25q16_erase(const struct lfs_config *c, lfs_block_t block)
{
    const uint32_t addr = block * c->block_size;
    bsp_flash::sector_erase(addr);
    while (bsp_flash::read_busy()) {}
    return LFS_ERR_OK;
}

static int w25q16_sync(const struct lfs_config *c)
{
    UNUSED(c);
    return LFS_ERR_OK;
}

__aligned(4) static uint8_t w25q16_prog_cache[W25Q16_FS_CACHE_SIZE];
__aligned(4) static uint8_t w25q16_read_cache[W25Q16_FS_CACHE_SIZE];
__aligned(4) static uint8_t w25q16_lookahead_cache[W25Q16_FS_LOOKAHEAD_SIZE];

lfs_t fs_w25q16;

constexpr lfs_config fs_w25q16_config = {
    .read = w25q16_read,
    .prog = w25q16_write,
    .erase = w25q16_erase,
    .sync = w25q16_sync,

    .read_size = 1,
    .prog_size = 1,
    .block_size = W25Q16_SECTOR_SIZE,
    .block_count = W25Q16_SECTOR_SIZE / W25Q16_PAGE_SIZE,
    .block_cycles = 1000,
    .cache_size = W25Q16_FS_CACHE_SIZE,
    .lookahead_size = W25Q16_FS_LOOKAHEAD_SIZE,

    .read_buffer = w25q16_read_cache,
    .prog_buffer = w25q16_prog_cache,
    .lookahead_buffer = w25q16_lookahead_cache,

    .name_max = 32,
    .file_max = 0x1FFFFF,
};

int LittleFS_W25Q16::Mount()
{
    return lfs_mount(&fs_w25q16, &fs_w25q16_config);
}

int LittleFS_W25Q16::Format()
{
    return lfs_format(&fs_w25q16, &fs_w25q16_config);
}
