#include "gui.h"
#include "bsp_epaper.h"
#include <cstring>
#include "gui_resource.h"
#include "lfs_base.h"
#include "cmsis_os.h"

using namespace gui;

/* =============USER CODE BEGIN GUI_RESOURCE================= */
GRAPHIC_RAM_AREA uint8_t gram_cache[GUI_ARRAY];
GRAPHIC_RAM_AREA uint8_t gram_end[GUI_ARRAY];

static EPD_Handler handler {
    .full_update = epaper::pre_update,
    .fast_update = epaper::update_fast,
    .part_update = epaper::update_part_full
};

void render_rectangle(Scheme& sche, Control& self, bool onSelect)
{
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
}

Display gui_main(GUI_WIDTH, GUI_HEIGHT, &handler, gram_end, gram_cache);

extern Window wn_cds;

extern osThreadId defaultTaskHandle;

void gui::GUI_Task()
{
    uint32_t operation = 0;
    gui_main.switchFocusLag(&wn_cds);
    gui_main.process(OP_NULL);
    for (;;)
    {
        operation = 0;
        xTaskNotifyWait(0xFFFFFFFFUL, 0xFFFFFFFFUL, &operation, portMAX_DELAY);
        xTaskNotifyStateClear(nullptr);
        if (operation < 5)
        {
            gui_main.process(static_cast<gui::ui_operation>(operation - 2));
        }
    }
}
/* =========USER CODE END GUI_RESOURCE========== */

__STATIC_INLINE int my_abs(const int x)
{
    return x < 0 ? -x : x;
}

__STATIC_INLINE uint16_t my_max(const uint16_t a, const uint16_t b)
{
    return a > b ? a : b;
}

__STATIC_INLINE uint16_t my_min(const uint16_t a, const uint16_t b)
{
    return a < b ? a : b;
}

Scheme::Scheme(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Display& dis, uint8_t* buffer)
{
    // 垂直刷新，x轴无需对齐一字节
    rx = x >= dis.width ? dis.width : x;
    rw = x+w >= dis.width ? dis.width-x : w;
    // c for coefficient, when cw=1, it refers to as 8 pixels
    cy = y >= dis.height ? dis.height/8 : y/8;
    ry = y >= dis.height ? dis.height : y;
    ch = y+h >= dis.height ? (dis.height-y-1)/8 + 1 : (h-1)/8 + 1;
    rh = y+h >= dis.height ? dis.height-y : h;
    data = buffer;
}

/**
 * Draw point using absolute position
 * Points out of scheme would be ignored
 * @param x absolute x
 * @param y absolute y
 * @param val true for black point
 * @return
 */
Scheme& Scheme::dot(uint16_t x, uint16_t y, bool val)
{
    if (rx <= x && x <= rx + rw)
    {
        if (cy*8 <= y && y <= cy*8 + ch*8)
        {
            if (val)
                this->data[(y-cy*8)/8 + (x-rx)*ch] &= ~(static_cast<uint8_t>(0x80) >> (y % 8));
            else
                this->data[(y-cy*8)/8 + (x-rx)*ch] |= (static_cast<uint8_t>(0x80) >> (y % 8));
        }
    }
    return *this;
}

Scheme& Scheme::dot_nocheck(uint16_t x, uint16_t y, bool val)
{
    if (val)
        this->data[(y-cy*8)/8 + (x-rx)*ch] &= ~(static_cast<uint8_t>(0x80) >> (y % 8));
    else
        this->data[(y-cy*8)/8 + (x-rx)*ch] |= (static_cast<uint8_t>(0x80) >> (y % 8));
    return *this;
}

bool Scheme::getPixel(uint16_t x, uint16_t y) const
{
    return !(data[(y-cy*8)/8 + (x-rx)*ch] & (static_cast<uint8_t>(0x80) >> ((y-cy*8) % 8)));
}

Scheme& Scheme::fill(const uint8_t* buffer)
{
    memcpy(this->data, buffer, rw*ch);
    return *this;
}

