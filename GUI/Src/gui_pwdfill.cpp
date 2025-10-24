#include "gui.h"
#include "gui_resource.h"
#include "lfs.h"
#include "lfs_base.h"
#include <cstdio>
#include "zcbor_decode.h"
#include "aes.h"
#include "fingerprint.h"

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

constexpr char pwd_dir_path[] = "passwords";
static uint16_t pwd_item_count = 0;
static volatile uint16_t item_ptr = 0;
static uint32_t pwd_dir_index_map[PWD_LIST_PAGE_MAX * PWD_LIST_PAGE_SIZE];
static volatile bool in_select_mode = false;
static PWD_DataStruct currentPWD {};

extern Window wn_cds;
extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);
extern void hid_keyboard_string(const char* str);
extern void usbd_deinit();

void pwd_dir_update();

void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_up(Window& wn, Display& dis, ui_operation& opt);
void clickon_pwd_down(Window& wn, Display& dis, ui_operation& opt);
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
    // Up and Down
    {217, 29, 18, 18, true, clickon_pwd_up, render_rectangle},
    {217, 66, 18, 18, true, clickon_pwd_down, render_rectangle},
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

static void load_current_pwd()
{
    if (item_ptr >= pwd_item_count)
    {
        currentPWD.account[0] = '\0';
        currentPWD.password[0] = '\0';
        return;
    }
    uint8_t raw_data[160];
    uint8_t payload[sizeof(raw_data)];
    auto dir = LittleFS::fs_dir_handler(pwd_dir_path);
    dir.seek(pwd_dir_index_map[item_ptr]);
    lfs_info info;
    dir.next(&info);
    char path[42] = "passwords/";
    strcat(path, info.name);
    zcbor_string str;
    auto fs = LittleFS::fs_file_handler(path);
    uint16_t nread = fs.read(raw_data, sizeof(raw_data));
    HAL_CRYP_AESECB_Decrypt(&hcryp, raw_data, nread, payload, 1000);
    ZCBOR_STATE_D(state, 2, payload, sizeof(payload), 1, 0);
    zcbor_list_start_decode(state);

    zcbor_tstr_decode(state, &str);
    memcpy(currentPWD.account, str.value, str.len);
    currentPWD.account[str.len] = '\0';

    zcbor_tstr_decode(state, &str);
    memcpy(currentPWD.password, str.value, str.len);
    currentPWD.password[str.len] = '\0';

    memcpy(currentPWD.path, path, strlen(path) + 1);
}

/**
 * Load and update password data from flash
 */
void pwd_dir_update()
{
    auto fs_pwd_dir = LittleFS::fs_dir_handler(pwd_dir_path);
    int dir_count = fs_pwd_dir.count();
    if (dir_count < 0) return;
    pwd_item_count = 0;
    fs_pwd_dir.rewind();
    for (int i = 0; i < dir_count; i++)
    {
        lfs_info info {};
        uint32_t pos = fs_pwd_dir.tell();
        fs_pwd_dir.next(&info);
        if (info.type == LFS_TYPE_DIR) continue;
        uint32_t len = strlen(info.name);
        if (len < 4) continue;
        if (memcmp(info.name + len - 4, ".pwd", 4) != 0)
            continue;
        pwd_dir_index_map[pwd_item_count] = pos;
        pwd_item_count++;
    }
    item_ptr = 0;
    load_current_pwd();
}

void clickon_pwd_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    usbd_deinit();
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_pwd_up(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    if (item_ptr < PWD_LIST_PAGE_SIZE) return;
    item_ptr = ((item_ptr - PWD_LIST_PAGE_SIZE) / PWD_LIST_PAGE_SIZE) * PWD_LIST_PAGE_SIZE;
}

void clickon_pwd_down(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    uint16_t start = (item_ptr / PWD_LIST_PAGE_SIZE) * PWD_LIST_PAGE_SIZE;
    if (pwd_item_count - start <= PWD_LIST_PAGE_SIZE) return;
    item_ptr = ((item_ptr + PWD_LIST_PAGE_SIZE) / PWD_LIST_PAGE_SIZE) * PWD_LIST_PAGE_SIZE;
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
            load_current_pwd();
        }
        return;
    }
    if (in_select_mode)
    {
        uint16_t start = (item_ptr / PWD_LIST_PAGE_SIZE) * PWD_LIST_PAGE_SIZE;
        uint16_t end = pwd_item_count - start > PWD_LIST_PAGE_SIZE ? start + PWD_LIST_PAGE_SIZE : pwd_item_count - 1;
        if (item_ptr == 0 && opt == OP_UP)
        {
            item_ptr = end;
        }
        else
        {
            item_ptr += opt;
            if (item_ptr > end)
                item_ptr = start;
            if (item_ptr < start)
                item_ptr = end;
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

static void result_callback(FingerprintResult result, FingerprintRequest* self)
{
    if (result == FG_PASS)
    {
        hid_keyboard_string(currentPWD.password);
    }
}

static FingerprintRequest fg_req;

void clickon_pwd_account(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    hid_keyboard_string(currentPWD.account);
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
    LittleFS::fs_remove(currentPWD.path);
    pwd_dir_update();
}

void render_pwd_list(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear();
    auto fs_pwd_dir = LittleFS::fs_dir_handler(pwd_dir_path);
    // Display Icon
    sche.texture(icon_bk_menu, self.x, self.y, self.w, self.h)
        .texture(icon_down, 218, 67, 16, 16)
        .texture(icon_up, 218, 30, 16, 16)
        .texture(texture_pwdfill, 247, 70, 46, 56);
    // Display Page
    sche.put_string(211, 93, ASCII_1608, "Page");
    uint8_t page = (pwd_item_count-1) / PWD_LIST_PAGE_SIZE + 1;
    uint8_t index = item_ptr / PWD_LIST_PAGE_SIZE + 1;
    char text[10];
    sprintf(text, "%d/%d", index, page);
    sche.put_string(211, 109, ASCII_1608, text);
    // Display items
    for (uint8_t i = 0; i < PWD_LIST_PAGE_SIZE; i++)
    {
        lfs_info info;
        uint32_t pos = i + (index-1)*PWD_LIST_PAGE_SIZE;
        if (pos >= pwd_item_count) break;
        fs_pwd_dir.seek(pwd_dir_index_map[pos]);
        fs_pwd_dir.next(&info);
        char name[64];
        uint8_t name_size = strlen(info.name) - 4;
        memcpy(name, info.name, name_size);
        name[name_size] = '\0';
        sche.put_string(4, 5 + 20*i, ASCII_1608, name);
        if (pos == item_ptr)
        {
            sche.rectangle(3, 5+20*i, 201, 17, 1);
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

