/* --------------------------------------------------
*  Name: YongQuan Zhang
*  ID: 1515873
*  CMPUT 275, Winter 2020
*  Assignment 2 Part 1: Directions
*---------------------------------------------------*/
#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "wdigraph.h"
#include <utility>
#include <unordered_map>

using namespace std;

typedef pair<int, long long> PIL;

typedef pair<int, PIL> PIPIL;

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

#endif
