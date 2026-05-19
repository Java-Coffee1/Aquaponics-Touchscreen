#include "touch_ui.h"
#include "screens.h"


///_____________________________ CODE FOR THE BUTTONS FORMAT_____________________________________

// Global sensor list
static const char * sensors[] = {
    "ph_sensor",
    "Do_Sensor",
    "water_level_sensor1",
    "water_level_sensor2",
    "water_level_sensor3"
};

static const int num_of_sensors =
    sizeof(sensors) / sizeof(sensors[0]);

//shity code for the button on click will be changed later do do my sity codinh 
static void btn_event_cb(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * btn = lv_event_get_target(e);
        // Get sensor name from user data
        const char * sensor_name =
            (const char *)lv_event_get_user_data(e);

        printf("Button clicked: %s\n", sensor_name);
    }
}
// one of my examle funtions to make this thing work
// void create_sensor_list(lv_obj_t * parent, std::string sensor_name){
//     lv_obj_t * sensor_name_btn = lv_btn_create(parent);
//     lv_obj_set_size(sensor_name_btn, 120, 50);                          /*Set its size*/
//     lv_obj_add_event_cb(sensor_name_btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

//     lv_obj_t * label = lv_label_create(sensor_name_btn);          /*Add a label to the button*/
//     lv_label_set_text(label, sensor_name.c_str());                     /*Set the labels text*/
//     lv_obj_center(label);
//     lv_obj_set_user_data(sensor_name_btn, label);
//                           /*Store label handle for later retrieval*/
// }

// Create sensor buttons
void create_sensor_list(lv_obj_t * parent)
{
    for (int i = 0; i < num_of_sensors; i++) {

        lv_obj_t * item = lv_btn_create(parent);

        lv_obj_set_width(item, lv_pct(100));
        lv_obj_set_height(item, 40);

        lv_obj_set_style_pad_all(item, 5, 0);

        lv_obj_t * label = lv_label_create(item);

        lv_label_set_text(label, sensors[i]);

        lv_obj_center(label);

        // Pass sensor name into callback
        lv_obj_add_event_cb(item, btn_event_cb, LV_EVENT_CLICKED, (void*)sensors[i]);
    }
}


// ________________________________ CODE FOR THE INPUT TEXT AREA _____________________________________
static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);

    LV_LOG_USER("Enter was pressed. The current text is: %s",
                lv_textarea_get_text(ta));
}

static void btnm_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    lv_obj_t * ta =
        (lv_obj_t *)lv_event_get_user_data(e);

    const char * txt =
        lv_btnmatrix_get_btn_text(
            obj,
            lv_btnmatrix_get_selected_btn(obj));

    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        lv_textarea_del_char(ta);

    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0)
        lv_event_send(ta, LV_EVENT_READY, NULL);

    else
        lv_textarea_add_text(ta, txt);
}
// ____________________ Create The Thing _______________________

void create_numpad(lv_obj_t * parent, lv_obj_t * parent2)
{
    lv_obj_t * ta = lv_textarea_create(parent2);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(ta,
                        textarea_event_handler,
                        LV_EVENT_READY,
                        ta);

    lv_obj_add_state(ta, LV_STATE_FOCUSED);

    static const char * btnm_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        LV_SYMBOL_BACKSPACE, "0",
        LV_SYMBOL_NEW_LINE, ""
    };

    lv_obj_t * btnm = lv_btnmatrix_create(parent);

    lv_obj_set_size(btnm, 229, 239);

    lv_obj_align(btnm,
                 LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_btnmatrix_set_map(btnm, btnm_map);
}





void setup_ui()
{
    create_numpad(objects.numpad, objects.number_area);

    // Optional: auto-stack buttons vertically
    lv_obj_set_layout(objects.sensor_list, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(objects.sensor_list,
                         LV_FLEX_FLOW_COLUMN);

    create_sensor_list(objects.sensor_list);

}

