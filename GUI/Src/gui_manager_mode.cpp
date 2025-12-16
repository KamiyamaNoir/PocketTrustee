#include "gui_manager_mode.hpp"
#include "gui_menup1.hpp"
#include <cstring>
#include "bsp_core.h"

#define GUI_MANAGE_MODE_CTRNUM 1
#define GUI_MANAGE_RESPOND_CTRNUM 1

using namespace gui;

extern volatile bool in_managermode;
extern char connect_user[12];
extern void usbd_deinit();

static void clickon_manager_exit(Window& wn, Display& dis, ui_operation& opt);

static void render_user_resp(Scheme& sche, Control& self, bool onSelect);

static Control controls_manager_mode[GUI_MANAGE_MODE_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_manager_exit, nullptr}
};
static Control controls_manager_respond[GUI_MANAGE_RESPOND_CTRNUM]
{
    {0, 64, 0, 0, false, nullptr, render_user_resp},
};

static ResourceDescriptor res_manager_mode
{
    .path = "gui_manage/host",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
static ResourceDescriptor res_manager_respond
{
    .path = "gui_manage/respond",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_manager_mode(&res_manager_mode, controls_manager_mode, GUI_MANAGE_MODE_CTRNUM);
Window wn_manager_respond(&res_manager_respond, controls_manager_respond, GUI_MANAGE_RESPOND_CTRNUM);

void clickon_manager_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::StopIdealTask();
    in_managermode = false;
    connect_user[0] = '\0';
    usbd_deinit();
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}


void render_user_resp(Scheme& sche, Control& self, bool onSelect)
{
    uint16_t off_x = GUI_WIDTH/2 + 22 - 4*strlen(connect_user);
    sche.put_string(off_x, self.y, ASCII_1608, connect_user);
}
