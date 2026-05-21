#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_CALIBRATION = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *calibration;
    lv_obj_t *sensor_list;
    lv_obj_t *number_area;
    lv_obj_t *numpad;
    lv_obj_t *back;
    lv_obj_t *change_value;
    lv_obj_t *info_table;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_calibration();
void tick_screen_calibration();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/