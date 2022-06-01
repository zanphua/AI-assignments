#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

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

        std::vector<AI::Node*> operator()(Key key)
        {
            //UNUSED(key)

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

    // Base class with interfaces for Dijkstra's and AStar search
   /* class Pathfinder
    {
        Pathfinder(GetAdjacents* pGetAdjacents, )

        std::vector<char>run(Key starting, Key target)
    };*/

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

            while (true)
            {
                if (openlist.empty())
                {
                    pCurrent = nullptr;
                    break;
                }

                pCurrent = openlist.pop();

                closedlist->add(pCurrent->key, pCurrent);

                if (pCurrent->key == target) break;

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

            std::reverse(a.begin(), a.end());
            a.pop_back();

            return a;
        }
    };

} // end namespace

#endif