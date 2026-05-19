#pragma once
#include <iostream>


extern std::string sensor_name;
extern double current_value;
extern double min_value;
extern double max_value;
extern double sensor_hight;

void get_sensor_data();