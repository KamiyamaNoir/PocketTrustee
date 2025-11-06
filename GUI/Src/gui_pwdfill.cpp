#include "gui.h"
#include "gui_resource.h"
#include "lfs_base.h"
#include <cstdio>
#include "fingerprint.h"
#include "password.h"

#define GUI_PWDLIST_CTRNUM 7
#define GUI_PWDFILL_FAIL_CTRNUM 1

#define PWD_LIST_PAGE_SIZE 6
#define PWD_LIST_PAGE_MAX  5

using namespace gui;

extern osThreadId defaultTaskHandle;

struct PWD_DataStruct
{
    char account[32];
    char password[64];
    char path[42];
};

static uint16_t pwd_item_count = 0;
static int on_select_pwd_index = -1;
static char pwd_file_name_map[PWD_LIST_PAGE_SIZE * PWD_LIST_PAGE_MAX][PasswordFile::PWD_NAME_MAX];
static PasswordFile current_pwd;
static volatile bool in_select_mode = false;

extern Window wn_cds;
extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);
extern void hid_keyboard_string(const char* str);
extern void usbd_deinit();

void pwd_dir_update();

void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_pageup(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_pagedown(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_list(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_account(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_key(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_delete(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_fail(Window& wn, Display& dis, ui_operation& opt);

void render_pwd_list(Scheme& sche, Control& self, bool onSelect);
void render_pwd_select(Scheme& sche, Control& self, bool onSelect);


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
    {3, 4, 203, 120, true, clickon_pwd_list, render_pwd_select},
    // Page Up and Down
    {217, 29, 18, 18, true, clickon_pwd_pageup, render_rectangle},
    {217, 66, 18, 18, true, clickon_pwd_pagedown, render_rectangle},
    // Key
    {246, 69, 47, 18, true, clickon_pwd_key, render_rectangle},
    // Account
    {246, 87, 47, 18, true, clickon_pwd_account, render_rectangle},
    // Delete
    {246, 107, 47, 18, true, clickon_pwd_delete, render_rectangle},
};
Control controls_pwd_fail[GUI_PWDFILL_FAIL_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_pwd_fail, render_rectangle}
};

Window wn_pwd_list(&res_pwd_list, controls_pwd_list, GUI_PWDLIST_CTRNUM);
Window wn_pwd_fail(&res_pwdfill_fail, controls_pwd_fail, GUI_PWDFILL_FAIL_CTRNUM);

void pwd_dir_update()
{
    auto fs_pwd_dir = LittleFS::fs_dir_handler(PasswordFile::pwd_dir);
    int dir_count = fs_pwd_dir.count();
    if (dir_count < 0) return;
    pwd_item_count = 0;
    fs_pwd_dir.rewind();
    for (int i = 0; i < dir_count; i++)
    {
        //NOLINTNEXTLINE
        lfs_info info;
        fs_pwd_dir.next(&info);
        if (info.type == LFS_TYPE_DIR) continue;
        uint32_t len = strlen(info.name);
        if (len < sizeof(PasswordFile::pwd_suffix)) continue;
        if (memcmp(info.name + len - sizeof(PasswordFile::pwd_suffix) + 1, PasswordFile::pwd_suffix, sizeof(PasswordFile::pwd_suffix) - 1) != 0)
            continue;
        char name[PasswordFile::PWD_NAME_MAX];
        strcpy(name, info.name);
        name[len - sizeof(PasswordFile::pwd_suffix) + 1] = '\0';
        strcpy(pwd_file_name_map[pwd_item_count], name);
        pwd_item_count++;
    }
    if (pwd_item_count != 0)
    {
        on_select_pwd_index = 0;
        current_pwd.load(pwd_file_name_map[0]);
    }
    else
        on_select_pwd_index = -1;
}

void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    usbd_deinit();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_pwd_pageup(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    on_select_pwd_index -= PWD_LIST_PAGE_SIZE;
    if (on_select_pwd_index < 0)
    {
        on_select_pwd_index = 0;
    }
}

void clickon_pwd_pagedown(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    on_select_pwd_index += PWD_LIST_PAGE_SIZE;
    if (on_select_pwd_index > pwd_item_count-1 || on_select_pwd_index < 0)
    {
        on_select_pwd_index = pwd_item_count-1;
    }
}

void clickon_pwd_list(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (pwd_item_count == 0) return;
        in_select_mode = !in_select_mode;
        if (!in_select_mode)
        {
            // On exit select mode
            current_pwd.load(pwd_file_name_map[on_select_pwd_index]);
        }
        return;
    }
    if (in_select_mode)
    {
        on_select_pwd_index += opt;
        if (on_select_pwd_index < 0)
            on_select_pwd_index = 0;
        if (on_select_pwd_index > pwd_item_count-1)
            on_select_pwd_index = pwd_item_count-1;
        opt = OP_NULL;
    }
}

void clickon_pwd_fail(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_pwd_list);
    dis.refresh_count = 0;
}

static void result_callback(FingerprintResult result, FingerprintRequest* self)
{
    if (result == FG_PASS)
    {
        hid_keyboard_string(current_pwd.pwd);
    }
}

static FingerprintRequest fg_req;

void clickon_pwd_account(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    hid_keyboard_string(current_pwd.account);
}

void clickon_pwd_key(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    fg_req = {
        .dis = &dis,
        .wn_src = &wn,
        .result_callback = result_callback,
    };
    Fingerprint::Authen(&fg_req);
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
    uint8_t page = (pwd_item_count-1) / PWD_LIST_PAGE_SIZE + 1;
    uint8_t index = on_select_pwd_index / PWD_LIST_PAGE_SIZE + 1;
    char text[10];
    sprintf(text, "%d/%d", index, page);
    sche.put_string(211, 109, ASCII_1608, text);
    // Display items
    if (pwd_item_count != 0 && on_select_pwd_index >= 0)
    {
        uint8_t start = (on_select_pwd_index / PWD_LIST_PAGE_SIZE) * PWD_LIST_PAGE_SIZE;
        uint8_t end = start + PWD_LIST_PAGE_SIZE > pwd_item_count-1 ? pwd_item_count : (start + PWD_LIST_PAGE_SIZE);
        for (uint8_t i = start; i < end; i++)
        {
            sche.put_string(4, 5 + 20*(i % PWD_LIST_PAGE_SIZE), ASCII_1608, pwd_file_name_map[i]);
            if (i == on_select_pwd_index)
            {
                sche.rectangle(3, 5+20*(i % PWD_LIST_PAGE_SIZE), 201, 17, 1);
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

