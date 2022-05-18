#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stack>
#include <algorithm>

#include "data.h"

#define UNUSED(expr) (void)expr;

namespace AI 
{

    // Domain specific functor that returns adjacent nodes
    class GetMapAdjacents : public GetAdjacents
    {
        int* map; // the map with integers where 0 means an empty cell
        int size; // width and hight of the map in elements

    public:

        GetMapAdjacents(int* map=nullptr, int size=0)
            : GetAdjacents(), map{ map }, size{ size }
        {
        }

        virtual ~GetMapAdjacents()
        {
        }

        std::vector<AI::Node*> operator()(Key key)
        {
            //init function variables
            int j = key.j;
            int i = key.i;

            std::vector<AI::Node*> list = {};   //variable for vector of adjacent nodes

            // Find all empty adjacent cells and add to vector list
            if (j >= 0 && j < this->size && i >= 0 && i < this->size)
            {
                if (i > 0 && this->map[j * this->size + i - 1] == 0)
                {
                    list.emplace_back(new AI::Node({ j, i - 1 }));  //top
                }
                if (i < this->size - 1 && this->map[j * this->size + i + 1] == 0)
                {
                    list.emplace_back(new AI::Node({ j, i + 1 })); //bottom
                }
                if (j > 0 && this->map[(j - 1) * this->size + i] == 0)
                {
                    list.emplace_back(new AI::Node({ j - 1 , i })); //left
                }
                if (j < this->size - 1 && this->map[(j + 1) * this->size + i] == 0)
                {
                    list.emplace_back(new AI::Node({ j + 1 , i })); //right
                }
            }

            return list;    //return vector of adjcacent
        }
    };

    // Domain specific functor that returns shuffled adjacent nodes
    class GetMapStochasticAdjacents : public GetMapAdjacents
    {
    public:

        GetMapStochasticAdjacents(int* map, int size)
            : GetMapAdjacents{ map, size }
        {
        }

        std::vector<AI::Node*> operator()(Key key)
        {
            //UNUSED(key)
            
            std::vector<AI::Node*> list = {};
            
            // Find and return all empty adjacent cells
            // Use the base class operator() and then shuffle the result


            return list;
        }
    };

    // Wrappers that provide same interface for queue and stack

    struct Interface
    {
        virtual void clear() = 0;

        virtual void push(Node* pNode) = 0;

        virtual Node* pop() = 0;
    };

    struct Queue : Interface //...
    {
        void clear()
        {
            //...
        }

        void push(Node* pNode)
        {
           UNUSED(pNode)
           //...
        }

        Node* pop()
        {
            Node* pNode = nullptr;
            //...
            return pNode;
        }
    };

    struct Stack : Interface //...
    {
        void clear()
        {
            //...
        }

        void push(Node* pNode)
        {
            UNUSED(pNode)
            //...
        }

        Node* pop()
        {
            Node* pNode = nullptr;
            //...
            return pNode;
        }
    };

    // Recursive Flood Fill
    class Flood_Fill_Recursive
    {
        GetAdjacents* pGetAdjacents;

    public:
        Flood_Fill_Recursive(GetAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }
        {
        }

        void run(Key key, int color)
        {
            UNUSED(key)
            UNUSED(color)

            // Implement the flood fill
        }
    };

    // Iterative Flood Fill
    // Type T defines is it depth- or breadth-first
    template<typename T>
    class Flood_Fill_Iterative
    {
        GetAdjacents* pGetAdjacents;
        T openlist;

    public:
        Flood_Fill_Iterative(GetAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }, openlist{}
        {
        }

        void run(Key key, int color)
        {
            UNUSED(key)
            UNUSED(color)

            // Implement the flood fill
        }
    };

} // end namespace

#endif