# Firmware patches

Following patches need to be applied when updating LVGL:

`src/extra/libs/lv_extra.c`

Comment the `lv_freetype_init()` portion. The actual initializatio is done in Firmware code
because parameters depend on Firmware defines and presence of PSRAM.

``` C
// Firmware Specific, the initialization is done in Firmware code to adjust with PSRAM
// #if LV_USE_FREETYPE
//     /*Init freetype library*/
// #  if LV_FREETYPE_CACHE_SIZE >= 0
//     lv_freetype_init(LV_FREETYPE_CACHE_FT_FACES, LV_FREETYPE_CACHE_FT_SIZES, LV_FREETYPE_CACHE_SIZE);
// #  else
//     lv_freetype_init(0, 0, 0);
// #  endif
// #endif
```

## lv_spinner

Both function `arc_anim_start_angle()` and `arc_anim_end_angle()` are made non-static. This allows to search for `lv_anim` object based on their `exec_cb` callbacks and change properties on-the-fly.