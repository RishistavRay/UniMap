
/* 
 * File:   m2.cpp
 * Author: elangov8
 *
 * Created on March 2, 2022, 9:29 AM
 */

#include <queue>
#include<algorithm>

#include "m1.cpp"
#include <unordered_set>



typedef cairo_surface_t surface;


#define PI 3.14159265


void drawMap() {

    //settings for the 
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    ezgl::application application(settings);
    initial_world = ezgl::rectangle(ezgl::point2d(xMin, yMin), ezgl::point2d(xMax, yMax));
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);

    create_vector_inter();
    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);

    // FREE ALL THE IMAGES AFTER THE MAP CLOSES
    ezgl::renderer::free_surface(pin);
    ezgl::renderer::free_surface(res);
    ezgl::renderer::free_surface(education);
    ezgl::renderer::free_surface(bar);
    ezgl::renderer::free_surface(financial);
    ezgl::renderer::free_surface(hospital);
    ezgl::renderer::free_surface(bus_station);
    ezgl::renderer::free_surface(fuel);
    ezgl::renderer::free_surface(entertainment);
    
    ezgl::renderer::free_surface(pickup);
    ezgl::renderer::free_surface(uni);
    ezgl::renderer::free_surface(parking);
    ezgl::renderer::free_surface(rendezvous);
    ezgl::renderer::free_surface(Drop);
    ezgl::renderer::free_surface(Restaurant);
    ezgl::renderer::free_surface(cursor);
    ezgl::renderer::free_surface(my_location);
    ezgl::renderer::free_surface(Default_Pin);
    ezgl::renderer::free_surface(finish_pin);
    
}

//setting up all widgets for the screen

void initial_setup(ezgl::application* application, bool /*new window */){
GObject *my_widget = application->get_object("TextInput");
GObject *my_widget1 = application->get_object("mapNames");
GObject *my_widget2 = application->get_object("Dark");
GObject *my_widget3 = application->get_object("fav_new");
GObject *my_widget4 = application->get_object("pin_name");
GObject *my_widget5 = application->get_object("icons");
GObject *my_widget6 = application->get_object("Subway");
GObject *my_widget7 = application->get_object("TextInput2");
GObject *my_widget8 = application->get_object("Navigation");
GObject *my_widget9 = application->get_object("Help");
GObject *my_widget10 = application->get_object("Favorite");
GObject *my_widget11 = application->get_object("displayDirections");
GtkWidget* navBar = GTK_WIDGET(my_widget7);
GtkWidget* pinBar = GTK_WIDGET(my_widget4);
GtkWidget* favBar = GTK_WIDGET(my_widget3);
GtkWidget* iconBar = GTK_WIDGET(my_widget5);
GtkWidget* navWind = GTK_WIDGET(my_widget11);

//Hiding all things based on initial setup
gtk_widget_hide(navBar);
gtk_widget_hide(pinBar);
gtk_widget_hide(favBar);
gtk_widget_hide(iconBar);
gtk_widget_hide(navWind);

g_signal_connect(my_widget, "search-changed", G_CALLBACK(mapSearch), application);
g_signal_connect(my_widget,"activate",G_CALLBACK(mapZoom),application);
g_signal_connect(my_widget2,"clicked",G_CALLBACK(toggleDark),application);
g_signal_connect(my_widget1,"changed",G_CALLBACK(mapChange),application);
g_signal_connect(my_widget3,"changed",G_CALLBACK(favMode),application); //change signal
g_signal_connect(my_widget4,"activate",G_CALLBACK(createPin),application);
g_signal_connect(my_widget6,"clicked",G_CALLBACK(toggleSubway),application);
g_signal_connect(my_widget5,"changed",G_CALLBACK(setIcon),application);
g_signal_connect(my_widget7, "search-changed", G_CALLBACK(mapSearch1), application);
g_signal_connect(my_widget7,"activate",G_CALLBACK(mapZoom1),application);
g_signal_connect(my_widget8,"clicked",G_CALLBACK(toggleNavigation),application);
g_signal_connect(my_widget9,"clicked",G_CALLBACK(toggleHelp),application);
g_signal_connect(my_widget10,"clicked",G_CALLBACK(toggleFav),application);


}


GtkImage* createImage(double angle){
    GtkImage* image = NULL;
    if(angle==280){
        return image;
    }
    return NULL;
}

void instructionsAdding(displayInstr &holdingInfo,GtkListBox* insertBox ){
    
    auto leftBox = (GtkBox*) (gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0));
    GtkWidget* ID = gtk_label_new((holdingInfo.instruction).c_str());
    gtk_box_pack_start(leftBox,ID,true,true,0);

    auto rightBox = (GtkBox*)(gtk_box_new(GTK_ORIENTATION_VERTICAL,0));
    GtkWidget* ID1 = gtk_label_new((holdingInfo.distance).c_str());
    gtk_box_pack_end(rightBox,ID1,true,true,0);
    gtk_box_pack_start(rightBox,(GtkWidget*)createImage(holdingInfo.angle),true,true,0);
    gtk_box_pack_start(leftBox,(GtkWidget*)rightBox,true,true,0);
    
    gtk_list_box_prepend(insertBox,(GtkWidget*) leftBox); 
}

void createListBox(){
    GtkWidget* mainBox = gtk_list_box_new();
    GtkListBox* entryBox = (GtkListBox*) mainBox;
    for (int filler = 0; filler<displayDir.size();filler++){
        instructionsAdding(displayDir[filler], entryBox);
    }
}

void clearScrollWin(ezgl::application *application){
   GObject *my_widget = application->get_object("displayDirections"); 
   GList *it, *childs = gtk_container_get_children(GTK_CONTAINER (my_widget));
   for (it=childs;it!=NULL;it=g_list_next(it)){
       gtk_widget_destroy(GTK_WIDGET(it->data));
   }
   g_list_free(childs);
   displayDir.clear();
}


//To toggle the help window of the map
void toggleHelp(GtkButton* /*help*/,ezgl::application* application){
    helpMode = !helpMode;
    GtkWidget* wid = (GtkWidget*) application -> get_object("Window1");
    if (helpMode) {
        gtk_widget_show(wid);
    } else {
        gtk_widget_hide(wid);
    }
}


//toggling between light and dark mode

void toggleDark(GtkButton* darkMode, ezgl::application* application) {
    //GObject *my_w = application -> get_object("TextInput");
    //GtkWidget* wid = GTK_WIDGET(my_w);
    dark = !dark;
    if (dark) {
        gtk_button_set_label(darkMode, "Light Mode");
        //gtk_widget_hide(wid);
    } else {
        gtk_button_set_label(darkMode, "Dark Mode");
        //gtk_widget_show(wid);
    }

    application->refresh_drawing();
}

void toggleSubway(GtkButton* subwayMode, ezgl::application* application) {
    //GObject *my_w = application -> get_object("TextInput");
    //GtkWidget* wid = GTK_WIDGET(my_w);
    subway = !subway;
    if (subway) {
        gtk_button_set_label(subwayMode, "Subways: ON");
        //gtk_widget_hide(wid);
    } else {
        gtk_button_set_label(subwayMode, "Subways: OFF");
        //gtk_widget_show(wid);
    }

    application->refresh_drawing();
}


void toggleNavigation(GtkButton* nav, ezgl::application* application){
    navMode = !navMode;
    GObject *my_w = application -> get_object("TextInput2");
    GtkWidget* wid = GTK_WIDGET(my_w);
    GObject *my_widget1 = application -> get_object("TextInput");
    GtkEntry* wid1 = GTK_ENTRY(my_widget1);
    GObject *my_widget12 = application -> get_object("displayDirections");
    GtkWidget* wid3 = GTK_WIDGET(my_widget12);
    //GtkImage *image;
    //image = gtk_image_new_from_file ("libstreetmap/resources/hello.png");
    if (navMode) {
        gtk_button_set_label(nav, "Navigation: ON");
        gtk_widget_show(wid);
        gtk_widget_show(wid3);
        gtk_entry_set_placeholder_text(wid1,"From: use & for intersections");
        zoomPinFirst=true;
        zoomPin=true;
        zoomPins=false;
        application->refresh_drawing();
    } else {
        gtk_button_set_label(nav, "Navigation: OFF");
        gtk_entry_set_placeholder_text(wid1,"Search (& for Intersections)");
        gtk_widget_hide(wid);
        gtk_widget_hide(wid3);
        zoomPinFirst=false;
        zoomPin=false;
        application->refresh_drawing();
       // gtk_widget_hide(image);
    }
}


void toggleFav(GtkButton* fav, ezgl::application* application){
    favoriteMode = !favoriteMode;
    GObject *my_w = application -> get_object("pin_name");
    GObject *my_w1 = application -> get_object("icons");
    GObject *my_w2 = application -> get_object("fav_new");
    
    GtkWidget* wid = GTK_WIDGET(my_w);
    GtkWidget* wid1 = GTK_WIDGET(my_w1);
    GtkWidget* wid2 = GTK_WIDGET(my_w2);
    //GtkImage *image;
    //image = gtk_image_new_from_file ("libstreetmap/resources/hello.png");
    if (favoriteMode) {
        gtk_button_set_label(fav, "Favorite: ON");
        gtk_widget_show(wid);
        gtk_widget_show(wid1);
        gtk_widget_show(wid2);
    } else {
        gtk_button_set_label(fav, "Favorite: OFF");
        gtk_widget_hide(wid);
        gtk_widget_hide(wid1);
        gtk_widget_hide(wid2);
    }
}


