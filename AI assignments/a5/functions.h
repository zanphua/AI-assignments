/*!*****************************************************************************
\file functions.h
\author Zandra Phua Si-Yu
\par DP email: p.siyuzandra@digipen.edu
\par Course: CSD3182
\par Section: B
\par Assignment 5 (Bellman-Ford's Search)
\date 08-06-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>
#include <string>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{
    const int null = -1;
    const int inf = INT_MAX;

    // An implementation of the Bellman-Ford algorithm. 
    // The algorithm finds the shortest path between a
    // starting node and all other nodes in the graph. 
    // The algorithm also detects negative cycles.
    template<int SIZE = 0>
     class BellmanFord
    {
        int* matrix; // the cost adjacency matrix
        int* distance;
        int* predecessor;

    public:

        BellmanFord(int* matrix = nullptr)
            : matrix{ matrix }, distance{ nullptr }, predecessor{ nullptr }
        {
            distance = new int[SIZE];
            predecessor = new int[SIZE];

            //*distance = SIZE;
            //*predecessor = SIZE;
         }

        ~BellmanFord()
        {
            delete distance; 
            delete predecessor;
        }

        // Runs the algorithm. Returns false when negative 
        // cycles are found, and true otherwise.
        bool run(int starting = 0)
        {
            int temp = SIZE;
            if (temp <= 0) return false;

            for (int i{ 0 }; i < SIZE; ++i)
            {
                // Initialize the distance to all nodes to be infinity
                distance[i] = inf;
                // Initialize predecessor array which will be used in shortest path
                // reconstruction after the algorithm has terminated.
                predecessor[i] = null;
            }

            // except for the start node which is zero.
            distance[starting] = 0;

            // For each node, apply relaxation for all the edges
            for (int k{ 0 }; k < SIZE; ++k)
            {
                int counter = 0; // Number of relaxations
                for (int j = 0; j < SIZE; ++j)
                    for (int i = 0; i < SIZE; ++i)
                    {
                        if ((j != i) && (distance[j] != inf) && 
                            (matrix[j * SIZE + i] != inf))
                        {
                            int new_dist = distance[j] + matrix[j * SIZE + i];
                            if (new_dist < distance[i])
                            {
                                // Relaxation
                                distance[i] = new_dist;
                                predecessor[i] = j;
                                counter++;
                            }
                        }
                    }
                if (counter == 0) // Stop when no more relaxation
                    return true;  // There is no negative cycles
            }

            return false; // There is a negative cycle
        }

        // Interpretation of the result that reconstructs the shortest path
        // from starting point to target
        std::vector<int> getPath(int target)
        {
            std::vector<int> path{};
            for (int i = target; predecessor[i] != null; i = predecessor[i])
            {
                path.emplace_back(i);
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        // Interpretation of the result that create a route (step-by-step 
        // description) of the shortest path from start to end with cost
        std::vector<std::array<int, 3>> getRoute(int target)
        {
            std::vector<std::array<int, 3>> route{};

            for (int i = target; predecessor[i] != null; i = predecessor[i])
            {
                std::array<int, 3> arr1 {predecessor[i], i, distance[i]};
                route.emplace_back(arr1);
            }

            for (size_t i = 0; i < route.size() - 1; ++i)
            {
                route[i][2] -= route[i + 1][2];
            }

            std::reverse(route.begin(), route.end());
            return route;
        }

        friend std::ostream& operator<<(std::ostream& os, const BellmanFord& rhs)
        {
            std::string s{ "[" };

            for (int i{ 0 }; i < SIZE; ++i)
            {
                s += (rhs.distance[i] == inf ? "inf" : std::to_string(rhs.distance[i])) + (i < SIZE - 1 ? "," : "");
            }


            s += "] [";


            for (int i{ 0 }; i < SIZE; ++i)
            {
                s += (rhs.predecessor[i] == -1 ? "null" : std::to_string(rhs.predecessor[i])) + (i < SIZE - 1 ? "," : "");
            }
            s += "]";
            os << s;

            return os;
        }
    };

} // end namespace

#endif