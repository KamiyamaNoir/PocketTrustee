#ifndef POCKETTRUSTEE_GUI_LOCK_HPP
#define POCKETTRUSTEE_GUI_LOCK_HPP

#include "gui.hpp"

extern gui::Window wn_locked;
extern gui::Window wn_pin;

void cb_backto_gui_pin(gui::Window& wn, gui::Display& dis, gui::ui_operation& opt);

#endif //POCKETTRUSTEE_GUI_LOCK_HPP