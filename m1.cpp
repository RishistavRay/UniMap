/*
 * Copyright 2022 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated
 * documentation files (the "Software") in course work at the University
 * of Toronto, or for personal use. Other uses are prohibited, in
 * particular the distribution of the Software either publicly or to third
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <iostream>
#include <map>
#include <cmath>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
#include <cstdlib>
#include <thread>

#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include <gtk/gtk.h>
#include "ezgl/graphics.hpp"
#include "ezgl/application.hpp"
#include "intersections.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "universal_header.h"


#include <chrono>
#include <fstream>


void OSMLoad(std::string map_streets_database_filename);
void LoadStreetAPI(std::string no);

// loadMap will be called with the name of the file that stores the "layer-2"
// map data accessed through StreetsDatabaseAPI: the street and intersection
// data that is higher-level than the raw OSM data).
// This file name will always end in ".streets.bin" and you
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 (StreetsDatabase) API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the
// name of the ".osm.bin" file that matches your map -- just change
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.

    bool StreetsAPI;
    bool OSM;

bool loadMap(std::string map_streets_database_filename){
    
     //auto const startTime1 = std::chrono::high_resolution_clock::now();

    const std::string no = map_streets_database_filename.substr(0, map_streets_database_filename.find(".streets.bin")) + ".osm.bin";
    thread t1(OSMLoad,no);
    thread t2(LoadStreetAPI, map_streets_database_filename);
    t2.join();
    t1.join();
    bool load_successful = OSM && StreetsAPI; // Indicates whether the map has loaded                                //successfully
    std::cout << "loadMap: " << map_streets_database_filename << std::endl;
    if (!load_successful)
        return load_successful;

    ////////// USED TO DEFINE THE X-Y COORDINATE SYSTEM FOR THE MAP
    ////////// THIS IS CONSTANT FOR EVERY MAP


    ///////////////////////////////////////////////////////
    ///// MILESTONE 1 DATASTRUCTURES
    
    
    // puts the OSMID and LatLon of an OSMNode into an unordered map (OSMID being the key)
    // for quick and easy searching using OSMID


    // travel time for a segment at speed limit
    // this loop iterates for all street segments so that when the function is called
    //the return speed is high
        
    for (StreetSegmentIdx segId = 0; segId < getNumStreetSegments(); ++segId) { //3
        double time = findStreetSegmentLength(segId) / getStreetSegmentInfo(segId).speedLimit; // finds the travel time for all segments
        dataStructs.travelLength.push_back(time);
    }



        
    //parse all intersections
    for (IntersectionIdx inter_key = 0; inter_key < getNumIntersections(); ++inter_key) {

        std::vector<IntersectionIdx> intersections; //declares a new vector for every intersection cycle

        //nested for loop pushes back all streetsegmentIDs into vector "intersections"
        for (int con_seg = 0; con_seg < getNumIntersectionStreetSegment(inter_key); ++con_seg) {

            intersections.push_back(getIntersectionStreetSegment(con_seg, inter_key));

        }
        //push back the vector "intersections" for every cycle into vector of vectors
        dataStructs.vector_inter.push_back(intersections);

    }
    //iterator declared to search for repeated intersectionIDs

   
    
    //auto const endTime3 = std::chrono::high_resolution_clock::now();
    //auto const elapsedTime3 = std::chrono::duration_cast<std::chrono::duration<double>> (endTime3-startTime3);
    //std::cout<< "subway" << "," << elapsedTime3.count() << std::endl;s

    
    
  
    
    
    // Make sure this is updated to reflect whether
    // loading the map succeeded or failed
    
    zoomPin=False;
    return load_successful;
}


// Returns the length of a given street in meters
// Speed Requirement --> high

double findStreetLength(StreetIdx street_id) {
    double sum = 0;

    std::pair <std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> match;
    //Creates a variable to use as an iterator so that you do not search the whole multimap
    match = dataStructs.streetLength.equal_range(street_id);
    for (std::multimap<int, int>::iterator it = match.first; it != match.second; ++it) {
        sum = findStreetSegmentLength(it->second) + sum;
    }

    return sum;
}

// Return all intersection ids at which the two given streets intersect
// This function will typically return one intersection id for streets
// that intersect and a length 0 vector for streets that do not. For unusual
// curved streets it is possible to have more than one intersection at which
// two streets cross.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high

std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids) {

    //creation of two vectors that have all the intersections of a given street
    std::vector<IntersectionIdx> intersection1 = findIntersectionsOfStreet(street_ids.first);
    std::vector<IntersectionIdx> intersection2 = findIntersectionsOfStreet(street_ids.second);
    std::vector<IntersectionIdx> commonIntersections;

    std::sort(intersection1.begin(), intersection1.end());
    std::sort(intersection2.begin(), intersection2.end());

    //Used a set to ensure all unique ids, then use of intersection 
    //to find duplicates and add those duplicates to a vector
    //https://stackoverflow.com/questions/19483663/vector-intersection-in-c for intersection

    std::set_intersection(intersection1.begin(), intersection1.end(),
            intersection2.begin(), intersection2.end(), std::back_inserter(commonIntersections));
    return commonIntersections;
}

// Returns all street ids corresponding to street names that start with the
// given prefix
// The function should be case-insensitive to the street prefix.
// The function should ignore spaces.
//  For example, both "bloor " and "BloOrst" are prefixes to
// "Bloor Street East".
// If no street names match the given prefix, this routine returns an empty
// (length 0) vector.
// You can choose what to return if the street prefix passed in is an empty
// (length 0) string, but your program must not crash if street_prefix is a
// length 0 string.
// Speed Requirement --> high

std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix) {

    //Same function used to remove spaces and transform all char to lower case
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::tolower);
    street_prefix.erase(std::remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::vector<StreetIdx> matchedIds;

    //Edge case of empty prefix
    if (street_prefix == " " || street_prefix == "") {
        return matchedIds;
    }

    //For loop that uses the matching prefix string to the same one in the map and uses lower bound 
    //to go through a smaller segment    
    for (std::multimap<std::string, int>::iterator it = dataStructs.namesOfStreets.lower_bound(street_prefix);
            it != dataStructs.namesOfStreets.end(); it++) {

        //If statement to compare strings and prefixes
        if (it->first.compare(0, street_prefix.size(), street_prefix) == 0) {
            matchedIds.push_back(it->second);
        } else
            break;
    }

    return matchedIds;
}

// Returns the distance between two (lattitude,longitude) coordinates in meters
// Speed Requirement --> moderate

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points) {

    LatLon firstPos = points.first;
    LatLon secondPos = points.second;

    double avglat = kDegreeToRadian * (secondPos.latitude() + firstPos.latitude()) / 2;

    //conversion from latitude and longitude into x and y coordinates for the two different positions
    double y1 = firstPos.latitude() * kDegreeToRadian * kEarthRadiusInMeters;
    double y2 = secondPos.latitude() * kDegreeToRadian * kEarthRadiusInMeters;
    double x1 = kEarthRadiusInMeters * firstPos.longitude() * kDegreeToRadian * cos(avglat);
    double x2 = kEarthRadiusInMeters * secondPos.longitude() * kDegreeToRadian * cos(avglat);

    //using the formula given in handout to calculate the distance
    double distance = sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));
    return distance;
}

// Returns the length of the given street segment in meters
// Speed Requirement --> moderate

double findStreetSegmentLength(StreetSegmentIdx street_segment_id) {
    StreetSegmentInfo segment_info = getStreetSegmentInfo(street_segment_id);
    double curveDistance = 0;

    // creating a pair for the positions so that we cause use the function
    //findDistanceBetweenTwoPoints easily
    std::pair<LatLon, LatLon> position;

    if (segment_info.numCurvePoints == 0) { // the case for a straight line between the intersections

        position.first = getIntersectionPosition(segment_info.from);
        position.second = getIntersectionPosition(segment_info.to);

        return findDistanceBetweenTwoPoints(position);
    } else { //if there are curve points, then:
        // running the loop for all curve points        
        for (int curvePtNum = 0; curvePtNum < getStreetSegmentInfo(street_segment_id).numCurvePoints;
                ++curvePtNum) {

            // variables definitions for the ease of use later
            LatLon curveLoc = getStreetSegmentCurvePoint(curvePtNum, street_segment_id);
            LatLon intersectionPosFrom = getIntersectionPosition(getStreetSegmentInfo(street_segment_id).from);
            LatLon intersectionPosTo = getIntersectionPosition(getStreetSegmentInfo(street_segment_id).to);

            // if there is only one curve point, then take just the distance from the intersection's from/to location         
            if (curvePtNum == 0) {
                position.first = intersectionPosFrom;
                position.second = curveLoc;
                curveDistance = findDistanceBetweenTwoPoints(position);

                if (getStreetSegmentInfo(street_segment_id).numCurvePoints == 1) {
                    position.first = curveLoc;
                    position.second = intersectionPosTo;
                    curveDistance += findDistanceBetweenTwoPoints(position);
                }
            }

            if (curvePtNum > 0) { // if there are more than 1 curve points
                //previous curve point
                position.first = getStreetSegmentCurvePoint((curvePtNum - 1), street_segment_id);
                position.second = curveLoc;
                curveDistance += findDistanceBetweenTwoPoints(position);

                // for the last curve point, use the To intersection
                if (curvePtNum == getStreetSegmentInfo(street_segment_id).numCurvePoints - 1) {
                    position.second = intersectionPosTo;
                    position.first = curveLoc;
                    curveDistance += findDistanceBetweenTwoPoints(position);
                }
            }
        }
        return curveDistance;
    }
}


// Returns the travel time to drive from one end of a street segment
// to the other, in seconds, when driving at the speed limit
// Note: (time = distance/speed_limit)

// Speed Requirement --> high 

double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id) {

    // directly return the travel time for the specified street segment
    // check data structure in load map for more info
    return dataStructs.travelLength[street_segment_id];

}

// Returns true if the two intersections are directly connected, meaning you can legally
// drive from the first intersection to the second using only one streetSegment.
// Speed Requirement --> moderate 

//129, 128
//3744, 3743
//11695, 62146

bool intersectionsAreDirectlyConnected(std::pair<IntersectionIdx, IntersectionIdx> intersection_ids) {

    //the following vectors are created for the ease of finding the street segment that  
    // is the same between two segments. this will tell us whether or not a streetsegment exists
    //between the two intersections, and return what's required

    std::vector<StreetSegmentIdx> firstInter; // creating the vector for the first intersection

    std::vector<StreetSegmentIdx> secondInter; //creating the second vector for the second intersection

    // storing all the street segments of the first intersection
    firstInter = dataStructs.vector_inter[intersection_ids.first];

    // storing the street segments of the second intersection
    secondInter = dataStructs.vector_inter[intersection_ids.second];

    //Segcount=number of segments of an intersection
    //running through the first intersection's street segments
    for (int segCount = 0; segCount < firstInter.size(); ++segCount) {

        //running through the second intersection's street segments
        for (int segCount2 = 0; segCount2 < secondInter.size(); ++segCount2) {

            // if any of the street segments from the first intersection match the second intersection
            // then the intersections are connected
            if (firstInter[segCount] == secondInter[segCount2]) {
                if (getStreetSegmentInfo(firstInter[segCount]).oneWay == true && getStreetSegmentInfo(firstInter[segCount]).from == intersection_ids.first) {
                    return true;
                } else if (getStreetSegmentInfo(firstInter[segCount]).oneWay == false) {
                    return true;
                }

            }

        }
    }
    return false;
}

// Returns the geographically nearest intersection (i.e. as the crow flies) to the given position
// Speed Requirement --> none

IntersectionIdx findClosestIntersection(LatLon my_position) {

    std::pair<LatLon, LatLon> position; //Declaring pair of positions
    position.first = my_position; //first element is permanently set to user's position

    //closest intersection initialized to zero (will indefinitely change before return)
    IntersectionIdx closest_inter = 0;

    double minimum = kEarthRadiusInMeters; //minimum initialized to a very large value 

    //parsing all intersections
    for (IntersectionIdx secondPos = 0; secondPos < getNumIntersections(); ++secondPos) {
        //get position (LatLon) of the intersection under check in for loop and set to second element of pair "position"        
        position.second = getIntersectionPosition(secondPos);

        //calls function that calculates distance between both elements of pair  
        if (findDistanceBetweenTwoPoints(position) < minimum) {
            // if minimum distance is smaller than the initial minimum, "minimum" is reinstated to current distance measured          
            minimum = findDistanceBetweenTwoPoints(position);
            closest_inter = secondPos; //stored the intersectionID of the closest intersection

        }
    }
    return closest_inter;
}

// Returns the street segments that connect to the given intersection
// Speed Requirement --> high


std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id) {
    return dataStructs.vector_inter[intersection_id]; //returns vector for intersection_id from vector of vectors
}

// Returns the street names at the given intersection (includes duplicate
// street names in the returned vector)
// Speed Requirement --> high

std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id) {
    std::vector<std::string> inter_segment_names; // intersecting segment names

    //Conseg=connected segment
    for (StreetSegmentIdx con_seg = 0; con_seg < getNumIntersectionStreetSegment(intersection_id); ++con_seg) {
        inter_segment_names.push_back(getStreetName((getStreetSegmentInfo(getIntersectionStreetSegment
                (con_seg, intersection_id)).streetID)));
    }
    return inter_segment_names;
}

// Returns all intersections along the a given street.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high

//did not remove redundant results, 3 copied results were showing (8348, 11055)

std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id) {

    //accesses map of vectors and returns the node with key streetID with all its intersections     
    std::vector<IntersectionIdx> inter_of_street = dataStructs.streetMap[street_id];
    sort(inter_of_street.begin(), inter_of_street.end());
    inter_of_street.erase(unique(inter_of_street.begin(), inter_of_street.end()), inter_of_street.end());
    return inter_of_street;
}

// Returns the nearest point of interest of the given type to the given position
// Speed Requirement --> none

POIIdx findClosestPOI(LatLon my_position, std::string POIname) {
    POIIdx poi = -1; //returns -1 if no such POI type in map
    float closest;
    bool first = true;


    for (int numPoints = 0; numPoints < getNumPointsOfInterest(); numPoints++) {
        if (getPOIType(numPoints) == POIname) {

            float distance = findDistanceBetweenTwoPoints({my_position, getPOIPosition(numPoints)});

            //loads the first POI location

            if (first) {
                closest = distance;
                first = false;
                poi = numPoints;
            }//compares the closest position so far with new position
            else if (distance < closest) {
                closest = distance;
                poi = numPoints;
            }
        }
    }
    return poi;
}

// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate

double findFeatureArea(FeatureIdx feature_id) {
    double x = 0.0;
    double y = 0.0;
    double area = 0.0;


    if (getFeaturePoint(0, feature_id).longitude() != getFeaturePoint(getNumFeaturePoints(feature_id) - 1, feature_id).longitude()
            || getFeaturePoint(0, feature_id).latitude() != getFeaturePoint(getNumFeaturePoints(feature_id) - 1, feature_id).latitude())
        return area; //if featured area is not a closed area, return 0.


    for (int numPoint = 1; numPoint < getNumFeaturePoints(feature_id); numPoint++) {

        x = kEarthRadiusInMeters * kDegreeToRadian * cos(kDegreeToRadian * (getFeaturePoint(numPoint, feature_id).latitude()
                + getFeaturePoint(numPoint - 1, feature_id).latitude()) / 2) * (getFeaturePoint(numPoint, feature_id).longitude() +
                getFeaturePoint(numPoint - 1, feature_id).longitude()); //get the difference in x position of the area

        y = kEarthRadiusInMeters * kDegreeToRadian * (getFeaturePoint(numPoint, feature_id).latitude()
                - getFeaturePoint(numPoint - 1, feature_id).latitude()); //get the difference in y position of the area
        area += y * x / 2.0; //add to the total area of the feature_id
    }
    return fabs(area);
}

// Return the LatLon of an OSM node; will only be called with OSM nodes (not ways or relations)
// Speed requirement --> high

LatLon findLatLonOfOSMNode(OSMID OSMid) {
    return dataStructs.latLon[OSMid];
}

std::vector<IntersectionIdx> getIntersectionNames(StreetIdx id){

    //std::vector<std::string> inter_names;
    std::vector<IntersectionIdx> inter_names;
    std::vector<IntersectionIdx> intersections;

    std::set <StreetIdx> intersecting_streets;
    intersections = dataStructs.streetMap[id];
    
    for (IntersectionIdx inter = 0; inter < intersections.size(); ++inter){
        //Iterate through segments of intersection
        //Store street Idx of segment into intersecting_streets
        //Make sure to not store the id streetIdx
        //if(it != id) intersecting_streets.insert(it);
        IntersectionIdx inter_id = intersections[inter];
        inter_names.push_back((inter_id));
    }
    //Iterate through set and store each term into vector
    
    //At some point, probably in another function, create formatted strings 
    //To store in liststore
    
    //I.e Bay St. + College St
    // getStreetName(id) + " +

    return inter_names;

}


void closeMap() {
    // Clean-up your map related data structures here
    dataStructs.namesOfStreets.clear();
    dataStructs.streetLength.clear();
    dataStructs.latLon.clear();
    dataStructs.vector_inter.clear();
    dataStructs.streetMap.clear();
    dataStructs.streetMap2.clear();
    dataStructs.travelLength.clear();
    dataStructs.majorRoadType.clear();
    dataStructs.typePOI.clear();
    dataStructs.featureMap.clear();
    dataStructs.streetMap2.clear();
    dataStructs.segMap.clear();
    dataStructs.subwayInfo.clear();
    
    //intersections_plot.clear();

    closeOSMDatabase();
    closeStreetDatabase();
    
    

    //featureInfo.all_points.clear();
 


}




void LoadStreetAPI(std::string map_streets_database_filename){
    StreetsAPI = loadStreetsDatabaseBIN(map_streets_database_filename);
    
    double maxLat = -DBL_MAX;
    double minLat = DBL_MAX;
    double maxLon = -DBL_MAX;
    double minLon = DBL_MAX;
    for (int i = 0; i < getNumIntersections(); i++) { //1
        if (getIntersectionPosition(i).latitude() < minLat)
            minLat = getIntersectionPosition(i).latitude();
        if (getIntersectionPosition(i).latitude() > maxLat)
            maxLat = getIntersectionPosition(i).latitude();
        if (getIntersectionPosition(i).longitude() < minLon)
            minLon = getIntersectionPosition(i).longitude();
        if (getIntersectionPosition(i).longitude() > maxLon)
            maxLon = getIntersectionPosition(i).longitude();
    }

    latAverage = (minLat + maxLat) / 2;
    xMin = getXfromLon(minLon);
    xMax = getXfromLon(maxLon);
    yMin = getYfromLat(minLat);
    yMax = getYfromLat(maxLat);
    
    for (StreetSegmentIdx segId = 0; segId < getNumStreetSegments(); ++segId) { //3
        double time = findStreetSegmentLength(segId) / getStreetSegmentInfo(segId).speedLimit; // finds the travel time for all segments
        dataStructs.travelLength.push_back(time);
    }

    for (IntersectionIdx inter_key = 0; inter_key < getNumIntersections(); ++inter_key) {

        std::vector<IntersectionIdx> intersections; //declares a new vector for every intersection cycle

        //nested for loop pushes back all streetsegmentIDs into vector "intersections"
        for (int con_seg = 0; con_seg < getNumIntersectionStreetSegment(inter_key); ++con_seg) {

            intersections.push_back(getIntersectionStreetSegment(con_seg, inter_key));

        }
        //push back the vector "intersections" for every cycle into vector of vectors
        dataStructs.vector_inter.push_back(intersections);

    }
    //iterator declared to search for repeated intersectionIDs

   

   // auto const startTime3 = std::chrono::high_resolution_clock::now();
    
    for (StreetSegmentIdx id = 0; id < getNumStreetSegments(); ++id){
   
    dataStructs.streetMap2.insert(std::make_pair(getStreetSegmentInfo(id).streetID, getStreetSegmentInfo(id).from));
    
    dataStructs.streetMap2.insert(std::make_pair(getStreetSegmentInfo(id).streetID, getStreetSegmentInfo(id).to));
    //std::cout<<"1"<<std::endl;

    }
    
    for (StreetIdx id = 0; id < getNumStreets(); ++id) {

        dataStructs.streetMap.resize(getNumStreets());
        auto range = dataStructs.streetMap2.equal_range(id);



        for (auto it = range.first; it != range.second; ++it) {
            (dataStructs.streetMap[id]).push_back(it->second);
        }

        sort(dataStructs.streetMap[id].begin(), dataStructs.streetMap[id].end());

        dataStructs.streetMap[id].erase(unique(dataStructs.streetMap[id].begin(), dataStructs.streetMap[id].end()), dataStructs.streetMap[id].end());

    }    


        for (StreetSegmentIdx id = 0; id < getNumStreetSegments(); ++id) {
        
        // Insert the from/to intersections with the key being street id
        // into the multimap
        dataStructs.streetMap2.insert(std::make_pair(getStreetSegmentInfo(id).streetID, getStreetSegmentInfo(id).from));
        dataStructs.streetMap2.insert(std::make_pair(getStreetSegmentInfo(id).streetID, getStreetSegmentInfo(id).to));
   
    }

    for (StreetIdx id = 0; id < getNumStreets(); ++id) {
        dataStructs.streetMap.resize(getNumStreets());
        auto range = dataStructs.streetMap2.equal_range(id);

   
        // this for loop points to the first element of the respective key of the 
        // map
        for (auto it = range.first; it != range.second; ++it) {
            // iterating through these elements and storing it into a 
            // vector of vectors
            (dataStructs.streetMap[id]).push_back(it->second);
          
        }
        
        // sorting and removing the duplicate values respectively
        sort(dataStructs.streetMap[id].begin(), dataStructs.streetMap[id].end());
        dataStructs.streetMap[id].erase(unique(dataStructs.streetMap[id].begin(), dataStructs.streetMap[id].end()), dataStructs.streetMap[id].end());

    }
   

    //For loop for the partialPrefixfunction, inserts data into multimap
    for (StreetIdx streetNum = 0; streetNum < getNumStreets(); streetNum++) {
        //Function to lowercase and remove spaces
        std::string copyString = getStreetName(streetNum);
        std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
        copyString.erase(std::remove(copyString.begin(), copyString.end(), ' '), copyString.end());
        dataStructs.namesOfStreets.insert(std::make_pair(copyString, streetNum));
    }


    //For loop for insertion of data into data structure for streetLength
    for (StreetSegmentIdx streetSegNum = 0; streetSegNum < getNumStreetSegments(); streetSegNum++) {
        std::pair <int, int> insertion;
        insertion.first = getStreetSegmentInfo(streetSegNum).streetID;
        insertion.second = streetSegNum;
        dataStructs.streetLength.insert(insertion);
    }
    
        for (int i = 0; i < getNumFeatures(); i++) {
        featureInfo temp; // create temporary structure to place it in the multimap

        double area = findFeatureArea(i);
        temp.id = i;
        temp.type = getFeatureType(i);
        temp.numPoints = getNumFeaturePoints(i);

        if (area > 0) {// if it is a closed feature and not a line
            for (int k = 0; k < temp.numPoints; k++) {
                // push individual points into a vector
                temp.all_points.push_back(
                        ezgl::point2d(
                        getXfromLatLon(getFeaturePoint(k, i), latAverage),
                        getYfromLatLon(getFeaturePoint(k, i))
                        )
                        );
            }
        }
        
        // store the feature information into a map with the key being area
        std::pair<double, struct featureInfo> insertFeature = {area, temp};
        dataStructs.featureMap.insert(insertFeature);
    }
    
    
    
    
    //best_time_tracker.resize(getNumIntersections());
    


    /////////////////////////////////// M3 DATASTRUCTURES ///////
    ///////////////////////////////////
    ///////////////////////////////////
    
    for (int i = 0; i< getNumStreetSegments(); i++){
        streetData temp;
        temp.streetSegId = i;
        double x1,x2,y1,y2;
       
          ////////// FOR ORGANIZATION PURPOSES
        // defining the start and end of segments 
        // so that it is easier to implement in the later functions
        StreetSegmentInfo segInfo = getStreetSegmentInfo(i); 
        
        
        
        // intersection from/to positions
        LatLon from_position = getIntersectionPosition(segInfo.from);
        LatLon to_position = getIntersectionPosition(segInfo.to);
        
        temp.start_segment = ezgl::point2d(getXfromLatLon(from_position, latAverage), getYfromLatLon(from_position));
        temp.end_segment = ezgl::point2d(getXfromLatLon(to_position, latAverage), getYfromLatLon(to_position));
        x1 = temp.start_segment.x;
        x2 = temp.end_segment.x;
        y1 = temp.start_segment.y;
        y2 = temp.end_segment.y;
        
        if(x2-x1 != 0)
                temp.angle = (180/PI) * atan((y2- y1)/ (x2-x1+ep));
        
        if (segInfo.numCurvePoints > 0) {

            
            
            //drawing from the from intersection to the first curve points
            temp.end_firstCurve=ezgl::point2d(getXfromLatLon(getStreetSegmentCurvePoint(0, i), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(0, i)));
            x1 = temp.start_segment.x;
            x2 = temp.end_firstCurve.x;
            y1 = temp.start_segment.y;
            y2 = temp.end_firstCurve.y;
            
            if(x2-x1 != 0)
                temp.firstCurveAngle = (180/PI) * atan((y2- y1)/ (x2-x1+ep));
           

            //drawing from the first to the last curve point
            temp.curvePoints_start.resize(segInfo.numCurvePoints -1);
            temp.curvePoints_end.resize(segInfo.numCurvePoints -1);
            temp.curveAngle.resize(segInfo.numCurvePoints -1);
            
            for (int j = 0; j < segInfo.numCurvePoints - 1; j++) {
                ezgl::point2d start(getXfromLatLon(getStreetSegmentCurvePoint(j, i), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(j, i)));
                ezgl::point2d end(getXfromLatLon(getStreetSegmentCurvePoint(j + 1, i), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(j + 1, i)));

                //check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);
                //temp.curvePoints_start.resize(1);
                temp.curvePoints_start[j] = start;
                temp.curvePoints_end[j] = end;
                
                x1 = start.x;
                x2 = end.x;
                y1 = start.y;
                y2 = end.y;
                
                temp.curveAngle[j]= ( (180/PI) * atan((y2- y1)/ (x2-x1+ep)));
                
                //g->draw_line(start, end);

//                if (getStreetName(segInfo.streetID) != "<unknown>") {
//                    //text_roads(g, start.x, start.y, end.x, end.y, segInfo);
//
//
//                }
            }

            //drawing from the last curve point to the to intersection
            //check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);
            temp.start_lastCurve = ezgl::point2d(getXfromLatLon(getStreetSegmentCurvePoint(segInfo.numCurvePoints - 1, i), latAverage), 
                    getYfromLatLon(getStreetSegmentCurvePoint(segInfo.numCurvePoints - 1, i)));
            
            x1 = temp.start_lastCurve.x;
            x2 = temp.end_segment.x;
            y1 = temp.start_lastCurve.y;
            y2 = temp.end_segment.y;
            
            if(x2-x1 != 0)
                temp.lastCurveAngle = (180/PI) * atan((y2- y1)/ (x2-x1+ep));
                    
        }
        std::pair<StreetSegmentIdx, struct streetData> insertSegInfo = {i, temp};
        dataStructs.segMap.insert(insertSegInfo);
    }
    
}

void OSMLoad(std::string no){
    OSM = loadOSMDatabaseBIN(no);
    for (int nodeNum = 0; nodeNum < getNumberOfNodes(); nodeNum++) { //2
        const OSMNode *node = getNodeByIndex(nodeNum);
        dataStructs.indexNode[node->id()] = node;
        
        dataStructs.latLon[node->id()] = getNodeCoords(node);
    }
    
auto const startTime3 = std::chrono::high_resolution_clock::now();
    vector<const OSMRelation *> osm_subway_lines;
    
    
    // Loop through all OSM relations
    for (unsigned i = 0; i < getNumberOfRelations(); i++) {
        const OSMRelation *currRel = getRelationByIndex(i);

        // Check the tag of the currRel
        for (unsigned j = 0; j < getTagCount(currRel); j++) {
            pair<string, string> tagPair = getTagPair(currRel, j);

            // Push relations with the route=subway tag
            if (tagPair.first == "route" && tagPair.second == "subway") {
                osm_subway_lines.push_back(currRel);
                break;
            }
        }
    }
    auto const endTime3 = std::chrono::high_resolution_clock::now();
    auto const elapsedTime3 = std::chrono::duration_cast<std::chrono::duration<double>> (endTime3-startTime3);
    //std::cout<< "subway" << "," << elapsedTime3.count() << std::endl;

    
    // For each subway line (relation), get its name, color, and members
    for (unsigned i = 0; i < osm_subway_lines.size(); i++) {
        std::pair < std::string, std::vector < ezgl::point2d>> tempSubwayPair;

        // Get subway line color and name
        for (unsigned j = 0; j < getTagCount(osm_subway_lines[i]); j++) {
            pair<string, string> tagPair = getTagPair(osm_subway_lines[i], j);

            if (tagPair.first == "colour") {

                //cout << "Subway line color: " << tagPair.second << endl;
                tempSubwayPair.first = tagPair.second;
            } else if (tagPair.first == "name") {
                //cout << "Subway line name: " << tagPair.second << endl;
            }
        }

        // Get relation members
        vector<TypedOSMID> route_members = getRelationMembers(osm_subway_lines[i]);

        // Print subway station names
        //cout << "Subway line stations:" << endl;
        auto const startTime4 = std::chrono::high_resolution_clock::now();
        for (unsigned j = 0; j < route_members.size(); j++) {

            // A member of type node represents a subway station
            if (route_members[j].type() == TypedOSMID::Node) {

                const OSMNode *currNode = dataStructs.indexNode[route_members[j]];

                // Node lookup by OSMIDosm_subway_lines
//                for (unsigned k = 0; k < getNumberOfNodes(); k++) {
//                    currNode = getNodeByIndex(k);
//                    if (currNode->id() == route_members[j])
//                        break;
//                }
                
                // Get the name tag of that node
                for (unsigned k = 0; k < getTagCount(currNode); k++) {
                    pair<string, string> tagPair = getTagPair(currNode, k);

                    if (tagPair.first == "name") {


                        tempSubwayPair.second.push_back(
                                ezgl::point2d(
                                getXfromLatLon(getNodeCoords(currNode), latAverage),
                                getYfromLatLon(getNodeCoords(currNode))
                                )

                                );

                        //cout << "\t" << tagPair.second << endl;
                        break;
                    }
                }


            }

        }
        auto const endTime4 = std::chrono::high_resolution_clock::now();
    auto const elapsedTime4 = std::chrono::duration_cast<std::chrono::duration<double>> (endTime4-startTime4);
    //std::cout<< "subway2" << "," << elapsedTime4.count() << std::endl;
        dataStructs.subwayInfo.push_back(tempSubwayPair);
    }
    
    
/////////////////////////////////////////////////////
    /////////////////////////////////////////////// M2 DATASTRUCTURES

    // Used for distinguishing between major and minor roads
    for (int way = 0; way < getNumberOfWays(); ++way) {
        const OSMWay *wayIndex = getWayByIndex(way);
        
        // using tag count and way relations to see which streets 
        // are highways and secondary streets
        for (int j = 0; j < getTagCount(wayIndex); j++) {
            std::string key, value;
            std::tie(key, value) = getTagPair(wayIndex, j);

            if (key == "highway" && value == "motorway") {
                dataStructs.majorRoadType[wayIndex->id()] = "motorway";
            }

            if (key == "highway" && value == "secondary") {

                dataStructs.majorRoadType[wayIndex->id()] = "secondary";
            }
        }

    }

    

    
    
    //// SETTINGS POIs INTO CATEGORIES TO DISPLAY ON THE MAP WITH THE 
    //// RESPECTIVE ICON. 
    for (int poi = 0; poi < getNumberOfNodes(); poi++) {
        const OSMNode *poi_osm = getNodeByIndex(poi);
        
        // using the osmnodes, i am finding the poi categories and using those to
        // display their icons. the value is the type of category and then i give
        // them a category name so that we can call them easily
        for (int j = 0; j < getTagCount(poi_osm); j++) {
            std::string key, value;
            std::tie(key, value) = getTagPair(poi_osm, j);
            
            // restaurants
            if (key == "amenity" && (value == "fast_food" || value == "restaurant"
                    || value == "cafe" || value == "ice_cream"|| value == "food_court")) {
                dataStructs.typePOI[poi_osm->id()] = "restaurant";
            }
            //bars
            if (key == "amenity" && (value == "bar" || value == "biergarten"
                    || value == "pub" || value == "ice_cream"|| value == "food_court")) {
                dataStructs.typePOI[poi_osm->id()] = "bar";
            }
            // education buildings
            if (key == "amenity" && (value == "college" || value == "kindergarten"
                    || value == "language_school" || value == "library"|| value == "school"
                    || value == "music_school" || value == "university")) {
                dataStructs.typePOI[poi_osm->id()] = "education";
            }
            //financial buildings
            if (key == "amenity" && (value == "atm" || value == "bank"
                    || value == "bureau_de_change")) {
                dataStructs.typePOI[poi_osm->id()] = "financial";
            }
            // bus stops
            if (key == "amenity" && (value == "bus_station")) {
                dataStructs.typePOI[poi_osm->id()] = "bus_station";
            }
            // fuel stations
            if (key == "amenity" && (value == "fuel")) {
                dataStructs.typePOI[poi_osm->id()] = "fuel";
            }
            // all different healthcare places
            if (key == "amenity" && (value == "clinic" || value == "dentist"
                    || value == "doctors" || value == "hospital"|| value == "pharmacy" 
                    || value == "veterinary")) {
                dataStructs.typePOI[poi_osm->id()] = "hospital";
            }
            
            if (key == "amenity" && (value == "casino" || value == "cinema"
                    || value == "nightclub" || value == "stripclub"|| value == "theatre"
                    || value == "community_centre" || value == "arts_centre")) {
                dataStructs.typePOI[poi_osm->id()] = "entertainment";
            }
          
        }

    }    
    
    
}