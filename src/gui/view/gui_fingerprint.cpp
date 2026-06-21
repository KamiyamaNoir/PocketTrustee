#include "gui_fingerprint.hpp"
#include "gui_menup1.hpp"
// #include "bsp_finger.h"
#include "fingerprint.hpp"

#define GUI_FINGERPRINT_CTRNUM 4
#define GUI_FG_AUTHEN_CTRNUM 1

using namespace gui;

// static void clickon_fingerprint_exit(Window& wn, Display& dis, ui_operation& opt);
static void clickon_fgauthen_cancel(Window& wn, Display& dis, ui_operation& opt);
static void clickon_fgmanage_enroll(Window& wn, Display& dis, ui_operation& opt);
static void clickon_fgmanage_test(Window& wn, Display& dis, ui_operation& opt);
static void clickon_fgmanage_del(Window& wn, Display& dis, ui_operation& opt);

static Control controls_fingerprint[GUI_FINGERPRINT_CTRNUM]
{
    {228, 4, 63, 19, true, cb_backto_menup1, render_rectangle},
    {30, 94, 60, 23, true, clickon_fgmanage_enroll, render_rectangle},
    {112, 94, 60, 23, true, clickon_fgmanage_test, render_rectangle},
    {192, 94, 60, 23, true, clickon_fgmanage_del, render_rectangle},

};
static Control controls_fg_authen[GUI_FG_AUTHEN_CTRNUM]
{
    {115, 87, 67, 27, true, clickon_fgauthen_cancel, render_rectangle}
};

static ResourceDescriptor res_fingerprint
{
    .path = "gui_fg/fgmn",
    .rx = 0,
    .ry = 0,
    .rw = GUI_WIDTH,
    .rh = GUI_HEIGHT,
};
static ResourceDescriptor res_fg_authen
{
    .path = "gui_fg/authen",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};
static ResourceDescriptor res_fg_pass
{
    .path = "gui_fg/pass",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};
static ResourceDescriptor res_fg_fail
{
    .path = "gui_fg/fail",
    .rx = 38,
    .ry = 8,
    .rw = 220,
    .rh = 112,
};

Window wn_fingerprint(&res_fingerprint, controls_fingerprint, GUI_FINGERPRINT_CTRNUM);
Window wn_fg_authen(&res_fg_authen, controls_fg_authen, GUI_FG_AUTHEN_CTRNUM);
Window wn_fg_pass(&res_fg_pass, controls_fg_authen, GUI_FG_AUTHEN_CTRNUM);
Window wn_fg_fail(&res_fg_fail, controls_fg_authen, GUI_FG_AUTHEN_CTRNUM);

// void clickon_fingerprint_exit(Window& wn, Display& dis, ui_operation& opt)
// {
//     if (opt != OP_ENTER) return;
//     dis.switchFocusLag(&wn_menu_page1);
//     dis.refresh_count = 0;
// }

void clickon_fgauthen_cancel(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    Fingerprint::cancelOperation();
}

static FingerprintRequest fg_req;

void clickon_fgmanage_enroll(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    fg_req = {
        .dis = &dis,
        .wn_src = &wn,
        .result_callback = nullptr,
    };
    Fingerprint::Enroll(&fg_req);
}

void clickon_fgmanage_test(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    fg_req = {
        .dis = &dis,
        .wn_src = &wn,
        .result_callback = nullptr,
    };
    Fingerprint::Authen(&fg_req);
}

void clickon_fgmanage_del(Window& wn, Display& dis, ui_operation& opt)
{
    if (opt != OP_ENTER) return;
    fg_req = {
        .dis = &dis,
        .wn_src = &wn,
        .result_callback = nullptr,
    };
    Fingerprint::clearAllFinger(&fg_req);
}
