#pragma once
#include "m1.h"
#include "ezgl/application.hpp" 
#include "ezgl/graphics.hpp"
#include <cstdlib>
//#include "universal_header.h"

using namespace std;
void create_vector_inter();

extern double max_lat;
extern double min_lat ;
extern double max_lon ;
extern double min_lon ;
extern float max_latitude;
extern float min_latitude;

struct Intersection_data {
    
    ezgl::point2d position;
    std::string name;
    bool highlighted;
    bool secondHighlight;
    
};

std::vector<Intersection_data> intersections_plot;


struct pin{
    
    ezgl::point2d position;
    std::string name;
    bool highlighted;
    
    std::string icon;
    
};


struct pin temp_pin;

bool new_pin_added;

bool print_pins = false;

bool fav_Mode = false;

 LatLon temp_pos;

int num_fav = 0;

void create_pin_vector();

std::vector<struct pin> all_pins;




