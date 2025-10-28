#include "gui.h"
#include "gui_resource.h"
#include "otp.h"
#include <cstring>
#include "lfs_base.h"

#define GUI_TOTP_SEL_CTRNUM 2
#define GUI_TOTP_CTRNUM 1

#define TOTP_SEL_PAGE_SIZE 5
#define TOTP_SEL_PAGE_MAX 1

using namespace gui;

extern Window wn_cds;

void clickon_totp_sel_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_totp_sel(Window& wn, Display& dis, ui_operation& opt);
void clickon_totp_exit(Window& wn, Display& dis, ui_operation& opt);

void render_totp_sel(Scheme& sche, Control& self, bool onSelect);
void render_totp(Scheme& sche, Control& self, bool onSelect);

Control controls_totp_sel[GUI_TOTP_SEL_CTRNUM]
{
    {229, 1, 64, 24, true, clickon_totp_sel_exit, render_totp_sel},
    {0, 0, 1, 1, true, clickon_totp_sel, nullptr}
};
Control controls_totp[GUI_TOTP_CTRNUM]
{
    {229, 1, 64, 24, true, clickon_totp_exit, render_totp}
};
ResourceDescriptor res_totp_sel
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_totp
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_totp_sel(&res_totp_sel, controls_totp_sel, GUI_TOTP_SEL_CTRNUM);
Window wn_totp(&res_totp, controls_totp, GUI_TOTP_CTRNUM);

constexpr char totp_basepath[] = "otp/";
static int onselect_file_index = -1;
static uint8_t totp_file_count = 0;
static uint32_t totp_file_index_map[TOTP_SEL_PAGE_SIZE * TOTP_SEL_PAGE_MAX];
static char totp_file_name_map[TOTP_SEL_PAGE_SIZE][OTP_TOTP::TOTP_NAME_MAX];


void totp_name_map_update()
{
    uint8_t page_start;
    if (totp_file_count == 0)
        return;
    if (onselect_file_index < 0)
        page_start = 0;
    else
        page_start = (onselect_file_index / TOTP_SEL_PAGE_SIZE) * TOTP_SEL_PAGE_SIZE;
    uint8_t page_end = page_start + TOTP_SEL_PAGE_SIZE;
    if (page_end > totp_file_count - 1)
    {
        page_end = totp_file_count - 1;
    }
    auto dir = LittleFS::fs_dir_handler(totp_basepath);
    for (uint8_t i = 0; i < TOTP_SEL_PAGE_SIZE; i++)
    {
        if (page_start + i > page_end)
        {
            totp_file_name_map[i][0] = '\0';
            continue;
        }

        //NOLINTNEXTLINE
        lfs_info info;
        dir.seek(totp_file_index_map[page_start]);
        dir.next(&info);

        uint8_t len = strlen(info.name) - 5;
        memcpy(totp_file_name_map[i], info.name, len);
        totp_file_name_map[i][len] = '\0';

        page_start++;
    }
}

void totp_dir_update()
{
    totp_file_count = 0;
    onselect_file_index = -1;
    auto dir = LittleFS::fs_dir_handler(totp_basepath);
    int dir_count = dir.count();
    if (dir_count < 0) return;
    dir.rewind();

    for (int i = 0; i < dir_count; i++)
    {
        //NOLINTNEXTLINE
        lfs_info info;
        uint32_t pos = dir.tell();
        dir.next(&info);
        if (info.type == LFS_TYPE_DIR) continue;
        uint32_t len = strlen(info.name);
        if (len < 5) continue;
        if (memcmp(info.name + len - 5, ".totp", 5) != 0)
            continue;
        totp_file_index_map[totp_file_count] = pos;
        totp_file_count++;
    }

    totp_name_map_update();
}

void clickon_totp_sel_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_totp_sel(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        dis.switchFocusLag(&wn_totp);
        dis.refresh_count = 0;
        return;
    }
    if (totp_file_count == 0) return;
    if (opt == OP_UP && onselect_file_index == 0)
    {
        onselect_file_index = -1;
        return;
    }
    if (opt == OP_DOWN && onselect_file_index == totp_file_count-1)
    {
        onselect_file_index = -1;
        return;
    }
    onselect_file_index += opt;
    opt = OP_NULL;
}

void clickon_totp_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_totp_sel);
    dis.refresh_count = 0;
}

void render_totp_sel(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear();
    // Display icon
    sche.texture(icon_bk_menu, self.x, self.y, self.w, self.h);
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
    else if (onselect_file_index < 0)
    {
        onselect_file_index = 0;
    }
    // Display item
    for (uint8_t i = 0; i < TOTP_SEL_PAGE_SIZE; i++)
    {
        const char* name = totp_file_name_map[i];
        if (name[0] == '\0')
            break;
        sche.put_string(4, 5+20*i, ASCII_1608, name);
        if (i == onselect_file_index)
        {
            sche.rectangle(3, 5+20*i, 201, 17, 1);
        }
    }
}

void render_totp(Scheme& sche, Control& self, bool onSelect)
{
    sche.clear()
        .texture(icon_bk_menu, self.x, self.y, self.w, self.h)
        .put_string(2, 2, ASCII_3216, "TOTP");
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }

    auto totp = OTP_TOTP(totp_file_name_map[onselect_file_index % TOTP_SEL_PAGE_SIZE]);
    uint8_t off =  GUI_WIDTH/2 - 4*strlen(totp.getName());
    sche.rectangle(68, 93, 160, 2)
        .put_string(off, 36, ASCII_1608, totp.getName());
    uint32_t calculate = 0;
    int result = totp.calculate(&calculate);
    if (result == 0)
    {
        char text[7] = {
            static_cast<char>((calculate % 1000000UL) / 100000),
            static_cast<char>((calculate % 100000UL) / 10000),
            static_cast<char>((calculate % 10000UL) / 1000),
            static_cast<char>((calculate % 1000UL) / 100),
            static_cast<char>((calculate % 100UL) / 10),
            static_cast<char>((calculate % 10UL) / 1),
        };
        for (auto& i : text)
        {
            i += 48;
        }
        text[6] = '\0';
        sche.put_string(100, 60, ASCII_3216, text);
    }
}
