#include "gui.h"
#include "bsp_rfid.h"
#include <cstring>
#include <cstdio>
#include "cmsis_os.h"
#include "gui_resource.h"
#include "lfs_base.h"
#include "bsp_rtc.h"

#define GUI_IDCARD_CTRNUM 3
#define GUI_IDCARD_SEL_CTRNUM 2
#define GUI_IDCARD_SAVED_CTRNUM 1
#define GUI_IDCARD_EMULATE_CTRNUM 1
#define IDCARD_PAGE_SIZE 5

using namespace gui;
using rfid::IDCard;

constexpr char idcard_dir_path[] = "idcards/";
static uint8_t idcard_item_count = 0;
static volatile int8_t item_ptr = -1;
static uint32_t idcard_dir_index_map[IDCARD_PAGE_SIZE];
static IDCard current_idcard {};
char emulate_idcard_name[25] = "";

extern Window wn_menu_page1;
extern Window wn_cds;

void clickon_idcard_start(Window& wn, Display& dis, ui_operation& opt);
void clickon_idcard_save(Window& wn, Display& dis, ui_operation& opt);
void clickon_iccard_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_idcard_sel_exit(Window& wn, Display& dis, ui_operation& opt);
void clickon_idcard_sel(Window& wn, Display& dis, ui_operation& opt);
void clickon_idcard_saved_ok(Window& wn, Display& dis, ui_operation& opt);
void clickon_idcard_emulate(Window& wn, Display& dis, ui_operation& opt);

extern void render_rectangle(Scheme& sche, Control& self, bool onSelect);
void render_idcard(Scheme& sche, Control& self, bool onSelect);
void render_idcard_select(Scheme& sche, Control& self, bool onSelect);
void render_idcard_saved(Scheme& sche, Control& self, bool onSelect);
void render_idcard_emulate(Scheme& sche, Control& self, bool onSelect);

Control controls_idcard[GUI_IDCARD_CTRNUM]
{
    // Back to MENU
    {228, 2, 67, 21, true, clickon_iccard_exit, render_idcard},
    // Start read card
    {5, 98, 107, 27, true, clickon_idcard_start, render_rectangle},
    // Save
    {136, 98, 70, 28, true, clickon_idcard_save, render_rectangle},
};
Control controls_idcard_select[GUI_IDCARD_SEL_CTRNUM]
{
    // Back to MENU
    {229, 1, 64, 24, true, clickon_idcard_sel_exit, render_idcard_select},
    // Select box
    {4, 5, 288, 18, true, clickon_idcard_sel, nullptr},
};
Control controls_idcard_saved[GUI_IDCARD_SAVED_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_idcard_saved_ok, render_idcard_saved}
};
Control controls_idcard_emulate[GUI_IDCARD_EMULATE_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_idcard_emulate, render_idcard_emulate}
};

