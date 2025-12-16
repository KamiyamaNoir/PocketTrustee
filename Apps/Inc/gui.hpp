#ifndef APP_GUI_H
#define APP_GUI_H

#include "main.h"
#include "cmsis_os.h"

#define GUI_WIDTH 296
#define GUI_HEIGHT 128
#define GUI_ARRAY 4736

#define GUI_HANDLER defaultTaskHandle

#define GUI_OP_UP() xTaskNotify(GUI_HANDLER, 1, eSetValueWithoutOverwrite)
#define GUI_OP_DOWN() xTaskNotify(GUI_HANDLER, 3, eSetValueWithoutOverwrite)
#define GUI_OP_ENTER() xTaskNotify(GUI_HANDLER, 4, eSetValueWithoutOverwrite)
#define GUI_OP_NULL() xTaskNotify(GUI_HANDLER, 2, eSetValueWithoutOverwrite)

#define GUI_OP_UP_ISR() xTaskNotifyFromISR(GUI_HANDLER, 1, eSetValueWithoutOverwrite, nullptr)
#define GUI_OP_DOWN_ISR() xTaskNotifyFromISR(GUI_HANDLER, 3, eSetValueWithoutOverwrite, nullptr)
#define GUI_OP_ENTER_ISR() xTaskNotifyFromISR(GUI_HANDLER, 4, eSetValueWithoutOverwrite, nullptr)
#define GUI_OP_NULL_ISR() xTaskNotifyFromISR(GUI_HANDLER, 2, eSetValueWithoutOverwrite, nullptr)

namespace gui
{
    class Window;
    class Scheme;
    class Display;


    enum GUI_FONT
    {
        ASCII_1608,
        ASCII_3216
    };

    enum ui_operation
    {
        OP_UP = -1,
        OP_NULL = 0,
        OP_DOWN = 1,
        OP_ENTER = 2,
    };

    struct EPD_Handler
    {
        void (*full_update)(const uint8_t* data);
        void (*fast_update)(const uint8_t* data);
        void (*part_update)(const uint8_t* data);
    };

    struct ResourceDescriptor
    {
        const char* path;
        uint16_t rx, ry, rw, rh;
    };

    struct Control
    {
        uint16_t x, y, w, h;
        bool selectable;
        void (*onClick)(Window& wn, Display& dis, ui_operation& opt);
        void (*onRender)(Scheme& sche, Control& self, bool onSelect);
    };

    class Scheme
    {
    public:
        Scheme(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Display& dis, uint8_t* buffer);
        Scheme(ResourceDescriptor& res, Display& dis, uint8_t* buffer) :
            Scheme(res.rx, res.ry, res.rw, res.rh, dis, buffer) {}
        ~Scheme() = default;
        Scheme& dot(uint16_t x, uint16_t y, bool val);
        Scheme& dot_nocheck(uint16_t x, uint16_t y, bool val);
        bool getPixel(uint16_t x, uint16_t y) const;
        Scheme& fill(const uint8_t* buffer);
        Scheme& texture(const uint8_t* texture, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        Scheme& line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
        Scheme& rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        Scheme& rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t thick);
        Scheme& wrap(uint8_t thickness);
        Scheme& invert(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        Scheme& put_string(uint16_t x, uint16_t y, GUI_FONT font, const char* str);
        Scheme& cast(const Scheme& src);
        Scheme& accurate_cast(const Scheme& src);
        Scheme& clear();

        uint8_t cy, ch;
        uint16_t rx, rw, ry, rh;
        uint8_t* data;
    };

    class Display
    {
        friend void GUI_Task();
    public:
        Display(uint16_t width, uint16_t height, const EPD_Handler* handler, uint8_t* render_cache, uint8_t* load_cache) :
            width(width), height(height), epd(handler), load_cache(load_cache),
            refresh_cache(0, 0, width, height,*this, render_cache)
        {
            refresh_cache.clear();
        }
        ~Display() = default;

        // void process(ui_operation operation);

        void switchFocusLag(Window* wn);

        const uint16_t width, height;
        uint8_t refresh_count = 0;
        const EPD_Handler* epd;
        uint8_t* load_cache;
    private:
        void process(ui_operation operation);
        Scheme refresh_cache;
        Window* target = nullptr;
        Window* focus = nullptr;
    };

    class Window
    {
    public:
        Window(ResourceDescriptor* res, Control* controls, uint8_t count) :
        res(res), ctr_list(controls), ctr_count(count)
        {
            ctr_ptr = 128 - (128 % ctr_count);
            cy = res->ry >= GUI_HEIGHT ? GUI_HEIGHT/8 : res->ry/8;
            ch = res->ry + res->rh >= GUI_HEIGHT ? (GUI_HEIGHT-res->ry-1)/8 + 1 : (res->rh-1)/8 + 1;
        }
        ~Window() = default;

        void load(Display& dis) const;
        void resetPtr();
        void interact(Display& dis, ui_operation opt);
        void render(Display& dis, Scheme& sche) const;

        ResourceDescriptor* res;
        Control* ctr_list;
        uint8_t ctr_count = 0;
    private:
        uint8_t cy, ch;
        uint8_t ctr_ptr = 0;
    };

    void GUI_Task();

    /* Public Function */
    void render_rectangle(Scheme& sche, Control& self, bool onSelect);
}
#endif //APP_GUI_H
