#ifndef REAL_MAP_LOGIC_H
#define REAL_MAP_LOGIC_H

#include <vector>
#include "cab_management.h"


class CabManager;

struct Route {
    std::vector<Location> waypoints;
    double totalDistance;  
    double estimatedDuration; 
    double estimatedFare; 
    bool isValid;
    
    Route();
};

class RealMapCabSystem {
private:
    CabManager cabManager;
    
    struct TripPhase {
        enum Type { IDLE, CAB_TO_PICKUP, PICKUP_TO_DROP, COMPLETED };
        Type type;
        Location pickup;
        Location drop;
        int assignedCabId;
        Route currentRoute;
        
        TripPhase();
    } currentTrip;
    
    double averageSpeed; 
    
   
    double calculateDistance(Location a, Location b);
    int findNearestCab(Location pickup);
    double calculateFare(double distanceKm, double durationMinutes);
    Route estimateRoute(Location from, Location to);
    
public:
    RealMapCabSystem();
    
    // Core functionality
    bool addCab(int id, Location position, bool available = true);
    bool startTrip(Location pickup);
    bool addDropLocation(Location drop);
    void completeTrip();
    
   
    int getAvailableCabsCount() const;
    
    
    void demo();
};

#endif // REAL_MAP_LOGIC_H
