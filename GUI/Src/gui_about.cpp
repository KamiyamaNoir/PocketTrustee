#include "gui_about.hpp"
#include "gui_menup1.hpp"

#define GUI_ABOUT_CTRNUM 1

using namespace gui;

static void clickon_about_exit(Window& wn, Display& dis, ui_operation& opt);

static Control controls_about[GUI_ABOUT_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_about_exit, nullptr}
};

static ResourceDescriptor res_about
{
    .path = "device.info",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_about(&res_about, controls_about, GUI_ABOUT_CTRNUM);

void clickon_about_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}