Scheme& Scheme::texture(const uint8_t* texture, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint8_t srch = h/8;
    for (uint16_t i = x; i < x+w; i++)
    {
        for (uint16_t j = y; j < y+h; j++)
        {
            if (texture[(j-y)/8 + (i-x)*srch] & (static_cast<uint8_t>(0x80) >> ((j-y) % 8)))
                dot_nocheck(i, j, false);
            else
                dot_nocheck(i, j, true);
        }
    }
    return *this;
}


Scheme& Scheme::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (x1 == x2 && y1 == y2) {
        dot(x1, y1, true);
        return *this;
    }

    int current_x = x1;
    int current_y = y1;
    int dx = my_abs(x1 - x2);
    int dy = my_abs(y1 - y2);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;

    if (dx > dy) {
        int err = dx / 2;
        for (int i = 0; i <= dx; i++) {
            dot(static_cast<uint16_t>(current_x), static_cast<uint16_t>(current_y), true);
            err -= dy;
            if (err < 0) {
                current_y += sy;
                err += dx;
            }
            current_x += sx;
        }
    } else {
        int err = dy / 2;
        for (int i = 0; i <= dy; i++) {
            dot(static_cast<uint16_t>(current_x), static_cast<uint16_t>(current_y), true);
            err -= dx;
            if (err < 0) {
                current_x += sx;
                err += dy;
            }
            current_y += sy;
        }
    }
    return *this;
}

/**
 * Solid rectangle
 * @param x absolute x
 * @param y absolute y
 * @param w width
 * @param h height
 * @return
 */
Scheme& Scheme::rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // As the minimal value of w is 1, position x+w is unreachable
    for (uint16_t i = x; i < x + w; i++)
    {
        // same as h
        for (uint16_t j = y; j < y + h; j++)
        {
            dot(i, j, true);
        }
    }
    return *this;
}

// Clang-Tidy would show a warning when using recursion
// NOLINTNEXTLINE
Scheme& Scheme::rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t thick)
{
    if (thick == 0) return *this;
    for (uint16_t i = x; i < x + w; i++)
    {
        dot(i, y, true);
        dot(i, y+h-1, true);
    }
    for (uint16_t j = y; j < y + h; j++)
    {
        dot(x, j, true);
        dot(x+w-1, j, true);
    }
    return rectangle(x+1, y+1, w-2, h-2, thick-1);
}

/**
 * Wrap the border of scheme with rectangle
 * @param thickness
 * @return
 */
Scheme& Scheme::wrap(uint8_t thickness)
{
    return rectangle(rx, cy*8, rw, ch*8, thickness);
}

Scheme& Scheme::invert(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    for (uint16_t i = x; i < x + w; i++)
        for (uint16_t j = y; j < y + h; j++)
            dot_nocheck(i, j, !getPixel(i, j));
    return *this;
}


Scheme& Scheme::put_string(uint16_t x, uint16_t y, GUI_FONT font, const char* str)
{
    uint8_t width, height;
    const uint8_t* font_base;
    if (font == ASCII_1608)
    {
        width = 8;height=16;
        font_base = ASCII_FONT_1608;
    }
    else
    {
        width = 16;height=32;
        font_base = ASCII_FONT_3216;
    }
    for (uint16_t i = 0;;i++)
    {
        uint8_t chr = str[i];
        if (chr == '\0') break;
        chr -= 32;
        const uint8_t* bitmap = font_base + chr*height*width/8;
        const uint8_t ch = height/8;
        for (uint8_t x_pos = 0; x_pos < width; x_pos++)
        {
            for (uint8_t y_pos = 0; y_pos < height; y_pos++)
            {
                dot_nocheck(x_pos + x + i*width, y_pos + y,
                    !(bitmap[y_pos/8 + x_pos*ch] & (static_cast<uint8_t>(0x80) >> (y_pos % 8))));
            }
        }
    }
    return *this;
}