//Performs the ability to search intersections and streets in one search bar

void mapSearch(GtkSearchEntry* search_bar, gpointer user_data) {

    //auto casting

    auto app = static_cast<ezgl::application*> (user_data);

    //Gets entry from search bar and converts to strings
    GtkEntry* text_entry = GTK_ENTRY(search_bar);

    

    const char* text = gtk_entry_get_text(text_entry);
    std::string prefix(text);

    //Code template for this was found at the link below:
    //https://docs.gtk.org/gtk4/class.ListStore.html
    
    GtkListStore *listPtr1 = (GtkListStore *) app -> get_object("streetName");
    GtkTreeIter iter;
    gtk_list_store_clear(listPtr1);

    std::vector<StreetIdx> matchedIds = findStreetIdsFromPartialStreetName(prefix);



    //What to do if the string contains a & 
    std::size_t found = prefix.find_first_of("&");
    if (found != std::string::npos) {
        //Gets the first street in the entry
        std::string newString = prefix.substr(0, found - 1);
        std::vector<StreetIdx> newIds = findStreetIdsFromPartialStreetName(newString);
        std::vector<IntersectionIdx>interName;
        for (int i = 0; i < newIds.size(); i++) {
            interName = getIntersectionNames(newIds[i]);

            //loads the list store
            for (int j = 0; j < interName.size(); j++) {
                std::string data = getIntersectionName(interName[j]);
                std::string u = stringSwap(data, newString);
                gtk_list_store_append(listPtr1, &iter);
                gtk_list_store_set(listPtr1, &iter, 0, u.c_str(), -1);
            }
        }
        
        for (int i = 0; i < newIds.size(); i++) {
            interName = getIntersectionNames(newIds[i]);
            for (int j = 0; j < interName.size(); j++) {
                inter_section_1 = interName[j];
                std::string t = getIntersectionName(interName[j]);
                std::string l = stringSwap(t,newString);
                if (prefix == l) {
                    int_pos = getIntersectionPosition(interName[j]);
                    select_one = interName[j];
                    break;
                } 
            }
        }
        xptr1 = getXfromLatLon(int_pos, latAverage);
        yptr1 = getYfromLatLon(int_pos);
        if(initialMode){
           zoomPin = true;
           initialMode = false;
           firstBar=false;
        }
        else{zoomPin = false;
        }
        return;
    }
    //If this is only a street
    for (int i = 0; i < matchedIds.size(); i++) {
        std::string data;
        data = getStreetName(matchedIds[i]);
        gtk_list_store_append(listPtr1, &iter);
        gtk_list_store_set(listPtr1, &iter, 0, data.c_str(), -1);
    }
    firstBar=true;

}


void mapZoom(GtkSearchEntry* search_bar, gpointer user_data) {
    auto app = static_cast<ezgl::application*> (user_data);
    if (!navMode) {
        GtkEntry* text_entry = GTK_ENTRY(search_bar);
        const char* text = gtk_entry_get_text(text_entry);
        std::string prefix(text);

        std::vector<StreetIdx> mainId = findStreetIdsFromPartialStreetName(prefix);

        //What to do to zoom to a intersection
        std::size_t found = prefix.find_first_of("&");
        if (found != std::string::npos) {

            std::string newString = prefix.substr(0, found - 1);
            std::vector<StreetIdx> newIds = findStreetIdsFromPartialStreetName(newString);



            if (int_pos == LatLon{0, 0}) {
                app->update_message("Unknown Intersection");
                return;
            }
            //Gets location and places pin
            ezgl::renderer *g = app->get_renderer();

            g->set_visible_world({
                {xptr1 - 200, yptr1 - 200}, 400, 400
            });
            app->update_message("");
            zoomPinMain = true;
            app->refresh_drawing();

            //g->draw_surface(pin,{xptr, yptr}, 0.5);
            return;
        }

        //What to do if useless entry
        if (mainId.size() == 0) {
            firstBar = true;
            return;
        }
        std::vector<IntersectionIdx> street_inter;
        street_inter.resize(findIntersectionsOfStreet(mainId[0]).size());

        street_inter = findIntersectionsOfStreet(mainId[0]);

        //Sets pin for Street
        LatLon street_pos;

        street_pos = getIntersectionPosition(street_inter[0]);

        xptrs = getXfromLatLon(street_pos, latAverage);
        yptrs = getYfromLatLon(street_pos);
        ezgl::renderer *g = app->get_renderer();
        g->set_visible_world({
            {xptrs - 200, yptrs - 200}, 400, 400
        });
        zoomPins = true;
        app->refresh_drawing();
        //g->draw_surface(pin,{xptr, yptr}, 0.5);
        return;
    }
    //std::stringstream ss;
    //ss << "Please leave Navigation mode";
   // app->update_message(ss.str()); //updates message on status bar
}



void mapSearch1(GtkSearchEntry* search_bar, gpointer user_data) {

    //auto casting

    auto app = static_cast<ezgl::application*> (user_data);

    //Gets entry from search bar and converts to strings
    GtkEntry* text_entry = GTK_ENTRY(search_bar);

    

    const char* text = gtk_entry_get_text(text_entry);
    std::string prefix(text);

    //Code template for this was found at the link below:
    //https://docs.gtk.org/gtk4/class.ListStore.html
    
    GtkListStore *listPtr2 = (GtkListStore *) app -> get_object("streetName1");
    GtkTreeIter iter;
    gtk_list_store_clear(listPtr2);

    std::vector<StreetIdx> matchedIds = findStreetIdsFromPartialStreetName(prefix);



    //What to do if the string contains a & 
    std::size_t found = prefix.find_first_of("&");
    if (found != std::string::npos) {
        //Gets the first street in the entry
        std::string newString = prefix.substr(0, found - 1);
        std::vector<StreetIdx> newIds = findStreetIdsFromPartialStreetName(newString);
        std::vector<IntersectionIdx>interName1;
        for (int i = 0; i < newIds.size(); i++) {
            interName1 = getIntersectionNames(newIds[i]);

            //loads the list store
            for (int j = 0; j < interName1.size(); j++) {
                inter_section_2 = interName1[j];
                std::string data = getIntersectionName(interName1[j]);
                std::string u = stringSwap(data, newString);
                gtk_list_store_append(listPtr2, &iter);
                gtk_list_store_set(listPtr2, &iter, 0, u.c_str(), -1);
            }
        }
        return;
    }
    
    //If this is only a street
    for (int i = 0; i < matchedIds.size(); i++) {
        std::string data;
        data = getStreetName(matchedIds[i]);
        gtk_list_store_append(listPtr2, &iter);
        gtk_list_store_set(listPtr2, &iter, 0, data.c_str(), -1);
    }

}

//String swap function to switch strings for specific cases
std::string stringSwap(std::string s, std::string newS) {
    std::size_t found = s.find_first_of("&");
    std::string t;
    std::string first = s.substr(0, found - 1);
    if (newS == first) {
        t = s;
        return t;
    }
    t = newS + " & " + first;
    return t;
}


//Change map from Combo box

void mapChange(GtkComboBox* combo_box, gpointer user_data) {


    //Get the MapName from the ID
    auto app = static_cast<ezgl::application*> (user_data);
    const char* text = gtk_combo_box_get_active_id(combo_box);
    std::string text_ID(text);
    closeMap();
    loadMap("/cad2/ece297s/public/maps/" + text_ID + ".streets.bin");

    //After loading the map this will reset the main canvas
    app->refresh_drawing();
    ezgl::rectangle new_world1 = ezgl::rectangle(ezgl::point2d(xMin, yMin), ezgl::point2d(xMax, yMax));
    std::string main_canvas_id = app->get_main_canvas_id();
    app->change_canvas_world_coordinates(main_canvas_id, new_world1);
    auto new_render = app ->get_renderer();
    new_render->set_visible_world(new_world1);
    draw_main_canvas(new_render);
    create_vector_inter();
    
    GObject* myText = app->get_object("TextInput");
    gtk_entry_set_text(GTK_ENTRY(myText), "");
   

}

void favMode(GtkComboBox* combo_box, gpointer user_data){
    std::cout << "entered favmod" <<std::endl;
    
    fav_Mode = true;
    
    auto app = static_cast<ezgl::application*>(user_data);
    const char* text = gtk_combo_box_get_active_id(combo_box);
    std::string text_ID(text);
    if (text_ID == "add"){ 
        std::cout<<text_ID<< " selected"<< std::endl;
        std::stringstream ss;
    ss << "Please click on favourite location";
    app->update_message(ss.str()); //updates message on status bar
    }  
    
    if (text_ID == "all"){
        setAllPins(true);
        print_pins = true;
        app->refresh_drawing();
    }
    
    if (text_ID == "hide"){
        setAllPins(false);
        print_pins = false;
        
        app->refresh_drawing();
    }
    
    if (text_ID == "exit"){
        fav_Mode = false;
        
        std::stringstream ss;
    ss << "Exited Favourites Mode";
    app->update_message(ss.str());
    app->refresh_drawing();
        
    }
    
           
}

