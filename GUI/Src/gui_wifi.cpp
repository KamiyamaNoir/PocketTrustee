#include "gui.h"
#include "gui_component_list.h"

#define GUI_WIFI_SHARE_CTRNUM 1
#define GUI_WIFI_LIST_PAGESIZE 6

using namespace gui;

extern Window wn_menu_page1;

ComponentList<GUI_WIFI_LIST_PAGESIZE, 25> wifi_list("wifi/", ".wifi");
static char on_select_card_path[40];

void clickon_wifi_list(Window& wn, Display& dis, ui_operation& opt);
void render_wifi_list(Scheme& sche, Control& self, bool onSelect);

Control controls_wifi_share[GUI_WIFI_SHARE_CTRNUM]
{
    {0, 0, 1, 1, true, clickon_wifi_list, render_wifi_list}
};

ResourceDescriptor res_wifi_share
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_wifi_share(&res_wifi_share, controls_wifi_share, GUI_WIFI_SHARE_CTRNUM);

void wifi_card_update()
{
    wifi_list.update();
}

void clickon_wifi_list(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (res_wifi_share.path == nullptr && wifi_list.on_select() != -1)
        {
            strcpy(on_select_card_path, "wifi/");
            strcat(on_select_card_path, wifi_list.seek(wifi_list.on_select()));
            strcat(on_select_card_path, ".wifi");
            res_wifi_share.path = on_select_card_path;
            dis.switchFocusLag(&wn);
            dis.refresh_count = 0;
        }
        else
        {
            res_wifi_share.path = nullptr;
            dis.switchFocusLag(&wn_menu_page1);
            dis.refresh_count = 0;
        }
        return;
    }
    if (res_wifi_share.path == nullptr)
    {
        if (wifi_list.move(opt))
        {
            wifi_list.update();
        }
    }
    opt = OP_NULL;
}

void render_wifi_list(Scheme& sche, Control& self, bool onSelect)
{
    if (wifi_list.item_count() == 0)
    {
        sche.put_string(92, 56, ASCII_1608, "Nothing Here :(");
    }
    else if (res_wifi_share.path == nullptr)
    {
        sche.clear();
        for (uint8_t i = 0; i < GUI_WIFI_LIST_PAGESIZE; i++)
        {
            sche.put_string(4, 5 + 20*i, ASCII_1608, wifi_list.seek(i));
            if (i == wifi_list.on_select())
            {
                sche.rectangle(3, 5+20*i, 201, 17, 1);
            }
        }
    }
}
