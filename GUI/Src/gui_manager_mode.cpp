#include "gui.h"
#include <cstring>
#include "cmsis_os.h"
#include "task.h"

#define GUI_MANAGE_MODE_CTRNUM 1
#define GUI_MANAGE_MSGBOX_CTRNUM 3
#define GUI_MANAGE_RESPOND_CTRNUM 1

using namespace gui;

extern Window wn_menu_page1;
extern osThreadId manager_taskHandle;
extern volatile bool in_managermode;
extern char connect_user[12];
extern void usbd_deinit();

volatile char gui_manager_msgbox_clicked = 0;

void clickon_manager_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_manager_msgbox(Window& wn, Display& dis, ui_operation& opt, int args);

void render_user(Scheme& sche, Control& self, bool onSelect);
void render_user_resp(Scheme& sche, Control& self, bool onSelect);

extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);

Control controls_manager_mode[GUI_MANAGE_MODE_CTRNUM]
{
    {0, 0, 1, 1, false, clickon_manager_exit, nullptr}
};
Control controls_manager_msgbox[GUI_MANAGE_MSGBOX_CTRNUM]
{
    {54, 87, 67, 27, true, [](Window& wn, Display& dis, ui_operation& opt)->void{clickon_manager_msgbox(wn, dis, opt, 1);}, render_rectangle},
    {175, 87, 67, 27, true, [](Window& wn, Display& dis, ui_operation& opt)->void{clickon_manager_msgbox(wn, dis, opt, 2);}, render_rectangle},
    {0, 50, 0, 0, false, nullptr, render_user},
};
Control controls_manager_respond[GUI_MANAGE_RESPOND_CTRNUM]
{
    {0, 64, 0, 0, false, nullptr, render_user_resp},
};

ResourceDescriptor res_manager_mode
{
    .path = "gui_manage/host",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_manager_msgbox
{
    .path = "gui_manage/trust",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};
ResourceDescriptor res_manager_respond
{
    .path = "gui_manage/respond",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_manager_mode(&res_manager_mode, controls_manager_mode, GUI_MANAGE_MODE_CTRNUM);
Window wn_manager_msgbox(&res_manager_msgbox, controls_manager_msgbox, GUI_MANAGE_MSGBOX_CTRNUM);
Window wn_manager_respond(&res_manager_respond, controls_manager_respond, GUI_MANAGE_RESPOND_CTRNUM);

void clickon_manager_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    vTaskSuspend(manager_taskHandle);
    in_managermode = false;
    connect_user[0] = '\0';
    usbd_deinit();
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}

void clickon_manager_msgbox(Window& wn, Display& dis, ui_operation& opt, int args)
{
    if (opt != OP_ENTER) return;
    gui_manager_msgbox_clicked = args;
    dis.switchFocusLag(&wn_manager_respond);
    dis.refresh_count = 0;
}

void render_user(Scheme& sche, Control& self, bool onSelect)
{
    uint16_t off_x = GUI_WIDTH/2 - 4*strlen(connect_user);
    sche.put_string(off_x, self.y, ASCII_1608, connect_user);
}

void render_user_resp(Scheme& sche, Control& self, bool onSelect)
{
    uint16_t off_x = GUI_WIDTH/2 + 22 - 4*strlen(connect_user);
    sche.put_string(off_x, self.y, ASCII_1608, connect_user);
}