void setAllPins(bool state){
    for (int i = 0; i < all_pins.size(); ++i){
        all_pins[i].highlighted = state;
    }
}

void createPin(GtkSearchEntry* search_bar, gpointer /*user_data*/) {

    //auto casting

    //auto app = static_cast<ezgl::application*> (user_data);
    
    //Gets entry from search bar and converts to strings
    GtkEntry* text_entry = GTK_ENTRY(search_bar);
    
    const char* text = gtk_entry_get_text(text_entry);
    
    std::string pin_name(text);
    
    std::cout << pin_name << std::endl;
    
    temp_pin.highlighted = false;
    temp_pin.name = pin_name;
    temp_pin.position = {getXfromLon(temp_pos.longitude()),getYfromLat(temp_pos.latitude())};
    

    
    
}

void setIcon(GtkComboBox* combo_box, gpointer user_data){
    auto app = static_cast<ezgl::application*>(user_data);
    const char* text = gtk_combo_box_get_active_id(combo_box);
    std::string text_ID(text);
    
    
    
   
        temp_pin.icon = text_ID;
    
    
    all_pins.push_back(temp_pin);
    //std::cout <<temp_pin.name<<" "<<temp_pin.highlighted <<std::endl;
    //std::cout << "im here" << std::endl;
     
    new_pin_added = true;
    
    
    //std::cout << all_pins[] << std::endl;
    //app->refresh_drawing();
    for (int i = 0; i < all_pins.size(); ++i){
        std::cout <<all_pins[i].name<<" "<<temp_pin.highlighted <<std::endl;
        
    }
            std::stringstream ss;
            ss << "Pin "<<temp_pin.name<<" added at specified location";
            app->update_message(ss.str());
    
    
    //fav_Mode = false;
    
}


//Zooms to a certain location
//This function is to run the zoom for the second search bar similar implementation to mapZoom
void mapZoom1(GtkSearchEntry* search_bar, gpointer user_data) {
    pathDisplay=false;
    auto app = static_cast<ezgl::application*> (user_data);
    
    GObject *my_w = app -> get_object("TextInput");
    GtkEntry* wid = GTK_ENTRY(my_w);
    const char* firstText = gtk_entry_get_text(wid);
    std::string initial(firstText);
    
    //To prevent edge cases in the first from text entry
    if(initial==""){
        firstBar=false;
        std::stringstream ss;
        ss << "Please type a valid intersection in the first bar";
        app->update_message(ss.str());
        return;
    }
    else if(firstBar){
        firstBar=false;
        std::stringstream ss;
        ss << "Please type a valid intersection in the first bar";
        app->update_message(ss.str());
        return;
    }
    
    GtkEntry* text_entry = GTK_ENTRY(search_bar);
    const char* text = gtk_entry_get_text(text_entry);
    std::string prefix(text);

    std::vector<StreetIdx> mainId = findStreetIdsFromPartialStreetName(prefix);
    

    //What to do to zoom to a intersection
    std::size_t found = prefix.find_first_of("&");
    if (found != std::string::npos) {

        std::string newString = prefix.substr(0, found-1);
        std::vector<StreetIdx> newIds = findStreetIdsFromPartialStreetName(newString);

        LatLon int_pos1 {0,0};

        for (int i = 0; i < newIds.size(); i++) {
            std::vector<IntersectionIdx>interName = getIntersectionNames(newIds[i]);
            for (int j = 0; j < interName.size(); j++) {
                std::string t = getIntersectionName(interName[j]);
                std::string l = stringSwap(t,newString);
                if (prefix == l) {
                    int_pos1 = getIntersectionPosition(interName[j]);
                    select_two = interName[j];
                    break;
                } 
            }
        }
        if(int_pos1 == LatLon {0,0}){
            app->update_message("Unknown Intersection");
            return;
        }
        //Gets location and places pin
        xptr2 = getXfromLatLon(int_pos1, latAverage);
        yptr2 = getYfromLatLon(int_pos1);
        ezgl::renderer *g = app->get_renderer();
        double minX = min(xptr1,xptr2);
        double minY = min(yptr1,yptr2);
        double maxX = max(xptr1,xptr2);
        double maxY = max(yptr1,yptr2);
        
        ezgl::rectangle zoomVar({minX-20,minY-20}, {maxX+50,maxY+50});
        g->set_visible_world(zoomVar);
        app->update_message("");
        zoomPinMain = true;
        zoomPinFirst = true;
        navMode = true;
        pathDisplay = true;
        display_dir = true;


        std::pair<IntersectionIdx, IntersectionIdx>print_pair(select_one, select_two);
        
        print_vec = findPathBetweenIntersections(15,
                        print_pair);
        temp_dir = GetDirectionIds(print_vec);
        app->refresh_drawing();
        

        //g->draw_surface(pin,{xptr, yptr}, 0.5);
        //return;
    }

    //What to do if useless entry
    if (mainId.size() == 0) {
        
        return;
    }
    std::vector<IntersectionIdx> street_inter;
    street_inter.resize(findIntersectionsOfStreet(mainId[0]).size());

    street_inter = findIntersectionsOfStreet(mainId[0]);

    //Sets pin for Street
    LatLon street_pos;

    street_pos = getIntersectionPosition(street_inter[0]);

    xptr2 = getXfromLatLon(street_pos, latAverage);
    yptr2 = getYfromLatLon(street_pos);
    ezgl::renderer *g = app->get_renderer();
    g->set_visible_world({
        {xptr2 - 200, yptr2 - 200}, 400, 400
    });
    zoomPinFirst=true;
    //pathDisplay = true;
    app->refresh_drawing();
    //g->draw_surface(pin,{xptr, yptr}, 0.5);

}


