#include "get_sensor_data.h"

struct sensor_information
{
    int board_id;
    std::string sensor_name;
    double current_value;
    double min_value;
    double max_value;
    double sensor_hight;
};

sensor_information ph_sensor;

void get_sensor_data() {

    // convert the strut to a 2d array for easier access in the UI more.
    // this is passed java so have fun coding it futrure me looking over my shity ass code :)

    std::string sensor_data[4][4] = {
        // sensor_name, current_value, min_value, max_value
        {"pH Sensor", "7.2", "0.0", "14.0"},
        {"Temperature Sensor", "25.5", "-40.0", "125.0"},
        {"water level sensor", "50.0", "0.0", "100.0"},
        {"water level sensor 1", "50.0", "0.0", "100.0"}
    };
    
    // ph_sensor = {
    //     .board_id = 1,
    //     .sensor_name = "pH Sensor",
    //     .current_value = 7.2,
    //     .min_value = 0.0,
    //     .max_value = 14.0,
    //     .sensor_hight = -12345678
    // };  

    // if (ph_sensor.sensor_hight == -12345678) {

    //     // Access values directly
    //     std::string sensor_name = ph_sensor.sensor_name;
    //     double current_value = ph_sensor.current_value;
    //     double min_value = ph_sensor.min_value;
    //     double max_value = ph_sensor.max_value;

    //     // Example debug
    //     printf("Sensor: %s\n", sensor_name.c_str());
    // }
}
