/********************************************************************
 * Firmware LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

extern int lv0_start(bvm *vm);

extern int lv0_register_button_encoder(bvm *vm);  // add buttons with encoder logic

extern int lv0_load_montserrat_font(bvm *vm);
extern int lv0_load_seg7_font(bvm *vm);
extern int lv0_load_robotocondensed_latin1_font(bvm *vm);
extern int lv0_load_font_embedded(bvm *vm);

extern int lv0_screenshot(bvm *vm);
extern int lv0_load_freetype_font(bvm *vm);

#include "solidify/solidified_lv_Firmware.h"

/* @const_object_info_begin
module lv_Firmware (scope: global, strings: weak) {
    init, closure(lv_Firmware_init_closure)

    start, func(lv0_start)
    splash, closure(lv_Firmware_splash_closure)
    splash_init, closure(lv_Firmware_splash_init_closure)
    splash_remove, closure(lv_Firmware_splash_remove_closure)

    font_montserrat, func(lv0_load_montserrat_font)
    montserrat_font, func(lv0_load_montserrat_font)
    font_seg7, func(lv0_load_seg7_font)
    seg7_font, func(lv0_load_seg7_font)
    font_embedded, func(lv0_load_font_embedded)
    register_button_encoder, func(lv0_register_button_encoder)
    screenshot, func(lv0_screenshot)

    load_freetype_font, func(lv0_load_freetype_font)
}
@const_object_info_end */
#include "be_fixed_lv_Firmware.h"

#endif // USE_LVGL