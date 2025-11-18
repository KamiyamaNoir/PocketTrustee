#include "gui.h"
#include "gui_component_list.h"

#define GUI_NAMECARD_CTRNUM 1
#define GUI_NAMECARD_LIST_PAGESIZE 6

using namespace gui;

extern Window wn_menu_page1;

ComponentList<GUI_NAMECARD_LIST_PAGESIZE, 25> namecard_list("namecard/", ".ncard");
static char on_select_card_path[40];

void clickon_namecard_list(Window& wn, Display& dis, ui_operation& opt);
void render_namecard_list(Scheme& sche, Control& self, bool onSelect);

Control controls_namecard[GUI_NAMECARD_CTRNUM]
{
    {0, 0, 1, 1, true, clickon_namecard_list, render_namecard_list}
};

ResourceDescriptor res_namecard
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_namecard(&res_namecard, controls_namecard, GUI_NAMECARD_CTRNUM);

void namecard_update()
{
    namecard_list.update();
}

void clickon_namecard_list(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (res_namecard.path == nullptr && namecard_list.on_select() != -1)
        {
            strcpy(on_select_card_path, "namecard/");
            strcat(on_select_card_path, namecard_list.seek(namecard_list.on_select()));
            strcat(on_select_card_path, ".ncard");
            res_namecard.path = on_select_card_path;
            dis.switchFocusLag(&wn);
            dis.refresh_count = 0;
        }
        else
        {
            res_namecard.path = nullptr;
            dis.switchFocusLag(&wn_menu_page1);
            dis.refresh_count = 0;
        }
        return;
    }
    if (res_namecard.path == nullptr)
    {
        if (namecard_list.move(opt))
        {
            namecard_list.update();
        }
    }
    opt = OP_NULL;
}

void render_namecard_list(Scheme& sche, Control& self, bool onSelect)
{
    if (namecard_list.item_count() == 0)
    {
        sche.put_string(92, 56, ASCII_1608, "Nothing Here :(");
    }
    else if (res_namecard.path == nullptr)
    {
        sche.clear();
        for (uint8_t i = 0; i < GUI_NAMECARD_LIST_PAGESIZE; i++)
        {
            sche.put_string(4, 5 + 20*i, ASCII_1608, namecard_list.seek(i));
            if (i == namecard_list.on_select())
            {
                sche.rectangle(3, 5+20*i, 201, 17, 1);
            }
        }
    }
}
