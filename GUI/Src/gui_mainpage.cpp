#include "bsp_adc.h"
#include "gui.h"
#include "bsp_nfc.h"
#include "gui_resource.h"
#include "lfs_base.h"

#define GUI_MAINPAGE_CTRNUM 7
#define GUI_ICCARD_CTRNUM 4

using namespace gui;

extern nfc::NFC_Route nfc_current_route;
extern void cb_backto_menup1(Window& wn, Display& dis, ui_operation& opt);
extern Window wn_pwdgen;
extern Window wn_pwd_list;
extern Window wn_idcard_select;
extern Window wn_totp_sel;

void clickon_cds_ic(Window& wn, Display& dis, ui_operation& opt);
void clickon_cds_id(Window& wn, Display& dis, ui_operation& opt);
void clickon_cds_pwdgen(Window& wn, Display& dis, ui_operation& opt);
void clickon_cds_pwdfill(Window& wn, Display& dis, ui_operation& opt);
void clickon_cds_totp(Window& wn, Display& dis, ui_operation& opt);
void clickon_boxcard(Window& wn, Display& dis, ui_operation& opt, int args);
void cb_backto_mainpage(Window& wn, Display& dis, ui_operation& opt);

extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);
void render_cds_texture(Scheme& sche, Control& self, bool onSelect, int args);
void render_cds_idcard(Scheme& sche, Control& self, bool onSelect);
void render_cds_icon(Scheme& sche, Control& self, bool onSelect);

Control controls_card_selection[GUI_MAINPAGE_CTRNUM]
{
    // IC Card and ID Card
    {5, 25, 110,29, true, clickon_cds_ic, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, -1);}},
    {5, 79, 110, 29, true, clickon_cds_id, render_cds_idcard},
    // Quick Tools
    {133, 24, 100,23, true, clickon_cds_pwdgen, render_rectangle},
    {133, 44, 100, 23, true, clickon_cds_pwdfill, render_rectangle},
    {133, 62, 100,23, true, clickon_cds_totp, render_rectangle},
    {133, 82, 100, 22, true, cb_backto_menup1, render_rectangle},
    // Icon
    {0, 0, 1, 1, false, nullptr, render_cds_icon},
};
Control controls_cds_msgbox[GUI_ICCARD_CTRNUM]
{
    // Selection
    {48, 37, 92, 32, true, [](Window& wn, Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 1);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 1);}},
    {158, 37, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 2);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 2);}},
    {48, 79, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 3);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 3);}},
    {158, 79, 92, 32, true, [](Window& wn,  Display& dis, ui_operation& opt) -> void {clickon_boxcard(wn, dis, opt, 4);}, [](Scheme& a, Control& b, bool c) -> void {render_cds_texture(a, b, c, 4);}},
};
ResourceDescriptor res_cds
{
    .path = "gui_main/main",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_cds_msgbox
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
    nfc_current_route = static_cast<nfc::NFC_Route>(args);
    nfc::set_route();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void cb_backto_mainpage(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

extern void hid_keyboard_init();

void clickon_cds_pwdgen(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    hid_keyboard_init();
    dis.switchFocusLag(&wn_pwdgen);
    dis.refresh_count = 0;
}

extern void pwd_dir_update();

void clickon_cds_pwdfill(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    hid_keyboard_init();
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

void render_cds_texture(Scheme& sche, Control& self, bool onSelect, int args)
{
    render_rectangle(sche, self, onSelect);
    LittleFS::fs_file_handler texture("texture/iccard");
    if (args == -1)
        texture.seek(192*nfc_current_route);
    else
        texture.seek(192*args);
    uint8_t cache[192];
    texture.read(cache, 192);
    sche.texture(cache, self.x+self.w/2-32, self.y+self.h/2-12, 64, 24);
}

void render_cds_idcard(Scheme& sche, Control& self, bool onSelect)
{
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
    sche.put_string(50, self.y+self.h/2-7, ASCII_1608, "OFF");
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

