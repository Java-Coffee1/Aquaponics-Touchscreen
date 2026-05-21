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
    SCREEN_ID_TWOPOINT_CALIBRATION = 2,
    SCREEN_ID_SENSOR_HIGHT = 3,
    _SCREEN_ID_LAST = 3
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *twopoint_calibration;
    lv_obj_t *sensor_hight;
    lv_obj_t *sensor_list;
    lv_obj_t *number_area;
    lv_obj_t *numpad;
    lv_obj_t *back;
    lv_obj_t *info_table;
    lv_obj_t *set_cali;
    lv_obj_t *info_table_hight;
    lv_obj_t *back_btn_hight;
    lv_obj_t *numpad_hight;
    lv_obj_t *number_area_hight;
    lv_obj_t *set_new_hight_offset;
    lv_obj_t *btn_hight_offset;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_twopoint_calibration();
void tick_screen_twopoint_calibration();

void create_screen_sensor_hight();
void tick_screen_sensor_hight();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/