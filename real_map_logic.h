#ifndef REAL_MAP_LOGIC_H
#define REAL_MAP_LOGIC_H

#include <vector>
#include <string>

struct Location {
    double latitude;
    double longitude;
    
    Location(double lat = 0.0, double lng = 0.0) 
        : latitude(lat), longitude(lng) {}
};

struct Cab {
    int id;
    Location position;
    bool available;
    
    Cab(int id, double lat, double lng) //cab id and location details 
        : id(id), position(lat, lng), available(true) {}
};

class MapManager {
public:
    // Initializing with default location (by default New Delhi)
    MapManager();
    
    // Finding nearest available cab to a location
    int findNearestCab(const Location& location);
    
    // Calculating route between two points
    std::vector<Location> calculateRoute(const Location& from, const Location& to);
    
    // Updating cab positions 
    void updateCabPositions();
    
private:
    std::vector<Cab> cabs;
    void initializeCabs(int count, const Location& center, double radius);
};

#endif // real_map_logic.h