void draw_main_canvas(ezgl::renderer *g) {
   
    g->set_line_cap(ezgl::line_cap::round);
    ezgl::rectangle visible_world = g->get_visible_world();
    double left = g->get_visible_world().left() + 2000;
    double right = g->get_visible_world().right() + 2000;
    double top = g->get_visible_world().top() + 2000;
    double bottom = g->get_visible_world().bottom() + 2000;
    
    ezgl::rectangle outer_world = { ezgl::point2d(left, bottom), ezgl::point2d(right, top)};
    draw_background(g,visible_world); //background for the whole map 
    draw_features(g); // drawing all the features

   
    //only draw certain elements of the map depending on the zoom level

    if (g->get_visible_world().right()-g->get_visible_world().left()<=9500) {
        draw_roads(g, outer_world);
    }
    if (g->get_visible_world().right() - g->get_visible_world().left() <= 10500) {
        if(subway)
            draw_subway(g);
    }
    if (g->get_visible_world().right()-g->get_visible_world().left()<=1000) 
        draw_POIs(g);
    
    
   //std::cout<< pathDisplay << ", main canvas"<< std::endl;    
    if(navMode) {
        if (intersections_plot[select_one].highlighted == true) {
            g->draw_surface(pin,{intersections_plot[select_one].position}, 0.5);
        }
        if (intersections_plot[select_two].secondHighlight == true) {
            g->draw_surface(finish_pin,{intersections_plot[select_two].position}, 0.05);
        }
    }


    if(pathDisplay && navMode) {
        int counterDirection =0;
        
        //std::cout<<"im here"<< endl;
        for (int i = 0; i < print_vec.size(); ++i) {
            //std::cout << "here" << std::endl;
            
  
                drawPath = true;
                streetData temp2;
                for (dataStructs.segMapIT = dataStructs.segMap.begin(); dataStructs.segMapIT != dataStructs.segMap.end(); dataStructs.segMapIT++) {
                    if ((*dataStructs.segMapIT).first == print_vec[i]) {
                        //std::cout << "here4" << std::endl;
                        temp2 = (*dataStructs.segMapIT).second;
                        break;
                    }
                }
                drawStreetSegments(print_vec[i], g, outer_world, temp2);
                
                
                
                //std::cout << "here2" << std::endl;
                std::string name = getStreetName(getStreetSegmentInfo(print_vec[i]).streetID);
                
                if(i+1<print_vec.size() && i>0 && display_dir) {
                    std::string name2 = getStreetName(getStreetSegmentInfo(print_vec[i+1]).streetID);
                    if ((getStreetSegmentInfo(print_vec[i]).streetID != getStreetSegmentInfo(print_vec[i + 1]).streetID)) { 
                        std::string s;
                        if(temp_dir[counterDirection] == HARD_LEFT){
                            s = "turn left to";
                        }
                        else if(temp_dir[counterDirection] == SLIGHT_LEFT){
                            s = "turn slightly left to";
                        }
                        else if(temp_dir[counterDirection] == SLIGHT_RIGHT){
                            s = "turn slightly right to";
                        }
                        else if(temp_dir[counterDirection] == HARD_RIGHT){
                            s = "turn right to";
                        }
                        else if(temp_dir[counterDirection] == STRAIGHT){
                            s = "continue straight to";
                        }
                        
                        //std::string s = std::to_string();
                        name = name + ", " + s;
                        //displayDir[i].instruction = name;
                        std::cout << "from "<< name << ", " << name2 << std::endl;
                        //std::cout << "here3" << std::endl;
                        counterDirection++;
                    }
                }
            
            
        }
        display_dir = false;
        counterDirection = 0;
    } 
    else{
       // std::cout<< "clearing" << std::endl;
        temp_dir.clear();
       print_vec.clear();
    }
     
    if (fav_Mode == true){
        g->draw_surface(cursor, {getXfromLon(temp_pos.longitude()),getYfromLat(temp_pos.latitude())}, 0.025);
    }
    if (fav_Mode == true && print_pins == true){
        
       for (int i = 0; i < all_pins.size(); ++i) {
           
        if(all_pins[i].icon == "def"){
          g->draw_surface(Default_Pin, {all_pins[i].position},0.025);
        }
        
        if(all_pins[i].icon == "par"){
          g->draw_surface(parking, {all_pins[i].position},0.05);
        }
        
        if(all_pins[i].icon == "dro"){
          g->draw_surface(Drop, {all_pins[i].position},0.17);
        }
        
        if(all_pins[i].icon == "pic"){
          g->draw_surface(pickup, {all_pins[i].position},0.08);
        }
        
        if(all_pins[i].icon == "del"){
          g->draw_surface(delivery, {all_pins[i].position},0.05);
        }
        
        if(all_pins[i].icon == "myl"){
          g->draw_surface(my_location, {all_pins[i].position},0.08);
        }
        
        if(all_pins[i].icon == "res"){
          g->draw_surface(Restaurant, {all_pins[i].position},0.05);
        }
        
        if(all_pins[i].icon == "ren"){
          g->draw_surface(rendezvous, {all_pins[i].position},0.025);
        }
        
        if(all_pins[i].icon == "uni"){
          g->draw_surface(uni, {all_pins[i].position},0.025);
        }
        
       } 
       
       print_pins = false;//please do not remove this
    }

    //// if an intersection was clicked on, display a pin
//    intersections_plot.resize(getNumIntersections());
//
//    for (IntersectionIdx i = 0; i < intersections_plot.size(); ++i) {
//
//        if (intersections_plot[i].highlighted == true) {
//            g->draw_surface(pin,{intersections_plot[i].position}, 0.5);
//            if (i != previous_inter_id) {
//                intersections_plot[i].highlighted = false;
//                //intersections_plot[second].secondHighlight = false;
//            }
//        }
//        if (intersections_plot[i].secondHighlight == true) {
//            g->draw_surface(finish_pin,{intersections_plot[i].position}, 0.04);
//            if (i != previous_inter_id2) {
//                intersections_plot[i].secondHighlight = false;
//                //intersections_plot[second].secondHighlight = false;
//            }
//        }
//    }


    if (zoomPinMain == true){
        g->draw_surface(pin,{xptr1, yptr1}, 0.5);
    }
    if (zoomPinFirst == true && zoomPinMain==true){
        g->draw_surface(finish_pin,{xptr2, yptr2}, 0.05);
        //g->draw_surface(Default_Pin,{xptr1, yptr1}, 0.02);
    }

    if (zoomPins == true){
        g->draw_surface(pin,{xptrs, yptrs}, 0.5);
    }

}

void draw_background(ezgl::renderer *g, ezgl::rectangle world) {
    if (dark) {
        g->set_color(105, 105, 105);
    }
    else {
        g->set_color(232, 232, 232);
    }

    g->fill_rectangle(world);
}

//converts the colors from OSM database into our map
void colour_converter(ezgl::renderer *g, std::string subwayCol) {
    g->set_line_width(5);
    std::string hash = "#";
    
    //removes the hash before some of the hex numbers
    for (char c : hash) {
        subwayCol.erase(std::remove(subwayCol.begin(), subwayCol.end(), c), subwayCol.end());
    }
    
    
    if (subwayCol == "green") {
        g->set_color(ezgl::DARK_GREEN);

    } else if (subwayCol == "yellow") {
        g->set_color(ezgl::YELLOW);
    } else if (subwayCol == "purple") {
        g->set_color(ezgl::PURPLE);
    } else if (subwayCol == "blue") {
        g->set_color(ezgl::BLUE);
    } else {
        // converting hex to string and then extracting rgb colors
        const char *hexStr = subwayCol.c_str();
        int hexNumber = (int)strtol(hexStr, NULL, 16);

        double red = ((hexNumber >> 16) & 0xFF);  // Extract the RR byte
        double green = ((hexNumber >> 8) & 0xFF) ;   // Extract the GG byte
        double blue = ((hexNumber) & 0xFF) ; //extract the BB byte

        g->set_color(red,green,blue);


    }
}

void draw_subway(ezgl::renderer *g) {

    for (int i = 0; i < dataStructs.subwayInfo.size(); i++) {
        for (int j = 0; j < dataStructs.subwayInfo[i].second.size(); j++) {
            if ((j + 1) < dataStructs.subwayInfo[i].second.size()) {

                colour_converter(g, dataStructs.subwayInfo[i].first);
                
                // draw the stops
                g->fill_arc(dataStructs.subwayInfo[i].second[j], 5, 0, 360);
                //draws the subway lines between the stops
                g->draw_line(dataStructs.subwayInfo[i].second[j], dataStructs.subwayInfo[i].second[j + 1]);
            } else
                g->fill_arc(dataStructs.subwayInfo[i].second[j], 5, 0, 360);
        }
    }
}


double getXfromLatLon(LatLon position, double latAvg) {
    return kEarthRadiusInMeters * position.longitude() * kDegreeToRadian * cos(latAvg * kDegreeToRadian);
}

double getYfromLatLon(LatLon position) {
    return position.latitude() * kDegreeToRadian * kEarthRadiusInMeters;
}

double lon_from_x(double x) {
    return x/(kEarthRadiusInMeters * kDegreeToRadian * cos(latAverage * kDegreeToRadian));
}

double lat_from_y(double y) {
    return y/(kDegreeToRadian * kEarthRadiusInMeters);
}

// this function calls the datastructure from M1 and draws all the features
// for every color, there is an associated dark mode color. 
// the color pallete is similar to the one to google maps
void draw_features(ezgl::renderer *g) {
    g->set_line_width(5);

    for (dataStructs.featureIT = dataStructs.featureMap.begin(); dataStructs.featureIT != dataStructs.featureMap.end(); dataStructs.featureIT++) {
        // any features that are not in the list, make them the same as a building

        if (dark) {
            g->set_color(119, 136, 153);
        }
        else {
            // if it is not any of the given feature types, it has the same color
            // as a building
            g->set_color(255, 242, 175); // light yellow
        }

        FeatureType typeFeature = (*dataStructs.featureIT).second.type;
        //int pointsFeature = (*dataStructs.featureIT).second.numPoints;
        //FeatureIdx i = (*dataStructs.featureIT).second.id;
        double areaFeature = (*dataStructs.featureIT).first;


        //INTEGER VALUES FOR DIFFERENT FEATURES

        /*"PARK"= 1||"BEACH"= 2||"LAKE"= 3 ||"RIVER"= 4||"ISLAND"= 5 
         * ||"BUILDING"= 6||"GREENSPACE"= 7||"GOLFCOURSE"= 8||"STREAM"= 9
         * ||"GLACIER"= 10 */

        // if it is a park, greenspace or golfcourse
        if (typeFeature == PARK || typeFeature == GREENSPACE || typeFeature == GOLFCOURSE) {
                g->set_color(195, 236, 178); // google maps green
            

        }


        // island
        else if (typeFeature == ISLAND) {
            if(dark){
                g->set_color(105,105,105);
            }
            else{
                g->set_color(232, 232, 232);
            }
        }
        
        // beaches
        else if (typeFeature == BEACH) {
            if(dark){
                g->set_color(160,82,45);
            }
            else{
                
                g->set_color(255, 247, 207); // ezgl yellow differentiation
            }
        }
        // lakes, rivers and water streams
        else if (typeFeature == LAKE || typeFeature == RIVER || typeFeature == STREAM) {
            if(dark){
                g->set_color(0,0,80);
            }
            else{
                g->set_color(170, 218, 255);
            }
        }
        //glaciers
        else if (typeFeature == GLACIER) {
            if (dark)
                g->set_color(105, 105, 105);
            else
                g->set_color(ezgl::WHITE);
        }

        // this if statement is crucial. it only draws the buildings at a certain 
        // zoom level. 
        if (typeFeature != 6 ||
                g->get_visible_world().right() - g->get_visible_world().left() <= 2000) {
            
                //if the feature is closed and has an area
            if (areaFeature > 0) {
                g->fill_poly((*dataStructs.featureIT).second.all_points);

            }

        }

    }

}


