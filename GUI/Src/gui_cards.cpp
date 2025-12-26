#include "gui_component_list.h"
#include "gui_menup1.hpp"
#include "gui_cards.hpp"

#define GUI_CARDS_CTRNUM 1
#define GUI_CARDS_PAGESIZE 6

using namespace gui;

ComponentList<GUI_CARDS_PAGESIZE, 25> card_list("wifi/", ".wifi");

static char on_select_card_path[40];

static void clickon_cards_list(Window& wn, Display& dis, ui_operation& opt);
static void render_cards_list(Scheme& sche, Control& self, bool onSelect);

Control controls_cards[GUI_CARDS_CTRNUM]
{
    {0, 0, 1, 1, true, clickon_cards_list, render_cards_list}
};

ResourceDescriptor res_cards
{
    .path = nullptr,
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};

Window wn_cards(&res_cards, controls_cards, GUI_CARDS_CTRNUM);

void wn_cards_update()
{
    card_list.update();
}

void clickon_cards_list(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt == OP_ENTER)
    {
        if (res_cards.path == nullptr && card_list.on_select() != -1)
        {
            strcpy(on_select_card_path, "wifi/");
            strcat(on_select_card_path, card_list.seek(card_list.on_select()));
            strcat(on_select_card_path, ".wifi");
            res_cards.path = on_select_card_path;
            dis.switchFocusLag(&wn);
            dis.refresh_count = 0;
        }
        else
        {
            res_cards.path = nullptr;
            dis.switchFocusLag(&wn_menu_page1);
            dis.refresh_count = 0;
        }
        return;
    }
    if (res_cards.path == nullptr)
    {
        if (card_list.move(opt))
        {
            card_list.update();
        }
    }
    opt = OP_NULL;
}

void render_cards_list(Scheme& sche, Control& self, bool onSelect)
{
    if (card_list.item_count() == 0)
    {
        sche.put_string(92, 56, ASCII_1608, "Nothing Here :(");
    }
    else if (res_cards.path == nullptr)
    {
        sche.clear();
        for (uint8_t i = 0; i < GUI_CARDS_PAGESIZE; i++)
        {
            sche.put_string(4, 5 + 20*i, ASCII_1608, card_list.seek(i));
            if (i == card_list.on_select())
            {
                sche.rectangle(3, 5+20*i, 201, 17, 1);
            }
        }
    }
}