#ifndef UNIVERSAL_HEADER_H
#define UNIVERSAL_HEADER_H


#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "ezgl/graphics.hpp"
#include "ezgl/application.hpp"
#include <iostream>
#include <cmath>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <gtk/gtk.h>
#include "intersections.h"



#include <cstdlib>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <list>




//////////// ALL FUNCTIONS ARE DEFINED IN THEIR RESPECTIVE CPP FILES
//////////// THIS FILE IS JUST FOR DECLARATION

#define PI 3.14159265

struct deliveryPath{
    IntersectionIdx start_inter;
    
    IntersectionIdx end_inter;
    
    std::vector<StreetSegmentIdx> pathBetweenIntersections;
    
   //IntersectionIdx nextPickup;
    
};

struct locationInfo {

    //The intersection id where the item-to-be-delivered is picked-up.
    IntersectionIdx pick;

    //The intersection id where the item-to-be-delivered is dropped-off.
    IntersectionIdx drop;
    
    bool alreadyPicked = false;
    
    bool alreadyDropped = false;
};


std::vector<CourierSubPath> actualPath; 

std::vector<locationInfo> setAllDeliveryInfo(const std::vector<DeliveryInf>& deliveries);
//IntersectionIdx findClosestDepot(const std::vector<IntersectionIdx>& depots, IntersectionIdx deliveryPoint );

bool display_dir = true;
bool subway = false;
double xptr;
double yptr;
//bool zoomPin;

int clickCount = 0;
IntersectionIdx select_one;
IntersectionIdx select_two;

struct dataStructs {
    std::multimap<std::string, StreetIdx> namesOfStreets;

    std::vector<std::vector<StreetSegmentIdx> > vector_inter;

    std::multimap<int, int> streetLength;

    std::unordered_map<OSMID, LatLon> latLon;

    std::vector<double> travelLength;

    std::multimap< StreetIdx, IntersectionIdx> streetMap2;

    std::vector<std::vector<IntersectionIdx>> streetMap;


    std::unordered_map<OSMID, std::string> majorRoadType;

    std::unordered_map<OSMID, std::string> typePOI;

    std::multimap<double, struct featureInfo>::iterator featureIT;

    //std::vector<std::vector<std::string>> subwayLines;
    std::multimap<StreetSegmentIdx,struct streetData> segMap;
    std::multimap<StreetSegmentIdx, struct streetData >::iterator segMapIT;
    
   
    
    std::vector< std::pair<std::string, std::vector<ezgl::point2d>>> subwayInfo;
    std::vector< std::pair<std::string, std::vector<ezgl::point2d>>>::iterator subwayIT;

    std::multimap<double, struct featureInfo, std::greater<double> > featureMap;
    std::unordered_map<OSMID, const OSMNode *> indexNode;

} dataStructs;


struct featureInfo {
    FeatureIdx id; // feature id
    FeatureType type; // feature type
    int numPoints; // total number of x,y points of a feature
    
    
    std::vector<ezgl::point2d> all_points; // a vector to store all points
};

struct streetData{
    
    //int curvePts;
    //IntersectionIdx start, end;
    //OSMID osmIDNum;
    StreetSegmentIdx streetSegId;
    
    ezgl::point2d start_segment;
    ezgl::point2d end_segment;
    double angle;
    
    ezgl::point2d end_firstCurve;
     double firstCurveAngle;
      
    std::vector <ezgl::point2d> curvePoints_start; 
    std::vector <ezgl::point2d> curvePoints_end;
    std::vector <double> curveAngle;
    
    ezgl::point2d start_lastCurve;
   
    double lastCurveAngle;
    
};

IntersectionIdx inter_section_1;
IntersectionIdx inter_section_2;

 void createPin(GtkSearchEntry* search_bar, gpointer /*user_data*/);
double xptr1;
double yptr1;
double xptr2;
double yptr2;
double xptrs;
double yptrs;
bool zoomPin;
bool zoomPinFirst;
bool zoomPins;
bool zoomPinMain;
LatLon int_pos {0,0};

/// defining all the pictures and where to find them 
// all of the pictures were taken from OSM wiki page
// link: https://wiki.openstreetmap.org/wiki/Map_features#Amenity
ezgl::surface *pin = ezgl::renderer::load_png("libstreetmap/resources/pin.png");
ezgl::surface *res = ezgl::renderer::load_png("libstreetmap/resources/restaurant.png");
ezgl::surface *education = ezgl::renderer::load_png("libstreetmap/resources/education.png");
ezgl::surface *financial = ezgl::renderer::load_png("libstreetmap/resources/financial.png");
ezgl::surface *bar = ezgl::renderer::load_png("libstreetmap/resources/bar.png");
ezgl::surface *hospital = ezgl::renderer::load_png("libstreetmap/resources/hospital.png");
ezgl::surface *bus_station = ezgl::renderer::load_png("libstreetmap/resources/bus_station.png");
ezgl::surface *fuel = ezgl::renderer::load_png("libstreetmap/resources/fuel.png");
ezgl::surface *entertainment = ezgl::renderer::load_png("libstreetmap/resources/entertainment.png");


