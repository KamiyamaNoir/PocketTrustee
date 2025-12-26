#include "bsp_adc.h"
#include "gui_mainpage.hpp"
#include "gui_menup1.hpp"
#include "bsp_nfc.h"
#include "gui_resource.h"
#include "gui_idcard.hpp"
#include "gui_pwdgen.hpp"
#include "gui_pwdfill.hpp"
#include "gui_totp.hpp"
#include "gui_transmode.hpp"
#include "gui_manager_mode.hpp"
#include "gui_cards.hpp"
#include <cstdio>

#define GUI_MAINPAGE_CTRNUM 10
#define GUI_ICCARD_CTRNUM 4

using namespace gui;

static void clickon_cds_ic(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_id(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_pwdgen(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_pwdfill(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_totp(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_cards(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_transparent(Window& wn, Display& dis, ui_operation& opt);
static void clickon_cds_host(Window& wn, Display& dis, ui_operation& opt);
static void clickon_boxcard(Window& wn, Display& dis, ui_operation& opt, int args);

static void render_cds_texture(Scheme& sche, Control& self, bool onSelect, int args);
static void render_cds_icon(Scheme& sche, Control& self, bool onSelect);

static Control controls_card_selection[GUI_MAINPAGE_CTRNUM]
{
    // Quick Tools
    {7, 23, 100,23, true, clickon_cds_pwdgen, render_rectangle},
    {7, 44, 100, 23, true, clickon_cds_pwdfill, render_rectangle},
    {7, 62, 100,23, true, clickon_cds_totp, render_rectangle},
    {7, 82, 100,23, true, clickon_cds_id, render_rectangle},
    // IC Card
    {143, 29, 126,28, true, clickon_cds_ic, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, -1);}},
    // Tools
    {139, 65, 34, 37, true, clickon_cds_cards, render_rectangle},
    {175, 65, 34, 37, true, clickon_cds_transparent, render_rectangle},
    {209, 65, 34, 37, true, clickon_cds_host, render_rectangle},
    {244, 65, 37, 37, true, cb_backto_menup1, render_rectangle},
    // Icon
    {0, 0, 1, 1, false, nullptr, render_cds_icon},
};
static Control controls_cds_msgbox[GUI_ICCARD_CTRNUM]
{
    // Selection
    {48, 37, 92, 32, true, [](Window& wn, Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 1);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 1);}},
    {158, 37, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 2);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 2);}},
    {48, 79, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 3);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 3);}},
    {158, 79, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 4);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 4);}},
};
static ResourceDescriptor res_cds
{
    .path = "gui_main/main",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
static ResourceDescriptor res_cds_msgbox
{
    .path = "gui_main/iccard",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_cds(&res_cds, controls_card_selection, GUI_MAINPAGE_CTRNUM);
Window wn_cds_msg(&res_cds_msgbox, controls_cds_msgbox, GUI_ICCARD_CTRNUM);

void clickon_cds_ic(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds_msg);
    dis.refresh_count = 11;
}

extern void idcard_dir_update();

void clickon_cds_id(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    idcard_dir_update();
    dis.switchFocusLag(&wn_idcard_select);
    dis.refresh_count = 0;
}

void clickon_boxcard(Window& wn, Display& dis, ui_operation& opt, int args)
{
    if (opt != OP_ENTER) return;
    nfc::set_route(static_cast<nfc::NFC_Route>(args));
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void cb_backto_mainpage(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_cds_pwdgen(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::RegisterHIDDevice();
    dis.switchFocusLag(&wn_pwdgen);
    dis.refresh_count = 0;
}

void clickon_cds_pwdfill(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::RegisterHIDDevice();
    pwd_dir_update();
    dis.switchFocusLag(&wn_pwd_list);
    dis.refresh_count = 0;
}

extern void totp_dir_update();

void clickon_cds_totp(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    totp_dir_update();
    dis.switchFocusLag(&wn_totp_sel);
    dis.refresh_count = 0;
}

void clickon_cds_cards(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    wn_cards_update();
    dis.switchFocusLag(&wn_cards);
    dis.refresh_count = 0;
}

void clickon_cds_transparent(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    nfc::enable_transparent_mode();
    dis.switchFocusLag(&wn_transmode);
    dis.refresh_count = 0;
}

void clickon_cds_host(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::RegisterACMDevice();
    dis.switchFocusLag(&wn_manager_mode);
    dis.refresh_count = 0;
    core::StartManagerTask();
}

void render_cds_texture(Scheme& sche, Control& self, bool onSelect, int args)
{
    render_rectangle(sche, self, onSelect);
    uint16_t off = 0;
    if (args == -1)
        off = 192*nfc::get_route();
    else
        off = 192*args;
    sche.texture(&texture_icmode[off], self.x+self.w/2-32, self.y+self.h/2-12, 64, 24);
}

void render_cds_icon(Scheme& sche, Control& self, bool onSelect)
{
    float vot = bsp_adc::getVoltageBattery();
    bool charging = HAL_GPIO_ReadPin(FLG_CHG_GPIO_Port, FLG_CHG_Pin) == GPIO_PIN_RESET;
    char vot_display[6];
    sprintf(vot_display, "%.2fv", vot);
    sche.put_string(4, 110, ASCII_1608, vot_display);
    if (charging)
    {
        sche.texture(icon_battery_charging, 50, 110, 16, 16);
    }
}

