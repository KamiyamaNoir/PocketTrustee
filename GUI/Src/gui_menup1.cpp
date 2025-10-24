#include "gui.h"
#include "bsp_nfc.h"
#include "cmsis_os.h"

#define GUI_MENUP1_CTRNUM 8

using namespace gui;

extern Window wn_cds;
extern Window wn_transmode;
extern Window wn_idcard;
extern Window wn_manager_mode;
extern Window wn_wifi_share;
extern Window wn_namecard;
extern Window wn_about;
extern Window wn_fingerprint;
extern Window wn_pin;
extern osThreadId manager_taskHandle;
extern void cb_backto_mainpage(Window& wn, Display& dis, ui_operation& opt);
extern void cb_backto_gui_pin(Window& wn, Display& dis, ui_operation& opt);
extern void cdc_acm_init();
extern void usbd_deinit();

void clickon_menup1_trans(Window& wn, Display& dis, ui_operation& opt);
void clickon_menup1_idcard(Window& wn, Display& dis, ui_operation& opt);
void clickon_menup1_fgmanage(Window& wn, Display& dis, ui_operation& opt);
void clickon_wifi_share(Window& wn, Display& dis, ui_operation& opt);
void clickon_namecard(Window& wn, Display& dis, ui_operation& opt);
void clickon_about(Window& wn, Display& dis, ui_operation& opt);
void clickon_menup1_managermode(Window& wn, Display& dis, ui_operation& opt);
void cb_backto_menup1(Window& wn, Display& dis, ui_operation& opt);

extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);

Control controls_menu_page1[GUI_MENUP1_CTRNUM]
{
    // Home page
    {2, 2, 70, 60, true, cb_backto_mainpage, render_rectangle},
    // Transparent mode
    {78, 2, 70, 60, true, clickon_menup1_trans, render_rectangle},
    // ID Card
    {150, 2, 70, 60, true, clickon_menup1_idcard, render_rectangle},
    // Fingerprint Manage
    {219, 2, 70, 60, true, clickon_menup1_fgmanage, render_rectangle},
    // WIFI
    {2, 62, 70, 60, true, clickon_wifi_share, render_rectangle},
    // Name card
    {78, 62, 70, 60, true, clickon_namecard, render_rectangle},
    // About
    {150, 62, 70, 60, true, clickon_about, render_rectangle},
    // Host
    {219, 62, 70, 60, true, clickon_menup1_managermode, render_rectangle},
};
ResourceDescriptor res_menu_page1
{
    .path = "gui_menu",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_menu_page1(&res_menu_page1, controls_menu_page1, GUI_MENUP1_CTRNUM);

void clickon_menup1_trans(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    nfc::enable_transparent_mode();
    dis.switchFocusLag(&wn_transmode);
    dis.refresh_count = 0;
}

void clickon_wifi_share(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_wifi_share);
    dis.refresh_count = 0;
}

void clickon_namecard(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_namecard);
    dis.refresh_count = 0;
}

void clickon_about(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_about);
    dis.refresh_count = 0;
}

extern void idcard_dir_update();

void clickon_menup1_idcard(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    idcard_dir_update();
    dis.switchFocusLag(&wn_idcard);
    dis.refresh_count = 0;
}

extern void register_pin_window(Window* wn);

void clickon_menup1_fgmanage(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    register_pin_window(&wn_fingerprint);
    dis.switchFocusLag(&wn_pin);
    dis.refresh_count = 0;
}

extern bool in_managermode;

void clickon_menup1_managermode(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    in_managermode = true;
    cdc_acm_init();
    dis.switchFocusLag(&wn_manager_mode);
    dis.refresh_count = 0;
    vTaskResume(manager_taskHandle);
}

void cb_backto_menup1(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}

