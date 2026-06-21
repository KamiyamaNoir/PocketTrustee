#include "gui_pwdfill.hpp"
#include "gui_resource.h"
#include "little_fs.hpp"
#include <cstdio>
// #include "fingerprint.h"
#include "gui_component_list.h"
#include "password.hpp"
#include "gui_mainpage.hpp"

#define GUI_PWDLIST_CTRNUM 7
#define GUI_PWDFILL_FAIL_CTRNUM 1

#define PWD_LIST_PAGE_SIZE 6

using namespace gui;

ComponentList<PWD_LIST_PAGE_SIZE, PasswordFile::PWD_NAME_MAX> pwd_list(PasswordFile::pwd_dir, PasswordFile::pwd_suffix);
static PasswordFile current_pwd;
static volatile bool in_select_mode = false;

static void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_pageup(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_pagedown(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_list(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_account(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_key(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_delete(Window& wn, Display& dis, ui_operation& opt);
static void clickon_pwd_fail(Window& wn, Display& dis, ui_operation& opt);

static void render_pwd_list(Scheme& sche, Control& self, bool onSelect);
static void render_pwd_select(Scheme& sche, Control& self, bool onSelect);


ResourceDescriptor res_pwd_list
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_pwdfill_fail
{
    .path = "gui_pwd/fail",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};
Control controls_pwd_list[GUI_PWDLIST_CTRNUM]
{
    // Exit
    {229, 1, 64, 24, true, clickon_pwd_exit, render_pwd_list},
    // Select box
    {1, 3, 203, 120, true, clickon_pwd_list, render_pwd_select},
    // Page Up and Down
    {217, 29, 18, 18, true, clickon_pwd_pageup, render_rectangle},
    {217, 66, 18, 18, true, clickon_pwd_pagedown, render_rectangle},
    // Account
    {246, 69, 47, 18, true, clickon_pwd_account, render_rectangle},
    // Key
    {246, 89, 47, 18, true, clickon_pwd_key, render_rectangle},
    // Delete
    {246, 109, 47, 18, true, clickon_pwd_delete, render_rectangle},
};
Control controls_pwd_fail[GUI_PWDFILL_FAIL_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_pwd_fail, render_rectangle}
};

Window wn_pwd_list(&res_pwd_list, controls_pwd_list, GUI_PWDLIST_CTRNUM);
Window wn_pwd_fail(&res_pwdfill_fail, controls_pwd_fail, GUI_PWDFILL_FAIL_CTRNUM);

void pwd_dir_update()
{
    pwd_list.update();
    current_pwd.load(pwd_list.seek(pwd_list.on_select()));
}

void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::DeinitUSB();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_pwd_pageup(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    if (pwd_list.move(-PWD_LIST_PAGE_SIZE))
    {
        pwd_list.update();
    }
}

void clickon_pwd_pagedown(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    if (pwd_list.move(PWD_LIST_PAGE_SIZE))
    {
        pwd_list.update();
    }
}

void clickon_pwd_list(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (pwd_list.item_count() == 0) return;
        in_select_mode = !in_select_mode;
        if (!in_select_mode)
        {
            // On exit select mode
            current_pwd.load(pwd_list.seek(pwd_list.on_select()));
        }
        return;
    }
    if (in_select_mode)
    {
        if (pwd_list.move(opt))
        {
            pwd_list.update();
        }
        opt = OP_NULL;
    }
}

void clickon_pwd_fail(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_pwd_list);
    dis.refresh_count = 0;
}

// static void result_callback(FingerprintResult result, FingerprintRequest* self)
// {
//     if (result == FG_PASS)
//     {
//         hid_keyboard_string(current_pwd.pwd);
//     }
// }

// static FingerprintRequest fg_req;

void clickon_pwd_account(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    core::USB_HID_Send(current_pwd.account);
}

void clickon_pwd_key(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    /* TODO: Fix the finger recognize module
    fg_req = {
        .dis = &dis,
        .wn_src = &wn,
        .result_callback = result_callback,
    };
    Fingerprint::Authen(&fg_req);
    */
    core::USB_HID_Send(current_pwd.pwd);
}

void clickon_pwd_delete(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    current_pwd.remove();
    pwd_dir_update();
}

void render_pwd_list(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear();
    // Display Icon
    sche.texture(icon_bk_menu, self.x, self.y, self.w, self.h)
        .texture(icon_down, 218, 67, 16, 16)
        .texture(icon_up, 218, 30, 16, 16)
        .texture(texture_pwdfill, 247, 70, 46, 56);
    // Display Page
    sche.put_string(211, 93, ASCII_1608, "Page");
    char text[10];
    sprintf(text, "%d/%d", pwd_list.page_index(), pwd_list.page_count());
    sche.put_string(211, 109, ASCII_1608, text);
    // Display items
    if (pwd_list.item_count() != 0)
    {
        for (uint8_t i = 0; i < PWD_LIST_PAGE_SIZE; i++)
        {
            sche.put_string(4, 5 + 20*(i % PWD_LIST_PAGE_SIZE), ASCII_1608, pwd_list.seek(i));
            if (i == pwd_list.on_select())
            {
                sche.rectangle(3, 5+20*(i % PWD_LIST_PAGE_SIZE), 199, 17, 1);
            }
        }
    }
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
}

void render_pwd_select(Scheme& sche, Control& self, bool onSelect)
{
    if (onSelect && !in_select_mode)
    {
        // Display Select Box
        sche.rectangle(self.x, self.y, self.w, self.h, 1);
    }
}

