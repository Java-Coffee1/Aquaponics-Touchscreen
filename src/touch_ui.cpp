#include "touch_ui.h"
#include "screens.h"

std::string current_sensor_edit = "null";

int current_calibration_step = 0;
// 0 - not selected
// 1 - setting min
// 2 - setting max



///_____________________________ CODE FOR THE Button FORMAT_____________________________________
//shity code for the button on click will be changed later do do my sity coding
static void btn_event_cb(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * btn = lv_event_get_target(e);
        // Get sensor name from user data
        const char * sensor_name = (const char *)lv_event_get_user_data(e);
        printf("Button clicked: %s\n", sensor_name);
        lv_scr_load_anim(objects.twopoint_calibration, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0,false);
        current_sensor_edit = sensor_name;
        two_point_calibration_refresh();
    }
}

//back button to the screen
static void btn_back(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED){
        lv_scr_load_anim(objects.main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0,false);
    }
}


// Create sensor buttons
void create_sensor_list(lv_obj_t * parent)
{
    for (int i = 0; i < sensor_count; i++) {
        lv_obj_t * item = lv_btn_create(parent);
        lv_obj_set_width(item, lv_pct(100));
        lv_obj_set_height(item, 40);
        lv_obj_set_style_pad_all(item, 5, 0);
        lv_obj_t * label = lv_label_create(item);
        lv_label_set_text(label, sensors_list[i].c_str());
        lv_obj_center(label);
        lv_obj_add_event_cb(item, btn_event_cb, LV_EVENT_CLICKED, (void*)sensors_list[i].c_str());
    }
}

static void btn_set_cail_mtx(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    const char * txt =
        lv_btnmatrix_get_btn_text(
            obj,
            lv_btnmatrix_get_selected_btn(obj));

    if(txt) {
        if(strcmp(txt, "Set Min") == 0 || strcmp(txt, "set min") == 0) {
            current_calibration_step = 1;
        }
        else if(strcmp(txt, "Set Max") == 0 || strcmp(txt, "set max") == 0) {
            current_calibration_step = 2;
        }
        else {
            printf("Button pressed: %s", txt);
            fflush(stdout);
        }
    }
}

// ________________________________ CODE FOR THE INPUT TEXT AREA _____________________________________
static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);

    printf("Enter was pressed. The current text is: %s",
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

    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {
        lv_event_send(ta, LV_EVENT_READY, NULL);
        printf("Entered value: %s\n", lv_textarea_get_text(ta));
        if (current_calibration_step == 1) {
            write_sensor_value(current_sensor_edit.c_str(), MIN, atof(lv_textarea_get_text(ta)));
            current_calibration_step = 0;
            two_point_calibration_refresh();
        }
        else if (current_calibration_step == 2) {
            write_sensor_value(current_sensor_edit.c_str(), MAX, atof(lv_textarea_get_text(ta)));
            current_calibration_step = 0;
            two_point_calibration_refresh();
        }
        lv_textarea_set_text(ta, "");

    }
    else
        lv_textarea_add_text(ta, txt);
}
// ____________________ Create The Numpad _______________________

void create_numpad(lv_obj_t * parent, lv_obj_t * parent2)
{
    lv_obj_t * ta = lv_textarea_create(parent2);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 20);
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

// ________________________ Create the info Table ____________________________

static void table_style(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    /*If the cells are drawn...*/
    if(dsc->part == LV_PART_ITEMS) {
        uint32_t row = dsc->id /  lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Make the texts in the first cell center aligned*/
        if(row == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*In the first column align the texts to the right*/
        else if(col == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
        }

        /*MAke every 2nd row grayish*/
        if((row != 0 && row % 2) == 0) {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
    }
}

// Little helper to formt the valuss 
std::string fmt_float(float v, int precision = 2)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%.*f", precision, v);
    return std::string(buf);
}
void information_table(lv_obj_t * parent, const std::string &sensor_name)
{
    lv_obj_t * table = lv_table_create(parent);
    //get sensor type and value 
    std::string current  = fmt_float(get_sensor_value(sensor_name.c_str(), CURRENT));
    std::string avg      = fmt_float(get_sensor_value(sensor_name.c_str(), AVG));
    std::string max      = fmt_float(get_sensor_value(sensor_name.c_str(), MAX));
    std::string min      = fmt_float(get_sensor_value(sensor_name.c_str(), MIN));
    std::string last_min = fmt_float(get_sensor_value(sensor_name.c_str(), LAST_MIN));
    std::string last_max = fmt_float(get_sensor_value(sensor_name.c_str(), LAST_MAX));

    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "Name");
    lv_table_set_cell_value(table, 1, 0, "Current");
    lv_table_set_cell_value(table, 2, 0, "Average");
    lv_table_set_cell_value(table, 3, 0, "Min");
    lv_table_set_cell_value(table, 4, 0, "Max");
    lv_table_set_cell_value(table, 5, 0, "Last Min");
    lv_table_set_cell_value(table, 6, 0, "Last Max");

    /*Fill the second column*/
    lv_table_set_cell_value(table, 0, 1, "Value");
    lv_table_set_cell_value(table, 1, 1, current.c_str());
    lv_table_set_cell_value(table, 2, 1, avg.c_str());
    lv_table_set_cell_value(table, 3, 1, min.c_str());
    lv_table_set_cell_value(table, 4, 1, max.c_str());
    lv_table_set_cell_value(table, 5, 1, last_min.c_str());
    lv_table_set_cell_value(table, 6, 1, last_max.c_str());


    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_obj_set_height(table, 400);
    lv_obj_center(table);

    /*Add an event callback to to apply some custom drawing*/
    lv_obj_add_event_cb(table, table_style, LV_EVENT_DRAW_PART_BEGIN, NULL);
}


void two_point_calibration_refresh(){
    get_sensor_list();
    lv_obj_clean(objects.sensor_list);
    create_sensor_list(objects.sensor_list);
    information_table(objects.info_table, current_sensor_edit);
}

void setup_calibration_ui(){
    lv_obj_add_event_cb(objects.back, btn_back, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.set_cali, btn_set_cail_mtx, LV_EVENT_CLICKED, NULL);
    two_point_calibration_refresh();
}

void setup_main_ui(){
    create_numpad(objects.numpad, objects.number_area);
    lv_obj_set_layout(objects.sensor_list, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(objects.sensor_list,
                         LV_FLEX_FLOW_COLUMN);
    setup_calibration_ui();
    two_point_calibration_refresh();
}

