#include<bits/stdc++.h>
#include<cmath>
using namespace std;
#include "cab_management.h"
// Real-world map cab booking system logic
// Handles nearest cab finding, route computation, and trip management

struct Route {
    vector<Location> waypoints;
    double totalDistance;  // in meters
    double estimatedDuration;  // in seconds
    double estimatedFare;  // in local currency (e.g., USD)
    bool isValid;
    
    Route() : totalDistance(0.0), estimatedDuration(0.0), estimatedFare(0.0), isValid(false) {}
};

struct TripPhase {
    enum Type { IDLE, CAB_TO_PICKUP, PICKUP_TO_DROP, COMPLETED };
    Type type;
    Location pickup;
    Location drop;
    int assignedCabId;
    Route currentRoute;
    
    TripPhase() : type(IDLE), assignedCabId(-1) {}
};
class RealMapCabSystem {
private:
    CabManager cabManager;
    TripPhase currentTrip;
    double averageSpeed; // meters per second (default ~18 m/s = ~65 km/h)
    
public:
    RealMapCabSystem(double speed = 18.0) : averageSpeed(speed) {}
    
    // Add cabs to the system
    void addCab(int id, Location position) {
        cabManager.addCab(id, position);
    }
    
    // Spawn cabs randomly around a center point
    void spawnCabsAround(Location center, int count, double radiusMeters) {
        cabManager.reset();
        
        // Convert meters to degrees (approximate)
        double metersPerDegreeLat = 111320.0;
        double metersPerDegreeLng = cos(center.latitude * M_PI / 180.0) * 111320.0;
        
        for (int i = 0; i < count; i++) {
            // Generate random position within radius
            double angle = (rand() % 360) * M_PI / 180.0;
            double distance = (rand() % (int)radiusMeters);
            
            double latOffset = (distance * cos(angle)) / metersPerDegreeLat;
            double lngOffset = (distance * sin(angle)) / metersPerDegreeLng;
            
            Location cabPos(center.latitude + latOffset, center.longitude + lngOffset);
            addCab(i + 1, cabPos);
        }
    }
    
    // Calculate straight-line distance between two points (in meters)
    double calculateDistance(Location a, Location b) {
        const double R = 6371000.0; // Earth's radius in meters
        
        double lat1Rad = a.latitude * M_PI / 180.0;
        double lat2Rad = b.latitude * M_PI / 180.0;
        double deltaLatRad = (b.latitude - a.latitude) * M_PI / 180.0;
        double deltaLngRad = (b.longitude - a.longitude) * M_PI / 180.0;
        
        double a1 = sin(deltaLatRad / 2) * sin(deltaLatRad / 2) +
                   cos(lat1Rad) * cos(lat2Rad) *
                   sin(deltaLngRad / 2) * sin(deltaLngRad / 2);
        double c = 2 * atan2(sqrt(a1), sqrt(1 - a1));
        
        return R * c;
    }
    
    // Find nearest available cab to pickup location
    int findNearestCab(Location pickup) {
        return cabManager.findNearestAvailable(pickup);
    }
    
    // Calculate fare based on distance (in km) and time (in minutes)
    double calculateFare(double distanceKm, double durationMinutes) {
        const double BASE_FARE = 2.50;  // Base fare in local currency
        const double PER_KM_RATE = 1.20;  // Rate per kilometer
        const double PER_MINUTE_RATE = 0.20;  // Rate per minute
        const double MINIMUM_FARE = 5.00;  // Minimum fare
        
        double distanceFare = distanceKm * PER_KM_RATE;
        double timeFare = durationMinutes * PER_MINUTE_RATE;
        double totalFare = BASE_FARE + distanceFare + timeFare;
        
        return max(totalFare, MINIMUM_FARE);
    }
    
    // Estimate route distance, duration, and fare (simplified - assumes road distance is 1.3x straight line)
    Route estimateRoute(Location from, Location to) {
        Route route;
        route.waypoints = {from, to};
        
        double straightDistance = calculateDistance(from, to);
        route.totalDistance = straightDistance * 1.3; // Road distance factor
        route.estimatedDuration = route.totalDistance / averageSpeed;
        route.estimatedFare = calculateFare(route.totalDistance / 1000.0, route.estimatedDuration / 60.0);
        route.isValid = true;
        
        return route;
    }
    
    // Start a new trip
    bool startTrip(Location pickup) {
        if (currentTrip.type != TripPhase::IDLE) return false;
        
        int cabId = findNearestCab(pickup);
        if (cabId == -1) return false;
        
        // Find the cab object
        Cab assignedCab;
        if (!cabManager.getCab(cabId, assignedCab)) return false;
        
        // Set up trip
        currentTrip.type = TripPhase::CAB_TO_PICKUP;
        currentTrip.pickup = pickup;
        currentTrip.assignedCabId = cabId;
        currentTrip.currentRoute = estimateRoute(assignedCab.position, pickup);
        
        // Mark cab as unavailable
        cabManager.setAvailability(cabId, false);
        
        return true;
    }
    
    // Add drop location to existing trip
    bool addDropLocation(Location drop) {
        if (currentTrip.type != TripPhase::CAB_TO_PICKUP) return false;
        
        currentTrip.drop = drop;
        currentTrip.type = TripPhase::PICKUP_TO_DROP;
        currentTrip.currentRoute = estimateRoute(currentTrip.pickup, drop);
        
        return true;
    }
    
    // Complete the trip
    void completeTrip() {
        if (currentTrip.assignedCabId != -1) {
            // Mark cab as available again and update its position
            cabManager.setAvailability(currentTrip.assignedCabId, true);
            Location newPos = (currentTrip.type == TripPhase::PICKUP_TO_DROP) ? currentTrip.drop : currentTrip.pickup;
            cabManager.updatePosition(currentTrip.assignedCabId, newPos);
        }
        
        currentTrip = TripPhase(); // Reset trip
    }

    // abhishek part above 
    
    // Get current trip information
    TripPhase getCurrentTrip() const {
        return currentTrip;
    }
    
    // Get all cabs
    vector<Cab> getCabs() const {
        return cabManager.listCabs();
    }
    
    // Get available cabs count
    int getAvailableCabsCount() const {
        return cabManager.countAvailable();
    }
    
    // Reset the system
    void reset() {
        cabManager.reset();
        currentTrip = TripPhase();
    }
    
    // Format time duration
    string formatDuration(double seconds) {
        int minutes = (int)(seconds / 60);
        int hours = minutes / 60;
        minutes = minutes % 60;
        
        if (hours > 0) {
            return to_string(hours) + "h " + to_string(minutes) + "m";
        } else {
            return to_string(minutes) + "m";
        }
    }
    
    // Format distance
    string formatDistance(double meters) {
        if (meters >= 1000) {
            return to_string(round(meters / 100) / 10) + " km";
        } else {
            return to_string((int)meters) + " m";
        }
    }
    
    // Format currency
    string formatCurrency(double amount) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "$%.2f", amount);
        return string(buffer);
    }
};