ResourceDescriptor res_idcard
{
    .path = "gui_idcard/read_idcard",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_idcard_select
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
ResourceDescriptor res_idcard_saved
{
    .path = "gui_pwd/saved",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};
ResourceDescriptor res_idcard_emulate
{
    .path = "gui_idcard/emulate",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_idcard(&res_idcard, controls_idcard, GUI_IDCARD_CTRNUM);
Window wn_idcard_select(&res_idcard_select, controls_idcard_select, GUI_IDCARD_SEL_CTRNUM);
Window wn_idcard_saved(&res_idcard_saved, controls_idcard_saved, GUI_IDCARD_SAVED_CTRNUM);
Window wn_idcard_emulate(&res_idcard_emulate, controls_idcard_emulate, GUI_IDCARD_EMULATE_CTRNUM);

static void load_idcard()
{
    current_idcard = {};
    emulate_idcard_name[0] = '\0';
    if (item_ptr >= idcard_item_count || item_ptr < 0)
    {
        return;
    }
    auto dir = LittleFS::fs_dir_handler(idcard_dir_path);
    dir.seek(idcard_dir_index_map[item_ptr]);
    lfs_info info;
    dir.next(&info);
    char path[32] = "idcards/";
    strcat(path, info.name);
    IDCard cache {};
    auto fs = LittleFS::fs_file_handler(path);
    fs.read(reinterpret_cast<uint8_t*>(&cache), sizeof(IDCard));
    if (cache.idcode != 0)
    {
        current_idcard = cache;
        uint8_t len = strlen(info.name) - 5;
        memcpy(emulate_idcard_name, info.name, len);
        emulate_idcard_name[len] = '\0';
        rfid::loadEmulator(&cache);
    }
}

void idcard_dir_update()
{
    idcard_item_count = 0;
    item_ptr = -1;
    auto dir = LittleFS::fs_dir_handler(idcard_dir_path);
    int dir_count = dir.count();
    if (dir_count < 0) return;

    dir.rewind();
    for (int i = 0; i < dir_count; i++)
    {
        lfs_info info {};
        uint32_t pos = dir.tell();
        dir.next(&info);
        if (info.type == LFS_TYPE_DIR) continue;
        uint32_t len = strlen(info.name);
        if (len < 5) continue;
        if (memcmp(info.name + len - 5, ".card", 5) != 0)
            continue;
        idcard_dir_index_map[idcard_item_count] = pos;
        idcard_item_count++;
    }
}

extern osThreadId defaultTaskHandle;
static IDCard idcard_idcard {};
volatile bool idcard_onwaiting = false;

void rfid::RFID_BufferFullCallback()
{
    if (!idcard_onwaiting) return;
    idcard_onwaiting = false;
    auto rtvl = parseSample(&idcard_idcard);
    if (rtvl != BSP_OK)
    {
        idcard_idcard = {};
    }
    GUI_OP_NULL_ISR();
}

void clickon_idcard_start(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    idcard_idcard = {};
    core::StartIdealTask();
    rfid::newSample();
    idcard_onwaiting = true;
}

static char saved_name[26];

void clickon_idcard_save(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    if (idcard_idcard.idcode == 0) return;
    rtc::TimeDate td;
    rtc::getTimedate(&td);
    char name[26];
    sprintf(name, "%d-%d-%d %d-%d-%d", td.year, td.month, td.day, td.hour, td.minute, td.second);
    memcpy(saved_name, name, 26);
    strcat(name, ".card");
    char path[42] = "idcards/";
    strcat(path, name);
    auto fs = LittleFS::fs_file_handler(path);
    fs.write(reinterpret_cast<uint8_t*>(&idcard_idcard), sizeof(IDCard));
    dis.switchFocusLag(&wn_idcard_saved);
    dis.refresh_count = 11;
}

void clickon_iccard_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    idcard_idcard = {};
    idcard_onwaiting = false;
    rfid::set_drive_mode(rfid::STOP);
    core::StopIdealTask();
    dis.switchFocusLag(&wn_menu_page1);
    dis.refresh_count = 0;
}

void clickon_idcard_saved_ok(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    idcard_dir_update();
    idcard_idcard = {};
    saved_name[0] = '\0';
    dis.switchFocusLag(&wn_idcard);
    dis.refresh_count = 0;
}

void render_idcard_saved(Scheme& sche, Control& self, bool onSelect)
{
    uint16_t off_x = GUI_WIDTH/2 - 4*strlen(saved_name);
    sche.put_string(off_x, 54, ASCII_1608, saved_name);
    sche.rectangle(self.x, self.y, self.w, self.h, 2);
}

void render_idcard(Scheme& sche, Control& self, bool onSelect)
{
    render_rectangle(sche, self, onSelect);
    char idcode[11];
    if (idcard_idcard.idcode != 0) sprintf(idcode, "%ld", idcard_idcard.idcode);
    else idcode[0] = '\0';
    uint16_t off_x = 209 - 4*strlen(idcode);
    if (idcode[0] != '\0') sche.put_string(off_x, 51, ASCII_1608, idcode);

    if (idcard_onwaiting)
        strcpy(idcode, "Waiting");
    else
        strcpy(idcode, "Stop");
    off_x = 45 - 4*strlen(idcode);
    sche.put_string(off_x, 51, ASCII_1608, idcode);
}

void render_idcard_select(Scheme& sche, Control& self, bool onSelect)
{
    // Display icon
    sche.texture(icon_bk_menu, self.x, self.y, self.w, self.h);
    if (onSelect)
    {
        sche.rectangle(self.x, self.y, self.w, self.h, 2);
    }
    else if (item_ptr < 0)
    {
        item_ptr = 0;
    }
    // Display item
    auto dir = LittleFS::fs_dir_handler(idcard_dir_path);
    for (uint8_t i = 0; i < IDCARD_PAGE_SIZE; i++)
    {
        lfs_info info;
        if (i >= idcard_item_count) break;
        dir.seek(idcard_dir_index_map[i]);
        dir.next(&info);
        char name[25];
        uint8_t name_size = strlen(info.name) - 4;
        memcpy(name, info.name, name_size);
        name[name_size] = '\0';
        sche.put_string(4, 5 + 20*i, ASCII_1608, name);
        if (i == item_ptr)
        {
            sche.rectangle(3, 5+20*i, 201, 17, 1);
        }
    }
}

void clickon_idcard_sel_exit(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    dis.switchFocusLag(&wn_cds);
    dis.refresh_count = 0;
}

void clickon_idcard_sel(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        load_idcard();
        rfid::set_drive_mode(rfid::EMULATE);
        dis.switchFocusLag(&wn_idcard_emulate);
        dis.refresh_count = 11;
        return;
    }
    if (idcard_item_count == 0) return;
    if (opt == OP_UP && item_ptr == 0)
    {
        item_ptr = -1;
        return;
    }
    if (opt == OP_DOWN && item_ptr == idcard_item_count-1)
    {
        item_ptr = -1;
        return;
    }
    //NOLINTNEXTLINE
    item_ptr += opt;
    opt = OP_NULL;
}

void clickon_idcard_emulate(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    rfid::set_drive_mode(rfid::STOP);
    current_idcard = {};
    emulate_idcard_name[0] = '\0';
    rfid::clearEmulator();
    dis.switchFocusLag(&wn_idcard_select);
    dis.refresh_count = 0;
}

void render_idcard_emulate(Scheme& sche, Control& self, bool onSelect)
{
    sche.rectangle(self.x, self.y, self.w, self.h, 2);
    uint16_t off_x = GUI_WIDTH/2 - 4*strlen(emulate_idcard_name);
    sche.put_string(off_x, 60, ASCII_1608, emulate_idcard_name);
}
