/* --------------------------------------------------
*  Name: YongQuan Zhang
*  ID: 1515873
*  CMPUT 275, Winter 2020
*  Assignment 2 Part 2: Directions
*---------------------------------------------------*/
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <fstream>
#include <utility> 
#include <string>
#include "digraph.h"
#include "wdigraph.h"
#include "dijkstra.h"
#include "heap.h"
#include "serialport.h"

using namespace std;

typedef pair<int, long long> PIL;

struct Point{
	long long lat;
	long long lon;
};

long long manhattan(const Point& pt1, const Point& pt2){
	long long distance;
	distance = abs(pt1.lat-pt2.lat)+abs(pt1.lon-pt2.lon);
	return distance;
}

void readGraph(string filename, WDigraph& graph, 
	unordered_map<int, Point>& points){
	/* This function is for fetching all the Vertex information
	and edge information from a plain text input. Then add all
	information we fetched out to the wdigraph and points
	structure. 

	Args: char filename[], WDigraph& graph, 
	unordered_map<int, Point>& points

	Return: None, all structure will return by using pass by 
	reference.
	*/
	ifstream file;
	string linefromfile;
	file.open(filename);
	while(getline(file,linefromfile)){
		if(linefromfile.find("V,") != string::npos){
			// create a substring start from the ID number to
			// end of the previous string.
			string substrV = linefromfile.substr(2);
			int pos_1 = substrV.find(",");
			// Create another sub string start from ID to end
			// before the 2nd comma.
			string ID = substrV.substr(0,pos_1);
			int intID = stoi(ID);
			graph.addVertex(intID);
			// create a substring for latitude.
			string substrV_lat = substrV.substr(pos_1+1);
			int pos_lat = substrV_lat.find(",");
			string lat = substrV_lat.substr(0,pos_lat);
			double doublelat = stod(lat);
			long long intlat = static_cast<long long> (doublelat*100000);
			// create the substring for longtitude.
			string lon = substrV_lat.substr(pos_lat+1);
			double doublelon = stod(lon);
			long long intlon = static_cast<long long> (doublelon*100000);
			// Insert the ID and its lat and lon to points.
			points[intID] = {intlat, intlon};
		}
		else if(linefromfile.find("E,") != string::npos){
			string substrE_1 = linefromfile.substr(2);
			int pos_1 = substrE_1.find(",");
			// Start ID substring.
			string ID_E_1 = substrE_1.substr(0,pos_1);
			int intID_1 = stoi(ID_E_1);
			string substrE_2 = substrE_1.substr(pos_1+1);
			int pos_2 = substrE_2.find(",");
			// End ID substring.
			string ID_E_2 = substrE_2.substr(0,pos_2);
			int intID_2 = stoi(ID_E_2);
			long long distance = manhattan(points[intID_1],points[intID_2]);
			// directed graph.
			graph.addEdge(intID_1,intID_2,distance);
		}
	}
	file.close();
}

int findClosestVertex(const Point inputPoint,
	const unordered_map<int, Point>& points){
	/* This function is used to find the closest vertice to
	your input lat and lon.
	Args: 
		const Point inputPoint, 
		const unordered_map<int, Point>& points
	
	Returns: 
		closeID
	*/
	long long closetdistance;
	int closeID;
	auto itr = points.begin();
	Point lat_lon = itr->second;
	closetdistance = manhattan(inputPoint,lat_lon);
	closeID = itr->first;
	for (auto s: points){
		lat_lon = s.second;
		long long distance = manhattan(inputPoint,lat_lon);
		if(distance < closetdistance){
			closetdistance = distance;
			closeID = s.first;
		}
	}
	return closeID;
}

int main(){
	/*Used to execute all other functions
		Args: 
			None

		Return: 
			0/ None (0 is a format we dont need it to 
			return anything).
	*/
	WDigraph graph;
	unordered_map<int, Point> points;
	unordered_map<int, PIL> searchTree;
	readGraph("edmonton-roads-2.0.1.txt",graph,points);
	bool request = true;
	int closeID1,closeID2;
	long long lat1,lon1,lat2,lon2;
	long long closetdistance;
	SerialPort Serial("/dev/ttyACM0");
	int size;
	int pathID;
	int index = 0;
	int indexOfSpace;
	string readFromSerial;
	vector<int> path;
	while(true){
        readFromSerial = Serial.readline();
		if(readFromSerial.find("R ") != string::npos){
			Point inputPoint1, inputPoint2;
			index = 0;
			searchTree.clear();
			// all lat and lon forr start and end.
			string substr_start_end = readFromSerial.substr(2);
			// 2nd space.
			indexOfSpace = substr_start_end.find(" ");
			// substring for start lat.
			string substr_start_lat = substr_start_end.substr(0,indexOfSpace);
			string substr_start_lon_end = substr_start_end.substr(indexOfSpace+1);
			//3rd space.
			indexOfSpace = substr_start_lon_end.find(" ");
			string substr_start_lon = substr_start_lon_end.substr(0,indexOfSpace);
			string substr_end = substr_start_lon_end.substr(indexOfSpace+1);
			//4th space.
			indexOfSpace = substr_end.find(" ");
			string substr_end_lat = substr_end.substr(0,indexOfSpace);
			string substr_end_lon = substr_end.substr(indexOfSpace+1);
			inputPoint1.lat = stoll(substr_start_lat);
			inputPoint1.lon = stoll(substr_start_lon);
			inputPoint2.lat = stoll(substr_end_lat);
			inputPoint2.lon = stoll(substr_end_lon);
			// start vertex
			closeID1 = findClosestVertex(inputPoint1,points);
			// end vertex
			closeID2 = findClosestVertex(inputPoint2,points);
			// Call dijkstra to find the searchtree and the 
			// shortest path.
			dijkstra(graph, closeID1, searchTree);
			if(searchTree.find(closeID2) == searchTree.end()){
				// No path.
				Serial.writeline("N 0\n");
			}
			else{ 
				path.clear();
				while(closeID2 != closeID1){
					// Insert backward.
					path.insert(path.begin(),closeID2);
					// Update closeID2, since the search tree
					// contains a lot of pair structures like: 
					// (child,(parent,distance)).
					// So by updating the child vertice we could 
					// find the path from the end vertex to start
					// vertex.
					closeID2 = searchTree[closeID2].first;
				}
				// The condition of the while loop is 
				// closeID2 != closeID1.
				// So we need to insert the start vertex.
				path.insert(path.begin(),closeID1);
				// The size of the vector is how many vertice
				// we travelled.
				size = path.size();
				Serial.writeline("N");
				Serial.writeline(" ");
				Serial.writeline(to_string(size));
				Serial.writeline("\n");
				cout << "Sending: N " << size << endl;
			}
		}
		else if(readFromSerial.find("A\n") != string::npos){
			if(index > size-1){
				Serial.writeline("E\n");
				cout << "Sending: E" << endl;
			}
			else{
				Serial.writeline("W");
				Serial.writeline(" ");
				Serial.writeline(to_string(points[path[index]].lat));
				Serial.writeline(" ");
				Serial.writeline(to_string(points[path[index]].lon));
				Serial.writeline("\n");
				cout << "Sending: W " << points[path[index]].lat 
				<< " " << points[path[index]].lon << endl;
				index++;
			}
		}
	}
	return 0;
}