ezgl::surface *pickup = ezgl::renderer::load_png("libstreetmap/resources/pickup.png");
ezgl::surface *uni = ezgl::renderer::load_png("libstreetmap/resources/uni.png");
ezgl::surface *rendezvous= ezgl::renderer::load_png("libstreetmap/resources/rendezvous.png");
ezgl::surface *parking = ezgl::renderer::load_png("libstreetmap/resources/parking.png");
ezgl::surface *Drop = ezgl::renderer::load_png("libstreetmap/resources/Drop.png");
ezgl::surface *Restaurant = ezgl::renderer::load_png("libstreetmap/resources/Restaurant.png");
ezgl::surface *delivery = ezgl::renderer::load_png("libstreetmap/resources/delivery.png");
ezgl::surface *my_location = ezgl::renderer::load_png("libstreetmap/resources/my_location.png");
ezgl::surface *cursor = ezgl::renderer::load_png("libstreetmap/resources/cursor.png");
ezgl::surface *Default_Pin = ezgl::renderer::load_png("libstreetmap/resources/Default_Pin.png");
ezgl::surface *finish_pin = ezgl::renderer::load_png("libstreetmap/resources/finish_pin.png");



bool inter_already_highlighted = false;

bool drawPath;

IntersectionIdx first_highlighted_found = 0;

IntersectionIdx highlighted_id;

std::vector<IntersectionIdx> getIntersectionNames(StreetIdx id);

void initial_setup(ezgl::application* application, bool /*new window */);

void colour_converter(ezgl::renderer *g, std::string);

void draw_subway(ezgl::renderer *g);
void toggleSubway(GtkButton* subwayMode, ezgl::application* application);

void mapZoom(GtkSearchEntry* search_bar,gpointer user_data);

void mapChange(GtkComboBox* combo_box, gpointer user_data);

void mapSearch(GtkSearchEntry* search_bar , gpointer user_data);

void favMode(GtkComboBox* combo_box, gpointer user_data);

//void createPin(GtkSearchEntry* search_bar);


void draw_intersections(ezgl::renderer *g);
bool dark = false;
void toggleDark(GtkButton* darkMode, ezgl::application* application);

bool navMode = false;
bool helpMode = false;
bool favoriteMode = false;
bool initialMode = true;
bool firstBar = false;

void toggleNavigation(GtkButton* nav, ezgl::application* application);

void toggleHelp(GtkButton* /*help*/,ezgl::application* application);
void toggleFav(GtkButton* fav, ezgl::application* application);

void mapZoom1(GtkSearchEntry* search_bar, gpointer user_data);




void draw_main_canvas(ezgl:: renderer *g);

static ezgl::rectangle initial_world; //({-9e6,4e6},{-8e6, 5e6});

void text_POI(ezgl::renderer *g, int l);

//void text_roads(ezgl::renderer *g, double x1, double y1, double x2, double y2, StreetIdx id_street );

double getXfromLatLon(LatLon position, double latAverage);

double getYfromLatLon(LatLon position);

double getLatAverage();

double getXfromLon(double position);

double getYfromLat(double position);

void drawStreetSegments(StreetSegmentIdx streetSeg, ezgl::renderer *g, ezgl::rectangle outer_world, struct streetData);

void check_road(ezgl::renderer *g, std::string type);

void text_roads(ezgl::renderer *g, double x1, double y1, double x2, double y2, StreetSegmentInfo seg, double angle);

void draw_roads(ezgl::renderer *g, ezgl::rectangle outer_world);

void draw_png_example(ezgl::renderer *g);

void act_on_mouse_move(ezgl::application */*application*/, GdkEventButton */*event*/, double x, double y);

double xMin, xMax, yMin, yMax, latAverage;

double ep = 0.000000000001;

void draw_features(ezgl::renderer *g);

void draw_buildings(ezgl::renderer *g);

void draw_POIs(ezgl::renderer *g);

void draw_background(ezgl::renderer *g,ezgl::rectangle world);

void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y);

double previous_click_x;

double previous_click_y;

IntersectionIdx previous_inter_id;
IntersectionIdx previous_inter_id2;

double lon_from_x(double x);

double lat_from_y(double y);

bool pathDisplay;
bool pathMode;

//std::vector <StreetSegmentIdx> back_trace;
void showPath(ezgl::application* app, ezgl::renderer *g, std::vector<StreetSegmentIdx> displayPath);


void setAllPins(bool state);

void setIcon(GtkComboBox* search_bar, gpointer user_data);



//********************************************



#define NO_EDGE -1


struct waveObject {
    IntersectionIdx interID;
    StreetSegmentIdx trackEdge;
    double timePeriod;
    double heuristic_parameter;
    
    waveObject(IntersectionIdx x, StreetSegmentIdx y, double time, double h) {
        interID = x; trackEdge = y; timePeriod = time; heuristic_parameter = h;
        //heuristic_parameter = h;
    }
    
};

struct tempObject{ //temp memory to check if new path has lower bestTime

    StreetSegmentIdx trackEdge;
    double bestTime;
    //double IntersectionIdx inter_id;
    
    
    tempObject(StreetSegmentIdx id, double t){
        trackEdge = id; bestTime = t;
    }
    
};