//places all the icons depending on the POI category
void draw_POIs(ezgl::renderer *g) {
    // value with that key is the poi type (value = poitype)
    g->set_text_rotation(0);
    for (int l = 0; l < getNumPointsOfInterest(); l++) {

        OSMID POI_ID = getPOIOSMNodeID(l);
        double x = getXfromLatLon(getPOIPosition(l), latAverage);
        double y = getYfromLatLon(getPOIPosition(l));

        if (dataStructs.typePOI[POI_ID] == "restaurant"
                && g->get_visible_world().right() - g->get_visible_world().left() <= 500) {
            g->draw_surface(res,{x, y}, 0.5);
            text_POI(g, l);

        }
        else if (dataStructs.typePOI[POI_ID] == "bar"
                && g->get_visible_world().right() - g->get_visible_world().left() <= 300) {
            g->draw_surface(bar,{x, y}, 0.5);
            text_POI(g, l);

        } else if (dataStructs.typePOI[POI_ID] == "education" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 1500) {
            g->draw_surface(education,{x, y}, 0.5);
            text_POI(g, l);
            ;
        } else if (dataStructs.typePOI[POI_ID] == "financial" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 1000) {
            g->draw_surface(financial,{x, y}, 0.5);
            text_POI(g, l);
            ;
        } else if (dataStructs.typePOI[POI_ID] == "bus_station" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 800) {
            g->draw_surface(bus_station,{x, y}, 0.5);
            text_POI(g, l);
            ;
        } else if (dataStructs.typePOI[POI_ID] == "fuel" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 1200) {
            g->draw_surface(fuel,{x, y}, 0.5);
            text_POI(g, l);
            ;
        } else if (dataStructs.typePOI[POI_ID] == "hospital" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 1500) {
            g->draw_surface(hospital,{x, y}, 0.5);
            text_POI(g, l);
            ;
        } else if (dataStructs.typePOI[POI_ID] == "entertainment" &&
                g->get_visible_world().right() - g->get_visible_world().left() <= 800) {
            g->draw_surface(entertainment,{x, y}, 0.5);
            text_POI(g, l);
            ;
        }
    }
}

void text_POI(ezgl::renderer *g, int l) {
    //drawing the text for all poi
    if (g->get_visible_world().right() - g->get_visible_world().left() <= 200) {
        //dark mode toggle
        if (dark) {
            g->set_color(ezgl::WHITE);
        } else {
            g->set_color(ezgl::BLACK);
        }

        g->set_font_size(10);

        g->draw_text(ezgl::point2d(getXfromLatLon(getPOIPosition(l), latAverage),
                getYfromLatLon(getPOIPosition(l))), getPOIName(l), 35, DBL_MAX);
    }
}

void check_road(ezgl::renderer *g, std::string type) {

    // check which type of road it is and set the color accordingly
    if (type == "motorway") {
        if (dark) {
            g->set_color(210, 105, 30);
        } else {
            g->set_color(246, 207, 101);
        }
        g->set_line_width(7);
    }
    else if (type == "secondary") {
        if (dark) {
            g->set_color(ezgl::BLACK);
        } else {
            g->set_color(ezgl::WHITE);
        }
        g->set_line_width(5);
    }
    else{
        if (dark) {
            g->set_color(ezgl::BLACK);
        } else {
            g->set_color(ezgl::WHITE);
        }
        g->set_line_width(2);
    }
    
    //if we are in navigation mode
    if(drawPath){
        g->set_color(ezgl::RED);
        g->set_line_width(5);
    }
    
}

void text_roads(ezgl::renderer *g, double x1, double y1, double x2, double y2, StreetSegmentInfo seg, double angle) {
    // set color of text
    if (dark) {
        g->set_color(ezgl::WHITE);
    }
    else {
        g->set_color(ezgl::BLACK);
    }


    std::string name = getStreetName(seg.streetID);

    // setting the angles for the text

    g->set_text_rotation(angle);
    
    //std::string s = std::to_string(angle);
    // the conditional statement for one way and drawing arrows
    if(seg.oneWay == true){
        if(x2-x1 >=0 ){
            name = ">>> " + name + " >>>";
        }
        else 
            name = "<<< " + name + " <<<";

    }


    //draw the text
    g->draw_text(ezgl::point2d((x1 + x2) / 2, (y1 + y2) / 2), name, abs(x2 - x1), abs(y2 - y1));

}

void drawStreetSegments(StreetSegmentIdx streetSeg, ezgl::renderer *g, ezgl::rectangle outer_world, streetData bruh){
    
        if(dark){
            g->set_color(ezgl::BLACK);
        } else {
            g->set_color(ezgl::WHITE);
        }
        g->set_line_width(2);

        ////////// FOR ORGANIZATION PURPOSES
        // defining the start and end of segments 
        // so that it is easier to implement in the later functions
        StreetSegmentInfo segInfo = getStreetSegmentInfo(streetSeg);

        // intersection from/to positions
        LatLon from_position = getIntersectionPosition(segInfo.from);
        LatLon to_position = getIntersectionPosition(segInfo.to);

        if (!outer_world.contains(ezgl::point2d(from_position.longitude(),from_position.latitude())) || !outer_world.contains(ezgl::point2d(to_position.longitude(),to_position.latitude())) ) {
            // setting the locations for the start of the start/end of segments

            check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);

            // if it is a straight road
            if (segInfo.numCurvePoints == 0) {
                g->draw_line(bruh.start_segment, bruh.end_segment);
                if (getStreetName(segInfo.streetID) != "<unknown>")
                    text_roads(g, bruh.start_segment.x, bruh.start_segment.y, bruh.end_segment.x, bruh.end_segment.y, segInfo, bruh.angle);


            } else {
                

                //check road type
                check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);

                //drawing from the from intersection to the first curve points
                //ezgl::point2d end_firstCurve(getXfromLatLon(getStreetSegmentCurvePoint(0, streetSeg), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(0, streetSeg)));
                g->draw_line(bruh.start_segment, bruh.end_firstCurve);

                if (getStreetName(segInfo.streetID) != "<unknown>") {
                    text_roads(g, bruh.start_segment.x, bruh.start_segment.y, bruh.end_firstCurve.x, bruh.end_firstCurve.y, segInfo, bruh.firstCurveAngle);

                }

                //drawing from the first to the last curve point
                for (int j = 0; j < segInfo.numCurvePoints - 1; j++) {
                    //ezgl::point2d start(getXfromLatLon(getStreetSegmentCurvePoint(j, streetSeg), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(j, streetSeg)));
                    //ezgl::point2d end(getXfromLatLon(getStreetSegmentCurvePoint(j + 1, streetSeg), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(j + 1, streetSeg)));
                    
                    check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);
                    
                    ezgl::point2d start = bruh.curvePoints_start[j];
                    ezgl::point2d end = bruh.curvePoints_end[j];
                    double angle = bruh.curveAngle[j];
                    
                    g->draw_line(start, end);

                    if (getStreetName(segInfo.streetID) != "<unknown>") {
                        text_roads(g, start.x, start.y, end.x, end.y, segInfo, angle);


                    }
                }

                //drawing from the last curve point to the to intersection
                check_road(g, dataStructs.majorRoadType[segInfo.wayOSMID]);
               // ezgl::point2d start_lastCurve(getXfromLatLon(getStreetSegmentCurvePoint(segInfo.numCurvePoints - 1, streetSeg), latAverage), getYfromLatLon(getStreetSegmentCurvePoint(segInfo.numCurvePoints - 1, streetSeg)));

                g->draw_line(bruh.start_lastCurve, bruh.end_segment);

                if (getStreetName(segInfo.streetID) != "<unknown>") {
                    text_roads(g, bruh.start_lastCurve.x, bruh.start_lastCurve.y, bruh.end_segment.x, bruh.end_segment.y, segInfo, bruh.lastCurveAngle);

                }
            }
        }
        drawPath=false;
}

void draw_roads(ezgl::renderer *g, ezgl::rectangle outer_world) {

    g->set_font_size(10.5);
    for (dataStructs.segMapIT = dataStructs.segMap.begin(); dataStructs.segMapIT != dataStructs.segMap.end(); dataStructs.segMapIT++) { 
        StreetSegmentIdx streetSeg = (*dataStructs.segMapIT).first;
        
        drawStreetSegments(streetSeg, g, outer_world, (*dataStructs.segMapIT).second );
    }
}

double getXfromLon(double position) {
    return kEarthRadiusInMeters * position * kDegreeToRadian * cos(latAverage * kDegreeToRadian);
}

double getYfromLat(double position) {
    return position * kDegreeToRadian * kEarthRadiusInMeters;
}



