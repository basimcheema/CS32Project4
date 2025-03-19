#ifndef FLIGHTMANAGER
#define FLIGHTMANAGER

#include "provided.h"
#include "bstset.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

class FlightManager : public FlightManagerBase {
private:
    struct FlightSegmentWrapper {
    FlightSegmentWrapper(std::string airline, int flightNum, std::string source, std::string dest, int depTime, int duration) : 
        fs(airline, flightNum, source, dest, depTime, duration) { }
    FlightSegment fs;
    bool operator<(const FlightSegmentWrapper& other) const {
        if (fs.departure_time != other.fs.departure_time) {
            return fs.departure_time < other.fs.departure_time;  // Primary sort by departure time
        }
        return fs.flight_no < other.fs.flight_no;  //  Secondary sort by flight number
    }

    bool operator==(const FlightSegmentWrapper other) const {
        return fs.airline == other.fs.airline && fs.source_airport == other.fs.source_airport &&
            fs.destination_airport == other.fs.destination_airport && fs.departure_time == other.fs.departure_time && fs.duration_sec == other.fs.duration_sec;
    }
};
    std::unordered_map<std::string, BSTSet<FlightSegmentWrapper>> flightsByAirport;

public:
	//FlightManager();
	//~FlightManager();
	bool load_flight_data(std::string filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string airline, source, destination, flightNumStr, depTimeStr, arrTimeStr, durationStr;

            if (!std::getline(ss, airline, ',') ||
                !std::getline(ss, flightNumStr, ',') ||
                !std::getline(ss, source, ',') ||
                !std::getline(ss, destination, ',') ||
                !std::getline(ss, depTimeStr, ',') ||
                !std::getline(ss, arrTimeStr, ',') ||
                !std::getline(ss, durationStr, ',')) {
                continue; 
            }
            flightsByAirport[source].insert(FlightSegmentWrapper(airline, std::stoi(flightNumStr), source, destination, std::stoi(depTimeStr), std::stoi(durationStr)));
        }

        file.close();
        return true;
	}
    std::vector<FlightSegment> find_flights(std::string source, int start_time, int end_time) const {
        std::vector<FlightSegment> flights;

        // Ensure airport exists in the map
        if (!flightsByAirport.count(source)) return flights;

        const BSTSet<FlightSegmentWrapper>* tree = &flightsByAirport.at(source);

        FlightSegmentWrapper smallest("", 0, "", "", start_time, 0);

        BSTSet<FlightSegmentWrapper>::SetIterator it = tree->find_first_not_smaller(smallest);
        
        const FlightSegmentWrapper* seg = it.get_and_advance();
        while (seg != nullptr) {

            //  Stop if the departure time exceeds the limit
            if (seg->fs.departure_time >= end_time) {
                break;
            }
            
            flights.push_back(seg->fs);

            seg = it.get_and_advance();

        }

        return flights; //  Return the valid flights
    }


};



#endif //FLIGHTMANAGER