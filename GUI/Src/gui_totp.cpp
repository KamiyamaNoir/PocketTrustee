#include "gui_totp.hpp"
#include "gui_mainpage.hpp"
#include "gui_resource.h"
#include "otp.hpp"
#include <cstring>
#include "lfs_base.h"
#include "gui_component_list.h"

#define GUI_TOTP_SEL_CTRNUM 2
#define GUI_TOTP_CTRNUM 1

#define TOTP_SEL_PAGE_SIZE 5
#define TOTP_SEL_PAGE_MAX 1

using namespace gui;

static void clickon_totp_sel_exit(Window& wn, Display& dis, ui_operation& opt);
static void clickon_totp_sel(Window& wn, Display& dis, ui_operation& opt);
static void clickon_totp_exit(Window& wn, Display& dis, ui_operation& opt);

static void render_totp_sel(Scheme& sche, Control& self, bool onSelect);
static void render_totp(Scheme& sche, Control& self, bool onSelect);

static Control controls_totp_sel[GUI_TOTP_SEL_CTRNUM]
{
    {229, 1, 64, 24, true, clickon_totp_sel_exit, render_totp_sel},
    {0, 0, 1, 1, true, clickon_totp_sel, nullptr}
};
static Control controls_totp[GUI_TOTP_CTRNUM]
{
    {229, 1, 64, 24, true, clickon_totp_exit, render_totp}
};
static ResourceDescriptor res_totp_sel
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
static ResourceDescriptor res_totp
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_totp_sel(&res_totp_sel, controls_totp_sel, GUI_TOTP_SEL_CTRNUM);
Window wn_totp(&res_totp, controls_totp, GUI_TOTP_CTRNUM);

ComponentList<TOTP_SEL_PAGE_SIZE, OTP_TOTP::TOTP_NAME_MAX> totp_list(OTP_TOTP::totp_dir_base, OTP_TOTP::totp_suffix);

void totp_dir_update()
{
    totp_list.update();
}

void clickon_totp_sel_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_totp_sel(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        dis.switchFocusLag(&wn_totp);
        dis.refresh_count = 0;
        return;
    }
    if (!(opt == OP_UP && totp_list.on_select() == 0) && !(opt == OP_DOWN && totp_list.on_select() == totp_list.item_count()-1))
    {
        totp_list.move(opt);
        opt = OP_NULL;
    }
    else
    {
        totp_list.set_index(0);
    }
}

void clickon_totp_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_totp_sel);
    dis.refresh_count = 0;
}

void render_totp_sel(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear();
    if (totp_list.item_count() == 0)
    {
        sche.put_string(92, 56, ASCII_1608, "Nothing Here :(");
        return;
    }
    // Display icon
    sche.texture(icon_bk_menu, self.x, self.y, self.w, self.h);
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
    // Display item
    for (uint8_t i = 0; i < TOTP_SEL_PAGE_SIZE; i++)
    {
        sche.put_string(4, 5+20*i, ASCII_1608, totp_list.seek(i));
        if (i == totp_list.on_select() && !onSelect)
        {
            sche.rectangle(3, 5+20*i, 201, 17, 1);
        }
    }
}

void render_totp(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear()
        .texture(icon_bk_menu, self.x, self.y, self.w, self.h)
        .put_string(2, 2, ASCII_3216, "TOTP");
    auto totp = OTP_TOTP(totp_list.seek(totp_list.on_select()));
    uint8_t off =  GUI_WIDTH/2 - 4*strlen(totp.getName());
    sche.rectangle(68, 93, 160, 2)
        .put_string(off, 36, ASCII_1608, totp.getName());
    uint32_t calculate = 0;
    int result = totp.calculate(&calculate);
    if (result == 0)
    {
        char text[7] = {
            static_cast<char>((calculate % 1000000UL) / 100000),
            static_cast<char>((calculate % 100000UL) / 10000),
            static_cast<char>((calculate % 10000UL) / 1000),
            static_cast<char>((calculate % 1000UL) / 100),
            static_cast<char>((calculate % 100UL) / 10),
            static_cast<char>((calculate % 10UL) / 1),
        };
        for (auto& i : text)
        {
            i += 48;
        }
        text[6] = '\0';
        sche.put_string(100, 60, ASCII_3216, text);
    }
}