// Populates a vector of structs Intersection_data by parsing through 
// all intersection Ids
void create_vector_inter() {
    intersections_plot.resize(getNumIntersections());

    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id) {
        //uses getXfromLatLon to populate vector using cartesian coordinates
        intersections_plot[id].position.x = getXfromLon(getIntersectionPosition(id).longitude());
        intersections_plot[id].position.y = getYfromLat(getIntersectionPosition(id).latitude());
        intersections_plot[id].name = getIntersectionName(id);
        intersections_plot[id].highlighted = false;

    }
}

// Activates whenever mouse clicked;
//returns intersection name in GUI;
void act_on_mouse_click(ezgl::application* app, GdkEventButton* /* event */, double x, double y) {
    pathDisplay = false;
    pathMode = false;
    if (fav_Mode == true) {
        LatLon fav_pos = LatLon(lat_from_y(y), lon_from_x(x));

        temp_pos = fav_pos;

        std::stringstream ss;
        ss << "Please enter name in box below and press enter, then select category";
        app->update_message(ss.str());
        app->refresh_drawing();
    }

    if (fav_Mode == false && navMode) {
        LatLon click_pos = LatLon(lat_from_y(y), lon_from_x(x));

        std::pair<LatLon, LatLon> find_distance;

        find_distance.first = click_pos;

        find_distance.second = LatLon(lat_from_y(intersections_plot[findClosestIntersection(click_pos)].position.y),
                lon_from_x(intersections_plot[findClosestIntersection(click_pos)].position.x));


        double distance_from_closest = findDistanceBetweenTwoPoints(find_distance);


        if (distance_from_closest > 130) {
            std::stringstream ss;
            ss << "Please move closer to desired intersection.";
            app->update_message(ss.str());
            app->refresh_drawing();

        } else {
            ++clickCount;

            if (clickCount == 1) {
                select_one = findClosestIntersection(click_pos);
                
                intersections_plot[findClosestIntersection(click_pos)].highlighted = true;
                previous_inter_id = findClosestIntersection(click_pos);
                
                intersections_plot[select_two].secondHighlight = false;
                
                
               // std::cout << "first selection is " << select_one << std::endl;
            } else if (clickCount == 2) {
                select_two = findClosestIntersection(click_pos);
                intersections_plot[select_two].secondHighlight = true;
                previous_inter_id2 = select_two;
                
                //std::cout << "second selection is " << select_two << std::endl;
                std::pair<IntersectionIdx, IntersectionIdx> selects(select_one, select_two);
                //std::cout << "secdgdfgdfgdction is "<<select_two<<std::endl;

                std::pair<IntersectionIdx, IntersectionIdx>print_pair(select_one, select_two);

                print_vec = findPathBetweenIntersections(15,
                        print_pair);


                //std::cout << "There are " << print_vec.size() << " values" << std::endl;
                //std::cout << "Segments passed through" << std::endl;

                //std::cout << "************************" << std::endl;
                temp_dir = GetDirectionIds(print_vec);
               // std::cout << "There are " << temp_dir.size() << " directions" << std::endl;
                //std::cout << "************************" << std::endl;
//                for (int i = 0; i < temp_dir.size(); ++i) {
//                    std::cout << temp_dir[i] << std::endl;
//                }
                

                            
                pathDisplay = true;
                display_dir = true;
                pathMode = true;

                clickCount = 0;
                best_time_tracker.clear();

                

            }


            intersections_plot[findClosestIntersection(click_pos)].highlighted = true;

            previous_inter_id = findClosestIntersection(click_pos);

            


            std::stringstream ss;
            ss << "Intersection Selected: " << intersections_plot[findClosestIntersection(click_pos)].name;
            app->update_message(ss.str()); //updates message on status bar
            app->refresh_drawing();
        }
    }
    //pathDisplay = false;
    
} 

std::vector<StreetSegmentIdx> findPathBetweenIntersections(const double turn_penalty,
        const std::pair<IntersectionIdx, IntersectionIdx> intersect_ids){

    bool found_desti = false; //sets to true if path is legal
    IntersectionIdx end_desti; //stores final destination
    std::vector <StreetSegmentIdx> back_trace; //vector used to back trace final path
    
    if(intersect_ids.first==intersect_ids.second){
        return back_trace;
    }
    
    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id) {
        best_time_tracker.push_back(tempObject(-1, INFINITY));  
        //initializing best_time_tracker with really high
        //value to store first iteration
    }
    
    //creating MIN HEAP priority queue
    std::priority_queue <waveObject, std::vector<waveObject>, compareHeuristicTime> waveFront;

    waveFront.push(waveObject(intersect_ids.first, NO_EDGE, 0, 0)); //pushing head
    
    if (intersect_ids.first == intersect_ids.second) {

        return{}; //return empty vector if already in destination

    }//returns empty vector


    while (!waveFront.empty()) {

        waveObject extracted = waveFront.top();

        waveFront.pop(); //pop the top of heap or priority queue based on comparator

        IntersectionIdx currID = extracted.interID;
        // std::cout<<"intersection parsed "<<currID<<std::endl;


        double temp_time = extracted.timePeriod;
        //stores total travel time

        if (temp_time < best_time_tracker[currID].bestTime) {

            best_time_tracker[currID].bestTime = temp_time;
            best_time_tracker[currID].trackEdge = extracted.trackEdge; //new best_time should be less than current best_time to 
                                                                        //push into priority queue

            if (currID == intersect_ids.second) { 

                // std::cout<<"the destination is "<<currID<<std::endl;
                found_desti = true;
                end_desti = currID;
                break; //break for loop of destination reached
            }

            std::vector<StreetSegmentIdx> temp_seg = dataStructs.vector_inter[currID];

            for (int con_seg = 0; con_seg < temp_seg.size(); ++con_seg) {
                if (temp_seg[con_seg] != extracted.trackEdge) {
                    IntersectionIdx t3 = getStreetSegmentInfo(temp_seg[con_seg]).from;
                    //t3 stores the third intersection point from current node reference (the intersection we will travel to)
                    if (getStreetSegmentInfo(temp_seg[con_seg]).oneWay && currID != t3) { 
                        continue; //skip iteration because cannot travel from "to" to "from" in one-way street
                    }
                    

                    if (t3 == currID) {
                        t3 = getStreetSegmentInfo(temp_seg[con_seg]).to;
                    } // if traveling from "from" to "to" set t3 to segment.to


                    //std::pair<IntersectionIdx,IntersectionIdx> 
                    //start_end(currID,getStreetSegmentInfo(temp_seg[con_seg]).to);

                    int temp_turn_penalty = 0; //initialize turn penalty to 0 for every edge explored
                    //turn penalty is 0
                    if (extracted.trackEdge != NO_EDGE) { //prevent tracking edge of head to be searched in database
                        if (getStreetSegmentInfo(temp_seg[con_seg]).streetID != getStreetSegmentInfo(extracted.trackEdge).streetID) { 
                            //update turn penalty equal to argument value if street change detected
                            temp_turn_penalty = turn_penalty;
                        }
                    }

                    std::pair<LatLon, LatLon> heuristic(getIntersectionPosition(t3), getIntersectionPosition(intersect_ids.second));
                    //initialize heauristic points for distance from exploring node and destination

                    double total_time_to_reach_inter = temp_time + findStreetSegmentTravelTime(temp_seg[con_seg]) + temp_turn_penalty;

                    double push_h = temp_time + findStreetSegmentTravelTime(temp_seg[con_seg]) + temp_turn_penalty + findDistanceBetweenTwoPoints(heuristic) / 30;


                    waveFront.push(waveObject(t3, temp_seg[con_seg], total_time_to_reach_inter, push_h)); // push into wavefront
                }
            }
        }
    }



    if (found_desti == true) {
        IntersectionIdx tracing_id = end_desti; //current tracing id is end destination intersection
        IntersectionIdx t2 = tracing_id;
        StreetSegmentIdx prev_edge = best_time_tracker[tracing_id].trackEdge;
        back_trace=backtracer(t2,tracing_id,prev_edge,back_trace);
    }
    best_time_tracker.clear();
    return back_trace;
}


std::vector<StreetSegmentIdx> backtracer(IntersectionIdx tracer, IntersectionIdx tracerId,StreetSegmentIdx previousEdge,
                    std::vector <StreetSegmentIdx> back_traced){
    
    while(previousEdge != NO_EDGE){
            back_traced.push_back(previousEdge);
            tracer = getStreetSegmentInfo(previousEdge).from;
            if (tracer == tracerId) {
                tracer = getStreetSegmentInfo(previousEdge).to;
            }
            previousEdge = best_time_tracker[tracer].trackEdge;
            tracerId = tracer;
    }

    best_time_tracker.clear();

    std::reverse(back_traced.begin(), back_traced.end());
    return back_traced;

}

double computePathTravelTime(const double turn_penalty, const std::vector<StreetSegmentIdx>& path){
    double totalTime = 0;
    double turn_time = 0;
    
    for (int parser = 0; parser < path.size(); ++parser){ //this loop calculates time without turn penalty
        double temp_time = findStreetSegmentTravelTime(path[parser]);
        totalTime = totalTime + temp_time;
    }
         
    for (int change_seg = 0; change_seg < path.size(); ++change_seg){ // this calculates number of turns
         
        if(change_seg == path.size() - 1) break;//break when final iteration reached to prevent seg fault
        
         if(getStreetSegmentInfo(path[change_seg]).streetID != getStreetSegmentInfo(path[change_seg+1]).streetID){
             turn_time = turn_time + turn_penalty;
         }
      
    }
    
    //std::cout<<totalTime+turn_time<<std::endl;
    return totalTime + turn_time; //return the sum along with turn penalty
}


