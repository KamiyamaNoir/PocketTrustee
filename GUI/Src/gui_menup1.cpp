#include "gui_menup1.hpp"
#include "gui_mainpage.hpp"
#include "gui_transmode.hpp"
#include "gui_about.hpp"
#include "gui_idcard.hpp"
#include "gui_manager_mode.hpp"
#include "gui_fingerprint.hpp"
#include "gui_lock.hpp"
#include "bsp_nfc.h"
#include "gui_cards.hpp"

#define GUI_MENUP1_CTRNUM 8

using namespace gui;

static void clickon_menup1_trans(Window& wn, Display& dis, ui_operation& opt);
static void clickon_menup1_idcard(Window& wn, Display& dis, ui_operation& opt);
static void clickon_menup1_fgmanage(Window& wn, Display& dis, ui_operation& opt);
static void clickon_wifi_share(Window& wn, Display& dis, ui_operation& opt);
static void clickon_namecard(Window& wn, Display& dis, ui_operation& opt);
static void clickon_about(Window& wn, Display& dis, ui_operation& opt);
static void clickon_menup1_managermode(Window& wn, Display& dis, ui_operation& opt);

static Control controls_menu_page1[GUI_MENUP1_CTRNUM]
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
static ResourceDescriptor res_menu_page1
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
    wn_cards_update();
    dis.switchFocusLag(&wn_cards);
    dis.refresh_count = 0;
}

void clickon_namecard(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cards);
    dis.refresh_count = 0;
}

void clickon_about(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_about);
    dis.refresh_count = 0;
}

void clickon_menup1_idcard(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
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

void clickon_menup1_managermode(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::RegisterACMDevice();
    dis.switchFocusLag(&wn_manager_mode);
    dis.refresh_count = 0;
    core::StartIdealTask();
}

void cb_backto_menup1(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}

