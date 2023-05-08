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
#include <string>
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"

//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage


//The default map to load if none is specified
std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";


//std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
//std::string mapPath = "/cad2/ece297s/public/maps/" + mapName + ".streets.bin";



// The start routine of your program (main) when you are running your standalone
// mapper program. This main routine is *never called* when you are running 
// ece297exercise (the unit tests) -- those tests have their own main routine
// and directly call your functions in /libstreetmap/src/ to test them.
// Don't write any code in this file that you want run by ece297exerise -- it 
// will not be called!
//hello
int main(int argc, char** argv) {

    std::string map_path;
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }

    //Load the map and related data structures
    bool load_success = loadMap(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return ERROR_EXIT_CODE;
    }
    
//    std::vector<DeliveryInf> deliveries;
//    std::vector<IntersectionIdx> depots;
//    float turn_penalty;
//        deliveries = {DeliveryInf(77883, 182478), DeliveryInf(40066, 130962), DeliveryInf(183624, 151123), DeliveryInf(232423, 125207), DeliveryInf(126867, 42596), DeliveryInf(32577, 193387), DeliveryInf(32577, 9834), DeliveryInf(62452, 140541), DeliveryInf(166763, 228862), DeliveryInf(186169, 96318), DeliveryInf(235506, 8164), DeliveryInf(161266, 149477), DeliveryInf(198493, 164674), DeliveryInf(92375, 183196), DeliveryInf(54910, 158428), DeliveryInf(230206, 193387), DeliveryInf(195233, 236594), DeliveryInf(232423, 40167), DeliveryInf(78024, 79228), DeliveryInf(180268, 95978), DeliveryInf(32577, 125207), DeliveryInf(29207, 93454), DeliveryInf(118435, 129169), DeliveryInf(118435, 50800), DeliveryInf(126867, 7447), DeliveryInf(211781, 41906), DeliveryInf(114643, 173), DeliveryInf(118435, 193387), DeliveryInf(45123, 97124), DeliveryInf(120109, 239755), DeliveryInf(125792, 228862), DeliveryInf(126867, 14819), DeliveryInf(181226, 163848), DeliveryInf(32577, 151123), DeliveryInf(3204, 151123), DeliveryInf(169706, 164674), DeliveryInf(126867, 162194), DeliveryInf(44805, 222719), DeliveryInf(118435, 151123), DeliveryInf(112295, 76333), DeliveryInf(52038, 181241), DeliveryInf(32577, 193387), DeliveryInf(118435, 66462), DeliveryInf(32577, 227112), DeliveryInf(118435, 154963), DeliveryInf(45123, 125207), DeliveryInf(118435, 144188), DeliveryInf(32577, 45270), DeliveryInf(179665, 224184), DeliveryInf(77749, 125207), DeliveryInf(22970, 166401), DeliveryInf(126867, 130816), DeliveryInf(32577, 151123), DeliveryInf(45123, 143068), DeliveryInf(116820, 28991), DeliveryInf(32577, 224184), DeliveryInf(48044, 224184), DeliveryInf(126867, 228862), DeliveryInf(86342, 73151), DeliveryInf(126867, 224184), DeliveryInf(126867, 172006), DeliveryInf(86826, 66462), DeliveryInf(100119, 223662), DeliveryInf(118435, 66462), DeliveryInf(214718, 42462)};
//        depots = {44};
//        turn_penalty = 30.000000000;
//     
//    travelingCourier(turn_penalty, deliveries, depots);
   // std::vector<IntersectionIdx>test;
  //  test = findIntersectionsOfStreet(11055);
    
//    int size = test.size();
    
//    for (int i = 0; i < size;++i){
       // std::cout << test[i] << "\n" <<std::endl;
 //  }

    

//    std::pair<int,int> bruh= {129, 128};
//    //3744, 3743
////11695, 62146
//    std::pair<int,int> bruh1= {3744, 3743};
//    std::pair<int,int> bruh2= {11695, 62146};
    

//    drawMap();
    std::cout << "Successfully loaded map '" << map_path << "'\n";
    //You can now do something with the map data
    
    std::vector<DeliveryInf> deliveries;
    std::vector<IntersectionIdx> depots;
    std::vector<CourierSubPath> result_path;
    float turn_penalty;
        deliveries = {DeliveryInf(122474, 97031), DeliveryInf(71346, 53400), DeliveryInf(50160, 73642), DeliveryInf(122474, 53400), DeliveryInf(52922, 99578), DeliveryInf(122474, 53400), DeliveryInf(122474, 20263), DeliveryInf(71346, 73642), DeliveryInf(119721, 73642)};
        depots = {11072, 5162, 100304};
        turn_penalty = 30.000000000;
        result_path = travelingCourier(turn_penalty, deliveries, depots);
    
    //Clean-up the map data and related data structures
    std::cout << "Closing map\n";
   
    //closeMap(); 

    return SUCCESS_EXIT_CODE;
}