DirectionIdx getTurnId(IntersectionIdx from_inter, IntersectionIdx con_inter, IntersectionIdx to_inter){
    
    double x1,y1,x2,y2, x3, y3;
    x1 = getIntersectionPosition(from_inter).longitude();
    y1 = getIntersectionPosition(from_inter).latitude();
    x2 = getIntersectionPosition(con_inter).longitude();
    y2 = getIntersectionPosition(con_inter).latitude();
    x3 = getIntersectionPosition(to_inter).longitude();
    y3 = getIntersectionPosition(to_inter).latitude();
 
    double firstAngle, secondAngle, turning_angle;
    //bool flipRight = false;
    bool flip = false;
    
//    if(y3-y1 >0 && x3-x1 <0){
//        flip = true;
//    }

    //calculate street segment angles
    firstAngle = (180/PI) * abs(atan2((y2-y1),(x2-x1+ep)));
    secondAngle = (180/PI) *  abs(atan2((y3-y2),(x3-x2+ep)));
    turning_angle = secondAngle - firstAngle;
    int direction;
    
    std::cout<< firstAngle<< ", " << secondAngle << ", " << turning_angle<< std::endl;
    
    if(turning_angle>=-15 && turning_angle <= 15)
        direction= STRAIGHT;
    
    if(turning_angle>15 && turning_angle <= 25)
        direction=  SLIGHT_LEFT;
    if(turning_angle>25){
        direction=  HARD_LEFT;

        if(flip){
            direction = HARD_RIGHT;
            flip = false;
        }
    }
 
    if(turning_angle<-15 && turning_angle >=-25)
        direction=  SLIGHT_RIGHT;
    if(turning_angle<-25){
        
        direction=  HARD_RIGHT;
        if(flip){
            direction = HARD_LEFT;
            flip= false;
        }

    }
    
    return direction;
  
} 

std::vector <DirectionIdx> GetDirectionIds(const std::vector<StreetSegmentIdx> path){
    
    int path_size = path.size();
    
    IntersectionIdx from;
    IntersectionIdx con;
    IntersectionIdx to;
    
    std::vector<DirectionIdx> finalDir;
    if(path.empty()){
        return {};
    }
    
    if(path.empty()) return{};
    
     if((getStreetSegmentInfo(path[0]).from == getStreetSegmentInfo(path[1]).to) || getStreetSegmentInfo(path[0]).from == getStreetSegmentInfo(path[1]).from){
                con = getStreetSegmentInfo(path[1]).from; from = getStreetSegmentInfo(path[0]).to;
                
                if(getStreetSegmentInfo(path[1]).from == con){
                    to = getStreetSegmentInfo(path[1]).to;
                }else{
                    to = getStreetSegmentInfo(path[1]).from;
                }
                
     }else{
                from = getStreetSegmentInfo(path[0]).from;
                con = getStreetSegmentInfo(path[0]).to;
                
                if(getStreetSegmentInfo(path[1]).from == con){
                    to = getStreetSegmentInfo(path[1]).to;
                }else{
                    to = getStreetSegmentInfo(path[1]).from;
                }
     }
    
    for(StreetSegmentIdx seg_parse = 0; seg_parse < path_size; ++seg_parse){
        if (seg_parse == path_size - 1) break;
        
        if(getStreetSegmentInfo(path[seg_parse]).streetID != getStreetSegmentInfo(path[seg_parse+1]).streetID){
            finalDir.push_back(getTurnId(from,con,to));
        }
        //else{finalDir.push_back(SAME_STREET);}
        
        if (seg_parse == path_size - 2) break;
        
        from = con;
        con = to;
        
        if(getStreetSegmentInfo(path[seg_parse+2]).from == con){
                    to = getStreetSegmentInfo(path[seg_parse+2]).to;
        }else{
                    to = getStreetSegmentInfo(path[seg_parse+2]).from;
        }
       
   
    }
    return finalDir;
}




void findMultiPathBetweenIntersections(const double turn_penalty,IntersectionIdx intersect_id,const std::vector<DeliveryInf>& deliveries){
    
    int num_pickups = deliveries.size();
    better_times.resize(num_pickups*2);
    
    std::unordered_set<IntersectionIdx> checklist;
    
    for (int del_parse = 0; del_parse < num_pickups; ++del_parse){ 
        checklist.insert(deliveries[del_parse].pickUp);
        checklist.insert(deliveries[del_parse].dropOff);
        
        indexer.insert(make_pair(deliveries[del_parse].pickUp,index_info(del_parse,true)));
        indexer.insert(make_pair(deliveries[del_parse].dropOff,index_info(del_parse,false)));
        
        better_times[del_parse].resize(num_pickups*2);
        better_times[del_parse + num_pickups].resize(num_pickups*2);
        
        
    }
    
    IntersectionIdx end_desti; 
    std::vector <StreetSegmentIdx> new_back_trace; 


    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id) {
        new_best_time_tracker.push_back(tempObject(-1, INFINITY));  
    }
   
    std::priority_queue <newWaveObject, std::vector<newWaveObject>, newCompareTime> waveFront;

    waveFront.push(newWaveObject(intersect_id, NO_EDGE, 0));
    
    while (!waveFront.empty()) {

        newWaveObject extracted = waveFront.top();

        waveFront.pop(); 

        IntersectionIdx currID = extracted.interID;

        double temp_time = extracted.timePeriod;
        

        if (temp_time < new_best_time_tracker[currID].bestTime) {

            new_best_time_tracker[currID].bestTime = temp_time;
            new_best_time_tracker[currID].trackEdge = extracted.trackEdge; 
            
            std::unordered_set<IntersectionIdx>::iterator set_it = checklist.find(currID);
           
            if (set_it != checklist.end()) { 
                checklist.erase(set_it);
                end_desti = currID;
                
                    IntersectionIdx tracing_id = end_desti; 
                    IntersectionIdx t2 = tracing_id;

                    StreetSegmentIdx prev_edge = new_best_time_tracker[tracing_id].trackEdge;

                    while (prev_edge != NO_EDGE) { 

                        new_back_trace.push_back(prev_edge);
                        t2 = getStreetSegmentInfo(prev_edge).from;
                        if (t2 == tracing_id) {
                            t2 = getStreetSegmentInfo(prev_edge).to;
                        }
                        prev_edge = new_best_time_tracker[t2].trackEdge;
                        tracing_id = t2;
                    }

                    std::reverse(new_back_trace.begin(), new_back_trace.end());
                    double min_time = new_best_time_tracker[end_desti].bestTime;
                    if (min_time == 0) min_time = INFINITY;
                    
                    //good_times[intersect_id].push_back(end_point_info(end_desti,min_time,new_back_trace));
                 
                    auto match = indexer.equal_range(intersect_id);
                    
                    for (auto map_it = match.first; map_it != match.second; ++map_it){
                        
                        auto match2 = indexer.equal_range(end_desti); 
                        
                        for(auto map2_it = match2.first; map2_it != match2.second; ++map2_it){
                        
                            if(map_it->second.pickup == true && map2_it->second.pickup == true){
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index].end_point = end_desti;
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index].time = min_time;
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index].path = new_back_trace;
                            }else if(map_it->second.pickup == true && map2_it->second.pickup == false){
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index + num_pickups].end_point = end_desti;
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index + num_pickups].time = min_time;
                                better_times[map_it->second.delivery_index][map2_it->second.delivery_index + num_pickups].path = new_back_trace;
                            }else if(map_it->second.pickup == false && map2_it->second.pickup == true){
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index].end_point = end_desti;
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index].time = min_time;
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index].path = new_back_trace;
                            }else{
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index + num_pickups].end_point = end_desti;
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index + num_pickups].time = min_time;
                                better_times[map_it->second.delivery_index + num_pickups][map2_it->second.delivery_index + num_pickups].path = new_back_trace;
                            }
                        }
                        
                    }
                    
                    new_best_time_tracker.clear();
                    new_back_trace.clear();
                    
                    if(checklist.empty()) break;
           }

            std::vector<StreetSegmentIdx> temp_seg = dataStructs.vector_inter[currID];

            for (int con_seg = 0; con_seg < temp_seg.size(); ++con_seg) {
                if (temp_seg[con_seg] != extracted.trackEdge) {
                    IntersectionIdx t3 = getStreetSegmentInfo(temp_seg[con_seg]).from;
                   
                    if (getStreetSegmentInfo(temp_seg[con_seg]).oneWay && currID != t3) { 
                        continue; 
                    }
                    
                    if (t3 == currID) {
                        t3 = getStreetSegmentInfo(temp_seg[con_seg]).to;
                    } 

                    int temp_turn_penalty = 0; 
                    if (extracted.trackEdge != NO_EDGE) { 
                        if (getStreetSegmentInfo(temp_seg[con_seg]).streetID != getStreetSegmentInfo(extracted.trackEdge).streetID) { 
                      
                            temp_turn_penalty = turn_penalty;
                        }
                    }
                    
                    double total_time_to_reach_inter = temp_time + findStreetSegmentTravelTime(temp_seg[con_seg]) + temp_turn_penalty;

                    waveFront.push(newWaveObject(t3, temp_seg[con_seg], total_time_to_reach_inter)); 
                }
            }
        }
    }

   
	

}


