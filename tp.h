#ifndef TRAVELPLANNER
#define TRAVELPLANNER

#include "provided.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>

class TravelPlanner : public TravelPlannerBase {
private:
    std::vector<std::string> preferred;

    struct PathNode {
        std::vector<FlightSegment> path; // Sequence of flights taken
        std::string currentAirport; // Current airport code
        int arrivalTime; // Arrival time at current airport
        bool operator<(const PathNode& other) {
            return arrivalTime < other.arrivalTime;
        }
    };

public:
    TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db)
        : TravelPlannerBase(flight_manager, airport_db) {
    }

    void add_preferred_airline(std::string airline) {
        preferred.push_back(airline);
    }

    bool plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const {
        std::queue<PathNode> bfs;
        std::unordered_map<std::string, int> visited;

        // Start BFS with flights departing from the source airport
        std::vector<FlightSegment> initialFlights = get_flight_manager().find_flights(source_airport, start_time, start_time + get_max_layover());
        std::vector<PathNode> finalFlights;
        for (const FlightSegment& seg : initialFlights) {
            if (preferred.empty() || std::find(preferred.begin(), preferred.end(), seg.airline) != preferred.end()) {
                bfs.push({ {seg}, seg.destination_airport, seg.departure_time + seg.duration_sec });
            }
        }

        while (!bfs.empty()) {
            PathNode node = bfs.front();
        
            bfs.pop();

            // If we reach the destination, construct the itinerary and return
            if (node.currentAirport == destination_airport) {
                finalFlights.push_back(node);
                continue;
            }

            int minDeparture = node.arrivalTime + get_min_connection_time();
            int maxDeparture = node.arrivalTime + get_max_layover();
            std::vector<FlightSegment> nextFlights = get_flight_manager().find_flights(node.currentAirport, minDeparture, maxDeparture);
            
            for (FlightSegment& seg : nextFlights) {
                int newArrivalTime = seg.departure_time + seg.duration_sec;
                
                bool isPreferred = preferred.empty() || (std::find(preferred.begin(), preferred.end(), seg.airline) != preferred.end());

                // Prevent revisiting an airport unless we arrive sooner
                if (isPreferred && (!visited.count(seg.destination_airport) || newArrivalTime < visited[seg.destination_airport])) {
                    visited[seg.destination_airport] = newArrivalTime;
                    PathNode newNode = node;
                    newNode.path.push_back(seg);
                    newNode.currentAirport = seg.destination_airport;
                    newNode.arrivalTime = newArrivalTime;
                    //std::cout << "pushing " << seg.airline << " flight from " << seg.source_airport << " to " << seg.destination_airport << std::endl;
                    bfs.push(newNode); 
                }
            }
           
        }

        if (!finalFlights.empty()) {
            PathNode soonestArrival = *std::min_element(finalFlights.begin(), finalFlights.end());
            itinerary.flights = soonestArrival.path;
            itinerary.source_airport = source_airport;
            itinerary.destination_airport = destination_airport;
            if (soonestArrival.arrivalTime - start_time > get_max_duration())
                return false;
            itinerary.total_duration = soonestArrival.arrivalTime - start_time;
            return true;

        }
        return false; // No valid itinerary found
    }
};

#endif // TRAVELPLANNER
