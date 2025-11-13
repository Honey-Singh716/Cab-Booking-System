#include <iostream>
#include "cab_management.h"
#include "real_map_logic.h"

using namespace std;

int main() {
    cout << "Cab Booking System - Demo" << endl;
    cout << "=========================" << endl;
    
    // Initialize the cab system
    RealMapCabSystem cabSystem;
    
    // Add some sample cabs
    cabSystem.addCab(1, {28.6139, 77.2090});  // New Delhi coordinates
    cabSystem.addCab(2, {28.6140, 77.2100});  // Nearby cab
    cabSystem.addCab(3, {28.6200, 77.2200});  // Slightly further away
    
    // Display available cabs
    cout << "\nAvailable cabs in the system:";
    // Note: You would need to implement a method to list cabs in RealMapCabSystem
    
    // Set up a sample trip
    Location pickup = {28.6139, 77.2090};  // Pickup location
    Location drop = {28.6300, 77.2200};    // Drop location
    
    cout << "\nStarting trip..." << endl;
    if (cabSystem.startTrip(pickup)) {
        cout << "Cab is on its way to pickup location!" << endl;
        
        // Add drop location
        if (cabSystem.addDropLocation(drop)) {
            cout << "Drop location set. Cab is on its way to the destination!" << endl;
            
            // Complete the trip
            cabSystem.completeTrip();
            cout << "Trip completed successfully!" << endl;
        } else {
            cout << "Failed to set drop location." << endl;
        }
    } else {
        cout << "Failed to find an available cab. Please try again later." << endl;
    }
    
    return 0;
}
