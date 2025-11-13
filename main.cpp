#include <iostream>
#include "cab_management.h"
#include "real_map_logic.h"

using namespace std;

int main() {
    cout << "Cab Booking System - Demo" << endl;
    cout << "=========================" << endl;
    
    // Bro... initializing the cab system like a responsible adult
    RealMapCabSystem cabSystem;
    
    // Adding some sample cabs because empty system = no rides = sad life
    cabSystem.addCab(1, {28.6139, 77.2090});  // New Delhi coordinates (capital flex)
    cabSystem.addCab(2, {28.6140, 77.2100});  // This cab is literally your neighbour
    cabSystem.addCab(3, {28.6200, 77.2200});  // This guy is slightly far, probably stuck in traffic
    
    // Displaying available cabs (yeh feature baadme banaoge)
    cout << "\nAvailable cabs in the system:";
    // TODO: List cabs properly... not like this "trust me bro" version
    
    // Setting up a sample trip because demo ka koi respect hi nahi
    Location pickup = {28.6139, 77.2090};  // Pickup location (same as cab 1 ni?)
    Location drop = {28.6300, 77.2200};    // Drop location (thoda door but manageable)
    
    cout << "\nStarting trip..." << endl;
    if (cabSystem.startTrip(pickup)) {
        cout << "Cab is on its way to pickup location!" << endl;
        
        // Drop location set karte waqt thoda nervousness
        if (cabSystem.addDropLocation(drop)) {
            cout << "Drop location set. Cab is on its way to the destination!" << endl;
            
            // Anddd... trip complete. MVP moment.
            cabSystem.completeTrip();
            cout << "Trip completed successfully!" << endl;
        } else {
            cout << "Failed to set drop location." << endl;
        }
    } else {
        // Life moment when no cab is available and you question your existence
        cout << "Failed to find an available cab. Please try again later." << endl;
    }
    
    return 0;
}
