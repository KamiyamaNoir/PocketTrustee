#ifndef _GUI_COMPONENT_LIST_H
#define _GUI_COMPONENT_LIST_H

#include "lfs_base.h"

template<uint8_t PAGE_SIZE, uint8_t NAME_MAX>
class ComponentList
{
public:
    ComponentList(const char* base_dir_path, const char* file_suffix) :
    base_path(base_dir_path), file_suffix(file_suffix), suffix_len(strlen(file_suffix)) {}

    int update()
    {
        auto fs_dir = LittleFS::fs_dir_handler(base_path);
        int dir_count = fs_dir.count();
        _item_count = 0;
        fs_dir.rewind();
        uint8_t pstart = (_item_index / PAGE_SIZE) * PAGE_SIZE;
        char (*pcache)[NAME_MAX] = _name_cache;
        for (uint32_t i = 0; i < dir_count; i++)
        {
            //NOLINTNEXTLINE
            lfs_info info;
            fs_dir.next(&info);
            if (info.type == LFS_TYPE_DIR) continue;
            uint32_t len = strlen(info.name);
            if (len < suffix_len) continue;
            if (memcmp(info.name + len - suffix_len, file_suffix, strlen(file_suffix)) != 0)
                continue;
            if (pstart <= _item_count && _item_count < pstart + PAGE_SIZE)
            {
                char name[NAME_MAX];
                strcpy(name, info.name);
                name[len - suffix_len] = '\0';
                strcpy(*pcache, name);
                ++pcache;
            }
            _item_count++;
        }
        // clear all unused name cache
        while (pcache - _name_cache < PAGE_SIZE)
        {
            (*pcache)[0] = '\0';
            ++pcache;
        }
        if (_item_index >= _item_count)
            _item_index = 0;
        return 0;
    }

    bool move(int step)
    {
        bool _require_update = false;
        if (_item_count == 0)
            return true;
        int destination = _item_index + step;
        if (destination < 0)
            destination = 0;
        else if (destination >= _item_count)
            destination = _item_count - 1;

        if (_item_index / PAGE_SIZE != destination / PAGE_SIZE)
            _require_update = true;
        _item_index = destination;
        return _require_update;
    }

    void set_index(int index)
    {
        if (index < 0)
        {
            _item_index = _item_count - 1;
        }
        else if (index < _item_count)
        {
            _item_index = index;
        }
    }

    const char* seek(int index) const
    {
        if (index < 0)
            return nullptr;
        return _name_cache[index];
    }

    uint32_t item_count() const
    {
        return _item_count;
    }

    uint8_t page_count() const
    {
        if (_item_count == 0)
            return 0;
        return (_item_count-1) / PAGE_SIZE + 1;
    }

    uint8_t page_index() const
    {
        return _item_index / PAGE_SIZE + 1;
    }

    int on_select() const
    {
        if (_item_count == 0)
            return -1;
        return _item_index % PAGE_SIZE;
    }

    const char* base_path;
    const char* file_suffix;
    const uint8_t suffix_len;
private:
    uint32_t _item_count = 0;
    uint32_t _item_index = 0;
    uint32_t _page = 0;
    char _name_cache[PAGE_SIZE][NAME_MAX] {};
};

#endif