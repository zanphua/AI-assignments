/*!*****************************************************************************
\file functions.h
\author Zandra Phua Si-Yu
\par DP email: p.siyuzandra@digipen.edu
\par Course: CSD3182
\par Section: B
\par Assignment 4 (Dijkstra's Search)
\date 01-06-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"
#include <algorithm>

#define UNUSED(x) (void)x;

namespace AI 
{

    // Domain specific functor that returns adjacent nodes
    class GetMapAdjacents : public GetAdjacents
    {
        int* map; // the map with integers where 0 means an empty cell
        int size; // width and hight of the map in elements

    public:

        GetMapAdjacents(int* map = nullptr, int size = 0)
            : GetAdjacents(), map{ map }, size{ size }
        {
        }

        int getMapSize()
        {
            return this->size;
        }

        std::vector<AI::Node*> operator()(Key key)
        {
            std::vector<AI::Node*> list = {};

            // Find and return all empty adjacent cells
            int j = key[0];
            int i = key[1];

            if (j >= 0 && j < this->size && i >= 0 && i < this->size)
            {
                if (i > 0 && this->map[j * this->size + i - 1] == 0)
                    list.emplace_back(new Node(Key{ j, i - 1 }, 10, 'W'));
                if (i < this->size - 1 && this->map[j * this->size + i + 1] == 0)
                    list.emplace_back(new Node({ j, i + 1 }, 10, 'E'));
                if (j > 0 && this->map[(j - 1) * this->size + i] == 0)
                    list.emplace_back(new Node({ j - 1, i }, 10, 'N'));
                if (j < this->size - 1 && this->map[(j + 1) * this->size + i] == 0)
                    list.emplace_back(new Node({ j + 1, i }, 10, 'S'));
            }

            return list;
        }
    };

    class Dijkstras
    {
        GetAdjacents* pGetAdjacents;

    public:

        Dijkstras(GetAdjacents* pGetAdjacents)
            : pGetAdjacents(pGetAdjacents)
        {
        }

        // starting and target are arrays of 2 elements [j, i] that define positions on the map
        std::vector<char> run(Key starting, Key target)
        {
            HashTable* closedlist = new HashTable();
            PriorityQueue openlist;

            Node* pCurrent = nullptr;
            
            // Implement the search
            openlist.push(new Node(starting));

            while (!openlist.empty())
            {
                pCurrent = openlist.pop();

                closedlist->add(pCurrent->key, pCurrent);

                if (pCurrent->key == target) break;

                //static cast to get map size
                GetMapAdjacents* gma = static_cast<GetMapAdjacents*>(pGetAdjacents);

                int size = gma->getMapSize();

                //if target is out of the map size
                if (target[0] > size || target[1] > size)
                {
                    break;
                }

                std::vector<AI::Node*> adjacents = pGetAdjacents->operator()(pCurrent->key);

                for (AI::Node* i: adjacents )
                {
                    if (!closedlist->find(i->key))
                    {
                        Node* openlist_found = openlist.find(i->key);
                        if (!openlist_found)
                        {
                            openlist.push(new Node(i->key, pCurrent->g + i->g, i->info, pCurrent));
                        }
                        else
                        {
                            int temp = pCurrent->g + i->g;

                            if (temp < openlist_found->g)
                            {
                                openlist_found->parent = pCurrent;
                                openlist_found->info = pCurrent->info;
                                openlist_found->g = temp;
                            }
                        }
                    }
                }
            }

            return getPath(pCurrent);
        }

    private:

        //// Default interpretation of a pathfinding result.
        //// If path found, traverses back to starting point and returns
        //// array of info values from starting to target point that describe  
        //// the movement directions
        std::vector<char> getPath(Node* pNode)
        {
            std::vector<char> a{};
            
            // Trace back to return a vector of moves (.info)
            while (pNode)
            {
                a.emplace_back(pNode->info);
                pNode = pNode->parent;
            }

            a.pop_back();
            std::reverse(a.begin(), a.end());

            return a;
        }
    };

} // end namespace

#endif