#include <cstring>
#include "gui_lock.hpp"
#include "pin_code.hpp"
#include "gui_mainpage.hpp"

#define GUI_LOCK_CTRNUM 0
#define GUI_PIN_CTRNUM 1

#define IS_NUM(x) ((48 <= x && x <= 57) || x == 'X')

using namespace gui;

char gui_pin_input[6];
static Window* target_window = nullptr;

void cb_backto_gui_pin(Window& wn, Display& dis, ui_operation& opt);

static void clickon_pin_input(Window& wn, Display& dis, ui_operation& opt);

static void render_pin(Scheme& sche, Control& self, bool onSelect);

static uint8_t input_index = 0;

static ResourceDescriptor res_lock
{
    .path = "gui_lock/locked",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
static ResourceDescriptor res_pin
{
    .path = "gui_lock/pin",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

static Control controls_pin[GUI_PIN_CTRNUM]
{
    {59, 48, 16, 32, true, clickon_pin_input, render_pin}
};

Window wn_locked(&res_lock, nullptr, GUI_LOCK_CTRNUM);
Window wn_pin(&res_pin, controls_pin, GUI_PIN_CTRNUM);

void cb_backto_gui_pin(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_pin);
    dis.refresh_count = 0;
}

void clickon_pin_input(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (gui_pin_input[input_index] == 'X')
        {
            if (input_index > 0)
                input_index--;
        }
        else if (input_index < 5)
        {
            input_index++;
        }
        else
        {
            // authencate
            bool success = PIN_CODE::verifyPinCode(gui_pin_input);
            if (!success)
            {
                dis.switchFocusLag(&wn_locked);
                dis.refresh_count = 0;
                return;
            }
            memset(gui_pin_input, '0', sizeof(gui_pin_input));
            input_index = 0;
            if (target_window != nullptr)
            {
                dis.switchFocusLag(target_window);
                dis.refresh_count = 0;
                target_window = nullptr;
            }
            else
            {
                dis.switchFocusLag(&wn_cds);
                dis.refresh_count = 0;
            }
        }
        return;
    }
    gui_pin_input[input_index] += opt;
    if (gui_pin_input[input_index] == '9'+1 || gui_pin_input[input_index] == '0'-1) gui_pin_input[input_index] = 'X';
    else if (gui_pin_input[input_index] == 'X'+1) gui_pin_input[input_index] = '0';
    else if (gui_pin_input[input_index] == 'X'-1) gui_pin_input[input_index] = '9';
}

void render_pin(Scheme& sche, Control& self, bool onSelect)
{
    if (!IS_NUM(gui_pin_input[input_index])) gui_pin_input[input_index] = '0';
    for (int i = 0; i < input_index + 1; i++)
    {
        if (IS_NUM(gui_pin_input[i]))
        {
            char tp[2] = {gui_pin_input[i], '\0'};
            sche.put_string(self.x + i*32, self.y, ASCII_3216, tp);
            continue;
        }
        break;
    }
    sche.invert(self.x + input_index*32 - 1, self.y - 1, 18, 34);
}

void register_pin_window(Window* wn)
{
    target_window = wn;
}