struct newTempObject{ //temp memory to check if new path has lower bestTime

    StreetSegmentIdx trackEdge;
    double bestTime;
    IntersectionIdx inter;
    bool pick = false;
    bool drop = false;
    bool depot = false;
    
    newTempObject(StreetSegmentIdx id, double t,IntersectionIdx inte,bool x, bool y, bool z){
        trackEdge = id; bestTime = t; inter = inte;pick = x; drop = y; depot = z;
    }
    
};

class compareTime{
public:
    bool operator() (const waveObject& time1, const waveObject& time2)
    {
        return time1.timePeriod > time2.timePeriod;//DOUBT: = or no?
        //
    }
};


struct end_point_info{
    IntersectionIdx end_point;
    double time;
    std::vector<StreetSegmentIdx> path;
    
   
};

class compareHeuristicTime{ //operator for heuristic time
public:
    bool operator() (const waveObject& time1, const waveObject& time2)
    {
        return time1.heuristic_parameter > time2.heuristic_parameter;//DOUBT: = or no?
        //
    }
};

 bool cmpBestTime(end_point_info& lhs, end_point_info& rhs);

  bool cmpBestTime(end_point_info& lhs, end_point_info& rhs)
    {
            return lhs.time < rhs.time;
    }

std::vector <tempObject> best_time_tracker; //initialize with large value

std::vector <StreetSegmentIdx> print_vec;



bool display_path = false;


std::string stringSwap(std::string s, std::string newS);

void mapSearch1(GtkSearchEntry* search_bar, gpointer user_data);


bool rightTurn(StreetSegmentIdx to, StreetSegmentIdx from);

typedef int DirectionIdx;

std::vector <DirectionIdx> temp_dir;

DirectionIdx getTurnId(IntersectionIdx from_inter, IntersectionIdx con_inter, IntersectionIdx to_inter);

std::vector <DirectionIdx> GetDirectionIds(const std::vector<StreetSegmentIdx> path);


//std::unordered_map<IntersectionIdx,std::vector<end_point_info>> good_times;
std::vector<std::vector<end_point_info>> better_times;

struct index_info{
    int delivery_index;
    bool pickup;
    
    index_info(int x, bool y){
        delivery_index = x;
        pickup = y;
    }
    
};

std::multimap <IntersectionIdx,index_info> indexer;

//*****************************************************
struct newWaveObject {
    IntersectionIdx interID;
    StreetSegmentIdx trackEdge;
    double timePeriod;
    
    newWaveObject(IntersectionIdx x, StreetSegmentIdx y, double time) {
        interID = x; trackEdge = y; timePeriod = time;
    }
    
};

class newCompareTime{
public:
    bool operator() (const newWaveObject& time1, const newWaveObject& time2)
    {
        return time1.timePeriod > time2.timePeriod;
    }
};

std::vector <tempObject> new_best_time_tracker;

std::vector <StreetSegmentIdx> back_trace_vec(IntersectionIdx tracing_id);

std::unordered_map<IntersectionIdx,std::vector<tempObject>>shortest_trav_times;

//std::pair<double,std::vector<StreetSegmentIdx>> findMultiPathBetweenIntersections(const double turn_penalty,const std::pair<IntersectionIdx, IntersectionIdx> intersect_ids);
//std::vector<tempObject> findMultiPathBetweenIntersections(const double turn_penalty,IntersectionIdx intersect_id);
void findMultiPathBetweenIntersections(const double turn_penalty,IntersectionIdx intersect_id,const std::vector<DeliveryInf>& deliveries);

struct list_elem{
    IntersectionIdx pick;
    IntersectionIdx drop;
    
    bool pick_status;
    bool drop_status;
    
};

#define HARD_LEFT 1
#define SHARP_LEFT 2
#define SLIGHT_LEFT 3
#define STRAIGHT 4
#define SLIGHT_RIGHT 5
#define SHARP_RIGHT 6
#define HARD_RIGHT 7
#define SAME_STREET 8
#define SAME_LOC -3



struct displayInstr{
    double angle;
    std::string instruction;
    std::string distance;
};

std::vector<displayInstr> displayDir;


GtkImage* createImage(double angle);
void instructionsAdding(displayInstr &holdingInfo,GtkListBox* insertBox);
void createListBox();
void clearScrollWin(ezgl::application *application);

//std::vector<std::vector<std::pair<double,std::vector<StreetSegmentIdx>>>> pathDrop;
//std::vector<std::vector<std::pair<double,std::vector<StreetSegmentIdx>>>> dropDepot;
//std::vector<std::vector<std::pair<double,std::vector<StreetSegmentIdx>>>> pathDepot;

std::vector<CourierSubPath> greedyAlgo(const float turn_penalty, const std::vector<DeliveryInf>& deliveries, const std::vector<IntersectionIdx>& depots);

std::vector<StreetSegmentIdx> backtracer(IntersectionIdx tracer, IntersectionIdx tracerId,StreetSegmentIdx previousEdge,
                    std::vector <StreetSegmentIdx> back_traced);

#endif


