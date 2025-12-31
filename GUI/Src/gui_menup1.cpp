#include "gui_menup1.hpp"
#include "gui_mainpage.hpp"
#include "gui_about.hpp"

#define GUI_MENUP1_CTRNUM 2

using namespace gui;

static void clickon_about(Window& wn, Display& dis, ui_operation& opt);

static Control controls_menu_page1[GUI_MENUP1_CTRNUM]
{
    // Home page
    {62, 37, 66, 60, true, cb_backto_mainpage, render_rectangle},
    // About
    {166, 37, 66, 60, true, clickon_about, render_rectangle},
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

void clickon_about(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_about);
    dis.refresh_count = 0;
}


void cb_backto_menup1(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}

