#ifndef REAL_MAP_LOGIC_H
#define REAL_MAP_LOGIC_H

#include <vector>
#include <string>
#include "cab_management.h"
using namespace std;

struct Route {
    std::vector<Location> waypoints;
    double totalDistance;  
    double estimatedDuration; 
    double estimatedFare; 
    bool isValid;
    
    Route();
};
    
    struct TripPhase {
        enum Type { IDLE, CAB_TO_PICKUP, PICKUP_TO_DROP, COMPLETED };
        Type type;
        Location pickup;
        Location drop;
        int assignedCabId;
        Route currentRoute;
        
        TripPhase();
};


class RealMapCabSystem {
private:
    CabManager cabManager;
    TripPhase currentTrip;
    double averageSpeed; // meters per second

public:
    explicit RealMapCabSystem(double speed = 18.0);

    // Fleet management
    void addCab(int id, Location position);
    void spawnCabsAround(Location center, int count, double radiusMeters);
   
    double calculateDistance(Location a, Location b);
    int findNearestCab(Location pickup);
    double calculateFare(double distanceKm, double durationMinutes);
    Route estimateRoute(Location from, Location to);


    // Core functionality
    bool startTrip(Location pickup);
    bool addDropLocation(Location drop);
    void completeTrip();
    
    TripPhase getCurrentTrip() const;
    vector<Cab> getCabs() const;
    int getAvailableCabsCount() const;

    int getAvailableCabsCount() const;

    void reset();
    string formatDuration(double seconds);
    string formatDistance(double meters);
    string formatCurrency(double amount);
};

#endif // REAL_MAP_LOGIC_H
