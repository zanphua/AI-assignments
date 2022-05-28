/*!*****************************************************************************
\file functions.h
\author Zandra Phua Si-Yu
\par DP email: p.siyuzandra@digipen.edu
\par Course: CSD3182
\par Section: B
\par Assignment 2 (Flood Fill)
\date 18-05-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stack>
#include <queue>
#include <algorithm>
#include <random>

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

        void setValue(Key k, int color)
        {
            int j = k.j;
            int i = k.i;
            this->map[j * this->size + i] = color;
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

            // Find and return all empty adjacent cells
            // Use the base class operator()
            std::vector<AI::Node*> list = GetMapAdjacents::operator()(key);
            
            //shuffle the result
            auto rng = std::default_random_engine{};
            std::shuffle(list.begin(), list.end(), rng);    //shuffle

            return list;
        }
    };

    // Wrappers that provide same interface for queue and stack

    struct Interface
    {
        virtual bool empty() = 0;

        virtual void clear() = 0;

        virtual void push(Node* pNode) = 0;

        virtual Node* pop() = 0;
    };

    struct Queue : Interface //...
    {
        std::queue<Node*> my_queue;

        bool empty()
        {
            if (my_queue.empty())
            {
                return true;
            }

            return false;
        }

        void clear()
        {
            while (!my_queue.empty())
            {
                my_queue.pop();
            }
        }

        void push(Node* pNode)
        {
            Node* new_node = new Node;
            new_node->key.j = pNode->key.j;
            new_node->key.i = pNode->key.i;

            my_queue.push(new_node);
        }

        Node* pop()
        {
            Node* pNode = my_queue.front();
            
            my_queue.pop();

            return pNode;
        }
    };

    struct Stack : Interface //...
    {
        std::stack<Node*> my_stack{};

        bool empty()
        {
            if (my_stack.empty())
            {
                return true;
            }

            return false;
        }

        void clear()
        {
            while (!my_stack.empty())
            {
                my_stack.pop();
            }
        }

        void push(Node* pNode)
        {
            Node* new_node = new Node;
            new_node->key.j = pNode->key.j;
            new_node->key.i = pNode->key.i;

            my_stack.push(new_node);
        }

        Node* pop()
        {
            Node* pNode = my_stack.top();
            
            my_stack.pop();

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
            // Implement the flood fill
            std::vector<AI::Node*> adjacents = this->pGetAdjacents->operator()(key);

            GetMapAdjacents* map_adj = static_cast<GetMapAdjacents*>(pGetAdjacents);

            for (AI::Node* i : adjacents)
            {
                map_adj->setValue(i->key, color);
                this->run(i->key, color);
                delete i;
            }
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
            // Implement the flood fill
            this->openlist.clear();
            this->openlist.push(new Node(key));

            while (!this->openlist.empty())//check not empty
            {
                Node* current = this->openlist.pop();
                std::vector<AI::Node*> adjacents = this->pGetAdjacents->operator()(current->key);

                GetMapAdjacents* map_adj = static_cast<GetMapAdjacents*>(pGetAdjacents);

                for (Node* i : adjacents)
                {
                    map_adj->setValue(i->key, color);
                    this->openlist.push(i);
                }

                delete current;
            }
        }
    };

} // end namespace

#endif