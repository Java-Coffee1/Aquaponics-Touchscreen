#include <Arduino.h>
#include "rgb_lcd_port.h"
#include "gt911.h"
#include "lvgl_port.h"
// #include <demos/lv_demos.h>
#include "ui.h"
// #include "screens.h"

// Define menu items structure
typedef struct {
    const char *name;
    float price;
} menu_item_t;

// Menu items data
const menu_item_t menu_items[] = {
    {"Classic Beef", 6.50},
    {"Smoky BBQ Chicken", 7.00},
    {"Spicy Jalapeno", 7.25},
    {"Chilled Cola", 1.50},
    {"Fresh Lemon Mint", 2.50}
};

// Global variables to track order
float total_price = 0.0;
int item_count = 0;
lv_obj_t **item_objects = NULL;  // Array to store created item objects
#define MAX_ITEMS 10  // Maximum number of items in the order

// Function to add a new item to the order list
void add_item_to_order(int item_index) {
    if (item_index < 0 || item_index >= sizeof(menu_items) / sizeof(menu_items[0]) || item_count >= MAX_ITEMS) {
        return;  // Invalid index or max items reached
    }

    // Get the menu item data
    const menu_item_t *item = &menu_items[item_index];
    
    // Update total price
    total_price += item->price;
    
    // Show the template item if this is the first item in the order
    if (item_count == 0) {
        lv_obj_clear_flag(objects.items, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(objects.item, item->name);
        
        char price_str[10];
        snprintf(price_str, sizeof(price_str), "%.2f", item->price);
        lv_label_set_text(objects.price, price_str);
    } 
    // Otherwise create a new item
    else {
        // Create a duplicate of the items object
        lv_obj_t *new_item = lv_obj_create(lv_obj_get_parent(objects.items));
        
        // Position the new item below the previous items (36 pixels per item height)
        lv_obj_set_pos(new_item, lv_obj_get_x(objects.items), lv_obj_get_y(objects.items) + item_count * 36);
        lv_obj_set_size(new_item, lv_obj_get_width(objects.items), lv_obj_get_height(objects.items));
        
        // Copy styles from the template item
        lv_obj_set_style_pad_left(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(new_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(new_item, lv_color_hex(0xffa2a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(new_item, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(new_item, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // Create item name label
        lv_obj_t *name_label = lv_label_create(new_item);
        lv_obj_set_pos(name_label, lv_obj_get_x(objects.item), lv_obj_get_y(objects.item));
        lv_obj_set_size(name_label, lv_obj_get_width(objects.item), lv_obj_get_height(objects.item));
        lv_obj_set_style_text_font(name_label, lv_obj_get_style_text_font(objects.item, LV_PART_MAIN | LV_STATE_DEFAULT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(name_label, item->name);
        
        // Create price label
        lv_obj_t *price_label = lv_label_create(new_item);
        lv_obj_set_pos(price_label, lv_obj_get_x(objects.price), lv_obj_get_y(objects.price));
        lv_obj_set_size(price_label, lv_obj_get_width(objects.price), lv_obj_get_height(objects.price));
        lv_obj_set_style_text_font(price_label, lv_obj_get_style_text_font(objects.price, LV_PART_MAIN | LV_STATE_DEFAULT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(price_label, lv_obj_get_style_text_align(objects.price, LV_PART_MAIN | LV_STATE_DEFAULT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(price_label, lv_obj_get_style_text_color(objects.price, LV_PART_MAIN | LV_STATE_DEFAULT), LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // Format price with 2 decimal places
        char price_str[10];
        snprintf(price_str, sizeof(price_str), "%.2f", item->price);
        lv_label_set_text(price_label, price_str);
        
        // Store the new item in the array
        if (item_objects == NULL) {
            item_objects = (lv_obj_t **)malloc(MAX_ITEMS * sizeof(lv_obj_t *));
            // Initialize all pointers to NULL
            for (int i = 0; i < MAX_ITEMS; i++) {
                item_objects[i] = NULL;
            }
        }
        item_objects[item_count - 1] = new_item;  // -1 because first item is in the template
    }
    
    // Update total amount displays
    char total_str[10];
    snprintf(total_str, sizeof(total_str), "%.2f", total_price);
    lv_label_set_text(objects.total_amount_1, total_str);
    lv_label_set_text(objects.total_amount, total_str);
    
    item_count++;
}

// Function to clear the order
void clear_order() {
    // Hide the template item
    lv_obj_add_flag(objects.items, LV_OBJ_FLAG_HIDDEN);
    
    // Delete all created item objects
    if (item_objects != NULL) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (item_objects[i] != NULL) {
                lv_obj_del(item_objects[i]);
                item_objects[i] = NULL;
            }
        }
    }
    
    // Reset variables
    total_price = 0.0;
    item_count = 0;
    
    // Reset total displays
    lv_label_set_text(objects.total_amount_1, "0.00");
    lv_label_set_text(objects.total_amount, "0.00");
}

// Button event handler
static void button_matrix_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_CLICKED) {
        uint32_t btn_id = lv_btnmatrix_get_selected_btn(obj);
        add_item_to_order(btn_id);
    }
}

// Handle clicks on QR area to dismiss it
static void qr_area_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        // Hide QR code and clear order
        lv_obj_add_flag(objects.qr, LV_OBJ_FLAG_HIDDEN);
        clear_order();
    }
}

// Handle Place Order button event
static void place_order_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        // Only show QR if there are items in the order
        if (item_count > 0) {
            // Show QR code
            lv_obj_clear_flag(objects.qr, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

// Function to set up event handlers
void setup_ui_events() {
    // Add event handler to button matrix
    lv_obj_add_event_cb(objects.button_matrix, button_matrix_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Add event handler to Place Order button
    lv_obj_add_event_cb(objects.obj8, place_order_btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Add event handler to QR code area for dismissal and clearing
    lv_obj_add_event_cb(objects.qr, qr_area_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.qr_1, qr_area_event_cb, LV_EVENT_CLICKED, NULL);
}

void setup() {
    Serial.begin(115200);
    static esp_lcd_panel_handle_t panel_handle = NULL;
    static esp_lcd_touch_handle_t tp_handle = NULL;

    tp_handle = touch_gt911_init();  
    panel_handle = waveshare_esp32_s3_rgb_lcd_init(); 
    wavesahre_rgb_lcd_bl_on();   
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    ESP_LOGI(TAG, "Display LVGL demos");

    if (lvgl_port_lock(-1)) {
        ui_init();
        
        // Hide QR code initially
        lv_obj_add_flag(objects.qr, LV_OBJ_FLAG_HIDDEN);
        
        // Initialize item_objects array to NULL
        item_objects = NULL;
        
        // Initialize the order display
        clear_order();
        
        // Setup UI event handlers
        setup_ui_events();
        
        lvgl_port_unlock();
    }
}

void loop() {
    // LVGL events handle interactions
}