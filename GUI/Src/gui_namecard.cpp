#include "gui.h"

#define GUI_NAMECARD_CTRNUM 1

using namespace gui;

extern Window wn_menu_page1;

void clickon_namecard_exit(Window& wn, Display& dis, ui_operation& opt);

Control controls_namecard[GUI_NAMECARD_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_namecard_exit, nullptr}
};

ResourceDescriptor res_namecard
{
    .path = "namecards/demo.ncard",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_namecard(&res_namecard, controls_namecard, GUI_NAMECARD_CTRNUM);

void clickon_namecard_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}
