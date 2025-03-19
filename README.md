# CS 32 @ UCLA - Project 4: Flight Planner

## Overview
This repository contains my implementation of Project 4: Flight Planner for CS 32 at UCLA, Winter 2025, taught by Professor Smallberg. The project focuses on planning flight itineraries using a custom data structure and algorithms.

## Code Files
The main implementation can be found in the following files:
- `bstset.h`
- `fm.h`
- `tp.h`

## Testing Instructions
To test the flight planner, modify the `params.txt` file according to the following format:

```
all_flights.txt  # Flight data file
SNA              # Starting airport
YYZ              # Destination airport
1736284400       # Unix timestamp for start of journey
24               # Maximum trip duration (hours)
1                # Minimum layover time (hours)
4                # Maximum layover time (hours)
Air Canada       # Preferred airline (OPTIONAL, can add more on new lines)
```

Make sure `all_flights.txt` contains the necessary flight data for accurate itinerary generation.

## Notes
- Ensure the flight data file is formatted correctly before running tests.
- The program supports airline preferences, which can be listed on separate lines.

## License
This project was created for educational purposes as part of CS 32 at UCLA.
