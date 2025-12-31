#include "gui_transmode.hpp"
#include "gui_mainpage.hpp"
#include "bsp_nfc.h"

#define GUI_TRANSMODE_CTRNUM 1

using namespace gui;

static void clickon_trans_exit(Window& wn, Display& dis, ui_operation& opt);

static Control controls_transmode[GUI_TRANSMODE_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_trans_exit, nullptr}
};

static ResourceDescriptor res_trsmode
{
    .path = "gui_trans/trans",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_transmode(&res_trsmode, controls_transmode, GUI_TRANSMODE_CTRNUM);

void clickon_trans_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    nfc::disable_transparent_mode();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}
