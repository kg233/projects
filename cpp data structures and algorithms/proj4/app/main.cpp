// main.cpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
#include "Digraph.hpp"
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include "iostream"
#include <math.h> 

namespace {
	double distance(const RoadSegment& roadSeg){
		return roadSeg.miles;
	}
	double ttime(const RoadSegment& roadSeg){
		return roadSeg.miles/roadSeg.milesPerHour;
		
	}

	std::vector<int> parseMap(std::map<int, int> result, int startVertex, int endVertex){
		std::vector<int> parsed;
		int stored = endVertex;
		while (stored != startVertex){
			parsed.push_back(stored);
			stored = result.at(stored);
		}
		return parsed;
	}

	void timePrinter(double time_in_sec){
		int stem = int(time_in_sec);
		std::string result;


		int hour = stem/3600;
		if (hour != 0){
			std::cout << hour << " hours ";
		}
		time_in_sec -= hour*3600;

		stem = int(time_in_sec);

		int minutes = stem/60;
		if (hour != 0 or minutes != 0){
			std::cout << minutes << " minutes ";
		}

		time_in_sec -= minutes*60;

		std::cout << time_in_sec << " seconds ";

	}

	void outputDist(RoadMap map, std::vector<int> parsed, int startVertex, int endVertex){
		//Shortest distance from 1st St & 101st Ave to 3rd St & 103rd Ave:
		double totalDist = 0;
		std::cout << "Shortest distance from " << map.vertexInfo(startVertex) << " to " << map.vertexInfo(endVertex) << ":" << std::endl;
		std::cout << "\tBegin at " << map.vertexInfo(startVertex) << std::endl;
		for (auto i = parsed.rbegin(); i != parsed.rend(); i++){
			double distance;
			if (i == parsed.rbegin()){
				distance = map.edgeInfo(startVertex, *i).miles;
			}
			else{
				distance = map.edgeInfo(*(i-1), *i).miles;
			}
			totalDist += distance;
			std::cout << "\tContinue to " << map.vertexInfo(*i) <<" ("<<distance << " miles)" << std::endl;
		}
		std::cout << "Total Distance: " << totalDist << " miles\n" << std::endl;
	}

	void outputTime(RoadMap map, std::vector<int> parsed, int startVertex, int endVertex){
		//totalTime is in seconds
		double totalTime = 0;
		std::cout << "Shortest driving time from " << map.vertexInfo(startVertex) << " to " << map.vertexInfo(endVertex) << ":" << std::endl;
		std::cout << "\tBegin at " << map.vertexInfo(startVertex) << std::endl;
		for (auto i = parsed.rbegin(); i != parsed.rend(); i++){
			double distance;
			double mph;
			if (i == parsed.rbegin()){
				distance = map.edgeInfo(startVertex, *i).miles;
				mph = map.edgeInfo(startVertex, *i).milesPerHour;
			}
			else{
				distance = map.edgeInfo(*(i-1), *i).miles;
				mph = map.edgeInfo(*(i-1), *i).milesPerHour;
			}
			double time_in_sec = distance/mph*3600.0;

			totalTime += time_in_sec;
			// std::cout << time_in_sec << std::endl;
			std::cout << "\tContinue to " << map.vertexInfo(*i) <<" ("<<distance << " miles & " << mph << "mph = ";
			timePrinter(time_in_sec);
			std::cout << ")"<<std::endl;

		}
		std::cout << "Total time: ";
		timePrinter(totalTime);
		std::cout << "\n" << std::endl;
	}



}



