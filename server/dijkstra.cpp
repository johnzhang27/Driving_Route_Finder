/* --------------------------------------------------
*  Name: YongQuan Zhang
*  ID: 1515873
*  CMPUT 275, Winter 2020
*  Assignment 2 Part 1: Directions
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

using namespace std;

typedef pair<int, long long> PIL;

struct firetime {
	pair<long long,long long> vertexpair;
	int time;
};
void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree) {
    /* This function is for finding the shortest path between
    the start vertex to all of its neighbours. This function
    will be faster than what we discussed in lecture because 
    the using of binaryheap structure.

    Args: const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree

    Return: None, all structure will return by using pass by 
    reference.
    */
	pair<long long,long long> vertexpair(startVertex,startVertex);
	// ftime stands for fire burning time.
    HeapItem<pair<int,int>,long long> ftime;
	BinaryHeap<pair<int,int>,long long> events;
	events.insert(PIL(startVertex,startVertex),0);
	while(events.size() > 0){
        //(u,v),time
		ftime = events.min();
		events.popMin();
        // v which is item
		int v = ftime.item.second;
		int u = ftime.item.first;
        // This is the cost.
		long long time = ftime.key;
        // If we havn't visited this neighbour, then we insert
        // otherwise we ignore it.
		if(searchTree.find(v) == searchTree.end()){
			searchTree[v] = {u,time};
            // Create an iterator to go through all neighbours
            // v has.
			for(auto itr = graph.neighbours(v);
                itr != graph.endIterator(v);itr++){
				long long costVW = graph.getCost(v,*itr);
				events.insert(PIL(v,*itr),time+costVW);
			}
		}
	}
}