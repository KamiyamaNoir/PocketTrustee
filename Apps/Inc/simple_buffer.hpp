#ifndef SIMPLE_BUFFER_H
#define SIMPLE_BUFFER_H

#include "main.h"
#include <cstring>

class StaticRingBuffer
{
public:
    explicit StaticRingBuffer(uint8_t* buffer, const uint16_t size) : _size(size), _buffer(buffer) {}

    uint16_t write(const uint8_t* data, const uint16_t size, bool eof)
    {
        uint16_t nfree = get_free();

        uint16_t nwrite = nfree >= size ? size : nfree;

        uint16_t avaliable = _size - _write_ptr;

        if (avaliable >= nwrite)
        {
            memcpy(_buffer + _write_ptr, data, nwrite);
            _write_ptr += nwrite;
        }
        else
        {
            memcpy(_buffer + _write_ptr, data, avaliable);
            memcpy(_buffer, data + avaliable, nwrite - avaliable);
            _write_ptr = nwrite - avaliable;
        }
        end_of_file = eof;
        return nwrite;
    }

    uint16_t read(uint8_t* dst, const uint16_t size)
    {
        uint16_t used = get_used();

        uint16_t nread = used >= size ? size : used;

        uint16_t avaliable = _size - _read_ptr;

        if (avaliable >= nread)
        {
            memcpy(dst, _buffer + _read_ptr, nread);
            _read_ptr += nread;
        }
        else
        {
            memcpy(dst, _buffer + _read_ptr, avaliable);
            memcpy(dst + avaliable, _buffer, nread - avaliable);
            _read_ptr = nread - avaliable;
        }
        end_of_file = true;
        return nread;
    }

    bool is_full() const
    {
        return (_read_ptr == (_write_ptr + 1) % _size);
    }

    bool is_empty() const
    {
        return _read_ptr == _write_ptr;
    }

    bool is_eof() const
    {
        return end_of_file;
    }

    uint16_t get_used() const
    {
        int len = _write_ptr - _read_ptr;
        return len >= 0 ? len : (_size + len);
    }

    uint16_t get_free() const
    {
        int len = _write_ptr - _read_ptr;
        return len >= 0 ? _size - 1 - len : -1 - len;
    }

    void clear()
    {
        _write_ptr = _read_ptr = 0;
    }

    const uint16_t _size;

private:
    uint16_t _read_ptr = 0;
    uint16_t _write_ptr = 0;
    uint8_t * _buffer;
    volatile bool end_of_file = true;
};

class StaticPollBuffer
{
public:
    explicit StaticPollBuffer(uint8_t* poll, const uint16_t size) :
        _size(size), _buffer(poll) {}

    bool write(const uint8_t* data, const uint16_t size)
    {
        if (size > _size) return false;
        _available = size;
        memcpy(_buffer, data, size);
        return true;
    }

    void write(const uint16_t size)
    {
        _available = size;
    }

    bool is_empty() const
    {
        return _available == 0;
    }

    void read(uint8_t* dst, uint16_t size)
    {
        if (size == 0) size = _size;
        memcpy(dst, _buffer, size);
        _available = 0;
    }

    void read()
    {
        _available = 0;
    }

    const uint16_t _size;
    uint16_t _available = 0;
    uint8_t* _buffer;
};


#endif