int main()
{
	std::map<int, std::map<int, int>> storedShortestDist;
	std::map<int, std::map<int, int>> storedShortestTime;

	InputReader reader{std::cin};
	RoadMapReader mapReader;
	RoadMap map = mapReader.readRoadMap(reader);

	if (not map.isStronglyConnected()){
		std::cout << "Disconnected Map" << std::endl;
		return 0;
	}

	TripReader tReader;
	std::vector<Trip> tripVec = tReader.readTrips(reader);

	for (int i = 0; i < tripVec.size(); i++){
		int fromVertex = tripVec[i].startVertex;
		int toVertex = tripVec[i].endVertex;
		if (tripVec[i].metric == TripMetric::Distance){
			std::map<int, int> result;
			if (storedShortestDist.count(fromVertex) != 0){
				result = storedShortestDist[fromVertex];
			}
			else{
				result = map.findShortestPaths(fromVertex, distance);
				storedShortestDist[fromVertex] = result;
			}

			outputDist(map, parseMap(result, fromVertex, toVertex), fromVertex, toVertex);
		}
		else if (tripVec[i].metric == TripMetric::Time){
			// std::cout <<"hey" <<std::endl;
			std::map<int, int> result;
			if (storedShortestTime.count(fromVertex) != 0){
				result = storedShortestTime[fromVertex];
			}
			else{
				result = map.findShortestPaths(fromVertex, ttime);
				storedShortestTime[fromVertex] = result;
			}
			outputTime(map, parseMap(result, fromVertex, toVertex), fromVertex, toVertex);
		}
	}
	// Digraph<double , double> x;
	// x.addVertex(1, 2);
	// x.addVertex(2, 3);
	// x.addEdge(1,2,4);
	// x.addEdge(2,1,5);
	// std::cout << x.isStronglyConnected() << std::endl;




































	// Digraph<std::string, std::string> x;
	// Digraph<int, double> x;

	// x.addVertex(1, 1);
	// x.addVertex(2, 2);
	// x.addVertex(3, 3);
	// x.addVertex(4, 4);
	// x.addVertex(5, 5);
	// x.addVertex(6, 6);
	// x.addVertex(7, 7);
	// x.addVertex(8, 8);
	// x.addVertex(9, 9);
	// x.addVertex(10, 10);


	// x.addEdge(1, 10, 27.0);
	// x.addEdge(6, 10, 5.0);
	// x.addEdge(3, 10, 7.0);
	// x.addEdge(4, 10, 5.0);
	// x.addEdge(4, 4, 5.0);
	// x.addEdge(1, 6, 3.0);
	// x.addEdge(1, 3, 11.0);
	// x.addEdge(1, 4, 7.0);
	// x.addEdge(1, 7, 1.0);
	// x.addEdge(7, 4, 1.0);
	// x.addEdge(5, 6, 13.0);
	// x.addEdge(5, 7, 17.0);
	// x.addEdge(6, 7, 7.0);

	// x.visualize();

	// auto z = x.findShortestPaths(1, [](const double& value){return value;});

	// for (auto i = z.begin(); i != z.end(); i++){
	// 	std::cout << i->first << ": " << i->second <<std::endl;
	// }


	// x.visualize();
	// x.addVertex(1, "first node");

	// x.addVertex(2, "second node");

	// x.addVertex(3, "third node");

	// x.addVertex(4, "fourth node");

	// x.addEdge(1,2, "1 -> 2");
	// x.addEdge(2,1, "2 -> 1");
	// x.addEdge(2,2, "2 -> 2");
	// x.addEdge(2,3, "2 -> 3");
	// x.addEdge(3,4, "3 -> 4");
	// // x.addEdge(4, 2, "4 -> 2");
	// x.addEdge(1,3, "1 -> 3");
	// std::cout<< x.isStronglyConnected() << std::endl;
	// Digraph<std::string, std::string> y;
	// y.addVertex(1, "random");

	// x = y;
	// // x.removeEdge(1, 2);
	// // x.removeEdge(2, 1);
	// // x.removeEdge(2, 2);
	// // x.removeEdge(2, 3);
	// // x.removeEdge(3, 4);
	// // x.removeEdge(4, 2);
	// // x.removeEdge(1, 3);
	// // x.removeVertex(1);
	// // x.removeVertex(2);
	// // // x.removeVertex(3);
	// // x.removeVertex(4);
	// // x.removeVertex(2);
	// // x.removeEdge(2, 3);
	// // auto i = x.vertices();
	// // for (int a = 0; a < i.size(); a++){
	// // 	std::cout<< x.vertexInfo(9) <<std::endl;
	// // }
	// // std::cout << x.edgeInfo(2,4) <<std::endl;
	
	// y.addVertex(3,"4");
	// // x.visualize();
	// y.visualize();
	// x.visualize();

    return 0;
}