Scheme& Scheme::cast(const Scheme& src)
{
    uint16_t tx1 = my_max(rx, src.rx);
    uint8_t ty1 = my_max(cy, src.cy);
    uint16_t tx2 = my_min(rx+rw, src.rx+src.rw);
    uint8_t ty2 = my_min(cy+ch, src.cy+src.ch);
    for (uint16_t i = tx1; i < tx2; i++)
    {
        for (uint8_t j = ty1; j < ty2; j++)
        {
            data[(j-cy) + (i-rx)*ch] = src.data[(j-src.cy) + (i-src.rx)*src.ch];
        }
    }
    return *this;
}

Scheme& Scheme::accurate_cast(const Scheme& src)
{
    uint16_t tx1 = my_max(rx, src.rx);
    uint8_t ty1 = my_max(ry, src.ry);
    uint16_t tx2 = my_min(rx+rw, src.rx+src.rw);
    uint8_t ty2 = my_min(ry+rh, src.ry+src.rh);
    for (uint16_t i = tx1; i < tx2; i++)
    {
        for (uint16_t j = ty1; j < ty2; j++)
        {
            dot_nocheck(i, j, src.getPixel(i, j));
        }
    }
    return *this;
}


Scheme& Scheme::clear()
{
    memset(data, 0xFF, rw*ch);
    return *this;
}

void Display::switchFocusLag(Window* wn)
{
    target = wn;
}

void Display::process(ui_operation operation)
{
    if (target != nullptr)
    {
        focus = target;
        focus->load(*this);
        target = nullptr;
    }
    if (focus == nullptr) return;

    auto body = Scheme(*focus->res, *this, load_cache);

    refresh_cache.cast(body);

    focus->interact(*this, operation);
    // NOLINTBEGIN
    if (target != nullptr)
    {
        focus = target;
        focus->load(*this);
        body = Scheme(*focus->res, *this, load_cache);
        refresh_cache.cast(body);
        target = nullptr;
    }
    // NOLINTEND
    focus->render(*this, refresh_cache);

    if (refresh_count > 10)
    {
        epd->fast_update(refresh_cache.data);
        refresh_count = 1;
    }
    else if (refresh_count != 0)
    {
        epd->part_update(refresh_cache.data);
        refresh_count++;
    }
    else
    {
        epd->full_update(refresh_cache.data);
        refresh_count = 1;
    }
}


void Window::load(Display& dis) const
{
    if (res->path != nullptr)
        LittleFS::fs_read(res->path, dis.load_cache, res->rw*ch);
    else
        memset(dis.load_cache, 0xFF, res->rw*ch);
}

void Window::resetPtr()
{
    ctr_ptr = 128 - (128 % ctr_count);
}

void Window::interact(Display& dis, ui_operation opt)
{
    // when resource numer is zero, it is unnecessary to react with any operation
    if (ctr_count == 0) return;

    auto* res = &ctr_list[ctr_ptr % ctr_count];

    if (res->onClick != nullptr) res->onClick(*this, dis, opt);

    if (opt != OP_ENTER)
    {
        ctr_ptr += opt;
        res = &ctr_list[ctr_ptr % ctr_count];
        // check if resource is selectable
        if (res->selectable) return;
        uint8_t dest = ctr_ptr + opt*ctr_count;
        for (ctr_ptr+=opt; opt*(ctr_ptr - dest) < 0; ctr_ptr+=opt)
        {
            res = &ctr_list[ctr_ptr % ctr_count];
            if (res->selectable) break;
        }
    }
}


void Window::render(Display& dis, Scheme& sche) const
{
    // when res_count is zero, program would skip this loop, %0 would not be executed
    for (uint8_t i = 0; i < ctr_count; i++)
    {
        auto& item = ctr_list[i];
        if (item.onRender == nullptr) continue;
        if (i == ctr_ptr % ctr_count)
        {
            // if this resource is unselectable, then it is not expected to react with parameter "selected"
            // Due to a situation when there are many resources but none of them is selectable,
            // res_ptr could be set to the last item of res_list
            item.onRender(sche, item, true);
            continue;
        }
        item.onRender(sche, item, false);
    }
}
