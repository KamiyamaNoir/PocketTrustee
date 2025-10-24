#include "gui.h"
#include "crypto_base.h"
#include <cstring>
#include <cstdio>
#include "bsp_rtc.h"
#include "zcbor_encode.h"
#include "lfs_base.h"
#include "aes.h"

#define GUI_PWDGEN_CTRNUM 9
#define GUI_PWDGEN_SAVED_CTRNUM 1

using namespace gui;

extern Window wn_cds;
extern void hid_keyboard_string(const char* str);
extern void usbd_deinit();
extern void pwd_dir_update();

void clickon_pwdgen_keylength(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwdgen_gen(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwdgen_send(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwdgen_config(Window& wn, Display& dis, ui_operation& opt, int args);
void clickon_pwdgen_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwdgen_save(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwdgen_saved_ok(Window& wn, Display& dis, ui_operation& opt);

extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);
void render_pwdgen_keylength(Scheme& sche, Control& self, bool onSelect);
void render_pwdgen_saved(Scheme& sche, Control& self, bool onSelect);

Control controls_pwdgen[GUI_PWDGEN_CTRNUM]
{
    // Back to MENU
    {228, 2, 67, 21, true, clickon_pwdgen_exit, render_rectangle},
    // ABC
    {49, 59, 35, 20, true, [](Window& wn, Display& dis, ui_operation& opt) -> void { clickon_pwdgen_config(wn, dis, opt, 1); }, render_rectangle},
    // abc
    {103, 59, 35, 20, true, [](Window& wn, Display& dis, ui_operation& opt) -> void { clickon_pwdgen_config(wn, dis, opt, 2); }, render_rectangle},
    // 012
    {157, 59, 35, 20, true, [](Window& wn, Display& dis, ui_operation& opt) -> void { clickon_pwdgen_config(wn, dis, opt, 3); }, render_rectangle},
    // !@#
    {212, 59, 35, 20, true, [](Window& wn, Display& dis, ui_operation& opt) -> void { clickon_pwdgen_config(wn, dis, opt, 4); }, render_rectangle},
    // Length
    {73, 102, 34, 22, true, clickon_pwdgen_keylength, render_pwdgen_keylength},
    // Generate, Send and Save
    {104, 99, 62, 26, true, clickon_pwdgen_gen, render_rectangle},
    {167, 99, 62, 26, true, clickon_pwdgen_send, render_rectangle},
    {230, 99, 62, 26, true, clickon_pwdgen_save, render_rectangle},
};
Control controls_pwdgen_saved[GUI_PWDGEN_SAVED_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_pwdgen_saved_ok, render_pwdgen_saved}
};
ResourceDescriptor res_pwdgen
{
    .path = "gui_pwd/pwdgen",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_pwdgen_saved
{
    .path = "gui_pwd/saved",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_pwdgen(&res_pwdgen, controls_pwdgen, GUI_PWDGEN_CTRNUM);
Window wn_pwdgen_saved(&res_pwdgen_saved, controls_pwdgen_saved, GUI_PWDGEN_SAVED_CTRNUM);

bool pwdgen_clicked_length = false;
crypto::KeygenConfiguration pwdgen_config = {
    .containNumber = true,
    .containUppercase = true,
    .containLowercase = true,
    .containSChar = false,
    .length = 12,
};
char pwdgen_pwd[25] = "";

void render_pwdgen_keylength(Scheme& sche, Control& self, bool onSelect)
{
    render_rectangle(sche, self, onSelect);
    char num[4];
    sprintf(num, "%d", pwdgen_config.length);
    sche.put_string(self.x + 2, self.y + 3, ASCII_1608, num);
    if (pwdgen_clicked_length)
    {
        sche.invert(self.x, self.y, self.w, self.h);
    }
    // display pwd
    uint16_t off_x = GUI_WIDTH/2 - 4*strlen(pwdgen_pwd);
    sche.put_string(off_x, 37, ASCII_1608, pwdgen_pwd);
    // display config
    if (pwdgen_config.containNumber)
        sche.invert(157, 59, 35, 20);
    if (pwdgen_config.containUppercase)
        sche.invert(49, 59, 35, 20);
    if (pwdgen_config.containLowercase)
        sche.invert(103, 59, 35, 20);
    if (pwdgen_config.containSChar)
        sche.invert(212, 59, 35, 20);
}

void render_pwdgen_saved(Scheme& sche, Control& self, bool onSelect)
{
    uint16_t off_x = GUI_WIDTH/2 - 4*strlen(pwdgen_pwd);
    sche.put_string(off_x, 54, ASCII_1608, pwdgen_pwd);
    sche.rectangle(self.x, self.y, self.w, self.h, 2);
}

void clickon_pwdgen_keylength(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        pwdgen_clicked_length = !pwdgen_clicked_length;
        return;
    }
    if (pwdgen_clicked_length)
    {
        pwdgen_config.length -= opt;
        if (pwdgen_config.length > 24) pwdgen_config.length = 24;
        if (pwdgen_config.length < 4) pwdgen_config.length = 4;
        opt = OP_NULL;
    }
}

void clickon_pwdgen_config(Window& wn, Display& dis, ui_operation& opt, int args)
{
    if (opt != OP_ENTER) return;
    switch (args)
    {
    case 1:
        pwdgen_config.containUppercase = !pwdgen_config.containUppercase;
        break;
    case 2:
        pwdgen_config.containLowercase = !pwdgen_config.containLowercase;
        break;
    case 3:
        pwdgen_config.containNumber = !pwdgen_config.containNumber;
        break;
    case 4:
        pwdgen_config.containSChar = !pwdgen_config.containSChar;
    default:
        break;
    }
}

void clickon_pwdgen_gen(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    crypto::skeygen(&pwdgen_config, pwdgen_pwd);
}

void clickon_pwdgen_send(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    hid_keyboard_string(pwdgen_pwd);
}

void clickon_pwdgen_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    pwdgen_pwd[0] = '\0';
    usbd_deinit();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_pwdgen_saved_ok(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    pwd_dir_update();
    pwdgen_pwd[0] = '\0';
    dis.switchFocusLag(&wn_pwdgen);
    dis.refresh_count = 0;
}

void clickon_pwdgen_save(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    if (pwdgen_pwd[0] == '\0') return;
    rtc::TimeDate td;
    rtc::getTimedate(&td);
    char name[26];
    uint8_t payload[160];
    uint8_t encrypto_payload[sizeof(payload)];
    sprintf(name, "%d-%d-%d %d-%d-%d", td.year, td.month, td.day, td.hour, td.minute, td.second);
    ZCBOR_STATE_E(state, 2, payload, sizeof(payload), 1);
    zcbor_list_start_encode(state, 2);
    zcbor_tstr_put_lit(state, "Unknown");
    zcbor_tstr_put_lit(state, pwdgen_pwd);
    zcbor_list_end_encode(state, 2);
    memcpy(pwdgen_pwd, name, strlen(name) + 1);
    strcat(name, ".pwd");
    char path[42] = "passwords/";
    strcat(path, name);
    auto fs = LittleFS::fs_file_handler(path);
    HAL_CRYP_AESECB_Encrypt(&hcryp, payload, state->payload - payload, encrypto_payload, 1000);
    fs.write(encrypto_payload, sizeof(encrypto_payload));
    dis.switchFocusLag(&wn_pwdgen_saved);
    dis.refresh_count = 11;
}
