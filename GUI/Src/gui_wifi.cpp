#include "gui.h"

#define GUI_WIFI_SHARE_CTRNUM 1

using namespace gui;

extern Window wn_menu_page1;

void clickon_wifi_share_exit(Window& wn, Display& dis, ui_operation& opt);

Control controls_wifi_share[GUI_WIFI_SHARE_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_wifi_share_exit, nullptr}
};

ResourceDescriptor res_wifi_share
{
    .path = "wifi/demo.wifi",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_wifi_share(&res_wifi_share, controls_wifi_share, GUI_WIFI_SHARE_CTRNUM);

void clickon_wifi_share_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}
