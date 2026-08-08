#include "little_fs.hpp"
#include "driver_w25q16.hpp"

namespace {
    lfs_t lfs_w25q16;
}

lfs_t* CoreLfs::getInstance() {
    return &lfs_w25q16;
}