std::vector<CourierSubPath> travelingCourier(const float turn_penalty, const std::vector<DeliveryInf>& deliveries, const std::vector<IntersectionIdx>& depots) {
    std::vector<CourierSubPath> greedyPath = greedyAlgo(turn_penalty, deliveries, depots);
    return greedyPath;
}
int conterMe=0;
std::vector<CourierSubPath> greedyAlgo(const float turn_penalty, const std::vector<DeliveryInf>& deliveries, const std::vector<IntersectionIdx>& depots){
    conterMe++;
    std::cout<<"Count= "<<conterMe<<std::endl;
    std::cout<<"called"<<std::endl;
    for(int parse_inter = 0; parse_inter < deliveries.size(); ++parse_inter){
            findMultiPathBetweenIntersections(turn_penalty,deliveries[parse_inter].pickUp,deliveries);
            findMultiPathBetweenIntersections(turn_penalty,deliveries[parse_inter].dropOff,deliveries);               
    } 
    
    
    
    for(int q = 0; q < 2*deliveries.size(); ++q){
        for(int v = 0; v < 2*deliveries.size(); ++v){
            std::cout<<better_times[q][v].time<<std::endl;    
        }
    }
    std::cout<<"matrix created"<<std::endl;
    
    int del_index;
    int del_index2;
    bool pick2;
    int condCheck=0; 
    
    int num_pickups = deliveries.size();
    
    std::vector<CourierSubPath> path;
    
    CourierSubPath tempath;
    
    std::vector<std::pair<IntersectionIdx,IntersectionIdx>> destinations;
    
    double x = 9999999999;
    double distP=0,distD=0;
    
    IntersectionIdx depotStart,firstPickUp=0,possibleP=0,possibleD=0;
    IntersectionIdx end = 0;
    IntersectionIdx start = 0;
    
    double turn = (double)turn_penalty;
    
    //This creates my checker
    
    for(int i=0; i<deliveries.size();i++){
        destinations.push_back({deliveries[i].pickUp,deliveries[i].dropOff});
    }
    
    if(deliveries.empty()){
        return path;
    }
    
    //This finds the closest depot and closest pick up

    for(int i=0; i<depots.size();i++){
        for(int j = 0; j<deliveries.size();j++){
            double dist = findDistanceBetweenTwoPoints({getIntersectionPosition(depots[i]),getIntersectionPosition(deliveries[j].pickUp)});
            if(dist<x){
                x=dist;
                depotStart = depots[i];
                firstPickUp = deliveries[j].pickUp;
            }
        }
    }
    
    //Set all pickups that are done to -1
    for(int i=0;i<destinations.size();i++){
        if(destinations[i].first == firstPickUp){
            destinations[i].first = -1;
            del_index2 = i;
        }
    }
    //Pushback the first step
    tempath.start_intersection = depotStart;
    tempath.end_intersection = firstPickUp;
    tempath.subpath = findPathBetweenIntersections(turn,{depotStart,firstPickUp});
    path.push_back(tempath);
    
    start = firstPickUp;
    
    std::cout<<"reached first pickup: "<<start<<std::endl;
    
    pick2 = true;
    
    while(destinations.size()!=0){
         // std::cout<<"entered while loop"<<std::endl;   
        x = 999999999999;
        distP = x;
        distD = x;
        possibleP = 0;
        possibleD = 0;
        
        //Check pickup
        
        
      
       std::cout<<"start index is "<<del_index2<<std::endl;  
        std::cout<<"destinations size is : "<<destinations.size()<<std::endl;
        
       
        
        std::vector<end_point_info> start_point_potential = better_times[del_index2];
        
        if(del_index2 > num_pickups){
            for(int w = 0; w < num_pickups;++w){
                if(destinations[w].first == -1){
                    better_times[del_index2][w].time = INFINITY;
                }
                if(destinations[w].second == -1){
                    better_times[del_index2][w+num_pickups].time = INFINITY;
                }
                
            }
        }
        
        //if(destinations.size() != 1){
            auto min_it = std::min_element(start_point_potential.begin(), start_point_potential.end(), cmpBestTime);
       // }else{
          //  if(del_index2 <)
       // }
        
        end = min_it->end_point;
        std::cout<<"end_intersection is "<<end<<std::endl;
        
        
        del_index2 = std::distance(start_point_potential.begin(),min_it);
        std::cout<<"end index is : "<<del_index2<<std::endl;
        
        
       
        
        if (del_index2 < num_pickups){
            condCheck = 1;
            std::cout<<"cond check done pickup hai"<<std::endl;
            
            
        }else{
            condCheck = 2;
            std::cout<<"cond check done drop hai"<<std::endl;
        }
        
        
        
       /* for(int i=0; i<destinations.size();i++){
            if(destinations[i].first!=-1){
                double dist1 = findDistanceBetweenTwoPoints({getIntersectionPosition(start),getIntersectionPosition(destinations[i].first)});
                if(dist1<x){
                    x=dist1;
                    distP=dist1;
                    possibleP = destinations[i].first;
                }
            }
        }
        x=999999999999;
        //Check DropOff
        for (int i = 0; i < destinations.size(); i++) {
            if (destinations[i].first == -1) {
                if(destinations[i].first==-1 && destinations[i].second==-1) destinations.erase(destinations.begin() + i);
                else { 
                    double dist2 = findDistanceBetweenTwoPoints({getIntersectionPosition(start), getIntersectionPosition(destinations[i].second)});
                    if (dist2 < x) {
                        x = dist2;
                        distD = dist2;
                        possibleD = destinations[i].second;
                    }
                }
            }
        }
        
    
    //Check Which is closer
    if(distP<=distD){
        end=possibleP;
        condCheck=1;
    }
    else if(distD<distP){
        end=possibleD;
        condCheck=2;
    }
         */
    
        
    for(int i=0;i<destinations.size();i++){
        
        std::cout<<"inters are"<<destinations[i].first<<std::endl;
        std::cout<<"inters are"<<destinations[i].second<<std::endl;
         
        
        if(condCheck==1 && destinations[i].first == end){
            destinations[i].first=-1;
            std::cout<<"************1"<<std::endl;
        }
        else if(condCheck==2 && destinations[i].second==end && destinations[i].first==-1){
            destinations[i].second = -1;
            std::cout<<"************2"<<std::endl;
        }
        
       
    }

        tempath.start_intersection = start;
        tempath.end_intersection = end;
        tempath.subpath = min_it->path;
        path.push_back(tempath);
    
     
    for(int i=0;i<destinations.size();i++){
        if(destinations[i].first == -1 && destinations[i].second == -1){
            destinations.erase(destinations.begin()+i);
            //del_index2 = i;
           std::cout<<"i erased"<<std::endl;
        }
    }
        
    start=end;
        for (int i = 0; i < destinations.size(); i++) {
            if (destinations[i].first == -1 && destinations[i].second == -1) {
                destinations.erase(destinations.begin() + i);
                //del_index2 = i;
               std::cout<<"i erased"<<std::endl;
            }
        }
    
    if(del_index2 < num_pickups) {pick2 = true;
    //std::cout<<"pick2 is : "<<pick2<<std::endl;}
    }
    else {pick2 = false;
           // std::cout<<"pick2 is : "<<pick2<<std::endl;
    }
    
   // std::cout<<"while loop is fine"<<std::endl;
    
    }
    
    x=9999999999;
    
    for(int i=0; i<depots.size();i++){
            double dist = findDistanceBetweenTwoPoints({getIntersectionPosition(start),getIntersectionPosition(depots[i])});
            if(dist<x){
                x=dist;
                depotStart = depots[i];
            }
    }
    tempath.start_intersection=start;
    tempath.end_intersection=depotStart;
    tempath.subpath=findPathBetweenIntersections(turn,{start,depotStart});
    path.push_back(tempath);
    
    
//    for(int i=0;i<path.size();i++){
//        std::cout<<path[i].start_intersection<<std::endl;
//        std::cout<<path[i].end_intersection<<std::endl;
//    }
    
    better_times.clear();
    return path;
}


std::vector<locationInfo> setAllDeliveryInfo(const std::vector<DeliveryInf>& deliveries){
    
    std::vector<locationInfo> tempStruct;
    for(int i = 0; i< deliveries.size(); i++){
        tempStruct[i].pick = deliveries[i].pickUp;
        tempStruct[i].drop = deliveries[i].dropOff;
    }
    
    return tempStruct;
}




