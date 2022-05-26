/*!*****************************************************************************
\file functions.h
\author Zandra Phua Si-Yu
\par DP email: p.siyuzandra@digipen.edu
\par Course: CSD3182
\par Section: B
\par Assignment 3 (Tree Flood Fill)
\date 26-05-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <algorithm>
#include <random>
#include <queue>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{

    // A simple graph node definition with serialization functions
    template<typename T>
    struct Node
    {
        // Member data

        T value;
        Node* parent;
        std::list<Node*> children;

        Node(T value = {}, Node* parent = nullptr,
            const std::list<Node*>& children = {})
            : value{ value }, parent{ parent }, children{ children }
        {

        }

        ~Node()
        {
            for (auto child : children)
                delete child;
        }

        /*!*****************************************************************************
        \brief Friend function for Serialization
        \param std::ostream& os, output stream
        \param Node const& rhs, Node object

        \return std::ostream&
        *******************************************************************************/
        friend std::ostream& operator<<(std::ostream& os, Node const& rhs)
        {
            std::string temp{ "" };
            temp += rhs.value;
            temp += " {";    //  "a{"

            temp += std::to_string(rhs.children.size()) + " "; //"a{0 "

            for (Node* child : rhs.children)
            {
                std::ostringstream getChild;
                getChild << *child;

                temp += getChild.str();
            }

            temp += "} ";    //"a{0 }"
            os << temp;

            return os;
        }

        /*!*****************************************************************************
        \brief Friend function for Deserialization
        \param std::istream& is, input stream
        \param Node& rhs, Node object

        \return std::istream&
        *******************************************************************************/
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            std::istreambuf_iterator<char> item;
            std::string input(std::istreambuf_iterator<char>(is), item);    //retrieving input string

            size_t n = input.find_first_of(' ');    //find space
            if (n == std::string::npos) return is;//if space not found return

            rhs.value = input.substr(0, n); //get substr (first object)

            input = input.substr(n + 1, std::string::npos); //get remaining string

            if (input[0] != '{') return is; //if no more child, return

            input = input.substr(1, std::string::npos); //splice {

            n = input.find(' '); //find next space
            if (n == std::string::npos) return is;//if space not found return

            int len = stoi(input.substr(0, n)); //get length of substr remaining

            if (std::isnan(static_cast<float>(len)))  return is; //if number not found, return

            input = input.substr(n + 1, std::string::npos);

            for (int i{ 0 }; i < len; ++i)  //loop for each child
            {
                Node<std::string>* child = new Node();
                child->parent = &rhs;

                std::istringstream current{ input };
                current >> *child;

                rhs.children.emplace_back(child);

                size_t noc = input.find_first_of("1234567890");
                int num = stoi(input.substr(noc, 1));

                if (num > 0)
                {
                    size_t temp = num + 1;
                    size_t x = 0;
                    while (temp > 0)
                    {
                        x = input.find('}', x + 1);
                        temp--;
                    }

                    input = input.substr(x + 2, std::string::npos);
                }
                else
                {
                    n = input.find('}');
                    if (n == std::string::npos) return is;//if } not found return
                    input = input.substr(n + 2, std::string::npos);
                }
            }

            return is;
        }

        /*!*****************************************************************************
       /Brief   Returns values from root to this node as an array
       /returns std::vector<T>, Vector of path to this node
       *******************************************************************************/
        std::vector<T> getPath() const
        {
            std::vector<T> r = std::vector<T>();

            r.insert(r.begin(), this->value);

            Node<T>* node = this->parent;

            while (node)
            {
                r.insert(r.begin(), node->value);

                node = node->parent;
            }

            return r;
        }
    };


    // The actual node type for this assignment
    using TreeNode = Node<std::string>;


    // Abstract base class for domain specific functors that return adjacent nodes
    class GetAdjacents
    {

    public:

        virtual ~GetAdjacents()
        {
        }

        virtual std::vector<TreeNode*> operator()(TreeNode* pNode) = 0;
    };


    // Domain specific functor that returns adjacent nodes
    class GetTreeAdjacents : public GetAdjacents
    {
 
    public:

        GetTreeAdjacents()
            : GetAdjacents()
        {
        }

        void setValue(TreeNode* pNode, std::string val)
        {
            pNode->value = val;
        }

        std::vector<TreeNode*> operator()(TreeNode* pNode)
        {
            std::vector<AI::TreeNode*> list = {};

            // Push to the list, all children of pNode  
            // excluding those with value not equal "x"
            std::list<TreeNode*>::iterator it;

            for (it = pNode->children.begin(); it != pNode->children.end(); it++)
            {
                TreeNode* temp = *it;

                if (temp->value == "x")
                {
                    list.emplace_back(temp);
                }
            }           

            return list;
        }
    };

    // Domain specific functor that returns shuffled adjacent nodes
    class GetTreeStochasticAdjacents : public GetTreeAdjacents
    {
    public:

        GetTreeStochasticAdjacents()
            : GetTreeAdjacents{}
        {
        }

        std::vector<TreeNode*> operator()(TreeNode* pNode)
        {
            // Use the base class operator()
            std::vector<TreeNode*> adjacents = GetTreeAdjacents::operator()(pNode);

            //shuffle the result
            auto rng = std::default_random_engine{};
            std::shuffle(adjacents.begin(), adjacents.end(), rng);

            return adjacents;
        }
    };

    // Wrappers that provide same interface for queue and stack

    struct Interface
    {
        virtual bool empty() = 0;

        virtual void clear() = 0;

        virtual void push(TreeNode* pNode) = 0;

        virtual TreeNode* pop() = 0;
    };

    struct Queue : Interface //...
    {
        std::queue<TreeNode*> my_queue;

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

        void push(TreeNode* pNode)
        {
            TreeNode* new_node = new TreeNode;
            new_node->value = pNode->value;
            new_node->children = pNode->children;

            my_queue.push(new_node);
        }

        TreeNode* pop()
        {
            TreeNode* pNode = my_queue.front();

            my_queue.pop();

            return pNode;
        }
    };

    struct Stack : Interface //...
    {
        std::stack<TreeNode*> my_stack{};

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

        void push(TreeNode* pNode)
        {
            TreeNode* new_node = new TreeNode;
            new_node->value = pNode->value;
            new_node->children = pNode->children;

            my_stack.push(new_node);
        }

        TreeNode* pop()
        {
            TreeNode* pNode = my_stack.top();

            my_stack.pop();

            return pNode;
        }
    };

    // Recursive Flood Fill
    class Flood_Fill_Recursive
    {
        GetTreeAdjacents* pGetAdjacents;

    public:
        Flood_Fill_Recursive(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }
        {
        }

        void run(TreeNode* pNode, std::string value)
        {
            // Implement the flood fill
            std::vector<TreeNode*> adjacents = this->pGetAdjacents->operator()(pNode);

            GetTreeAdjacents* Tree_adj = static_cast<GetTreeAdjacents*>(pGetAdjacents);

            for (TreeNode* i : adjacents)
            {
                Tree_adj->setValue(i, value);
                this->run(i, value);
            }
        }
    };

    // Iterative Flood Fill
    // Type T defines is it depth- or breadth-first
    template<typename T>
    class Flood_Fill_Iterative
    {
        GetTreeAdjacents* pGetAdjacents;
        T openlist;

    public:
        Flood_Fill_Iterative(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }, openlist{}
        {
        }

        void run(TreeNode* pNode, std::string value)
        {
            // Implement the flood fill
            this->openlist.clear(); //clear current list
            this->openlist.push(pNode); //push root node

            while (!this->openlist.empty())//check not empty
            {
                TreeNode* current = this->openlist.pop();   //get node
                std::vector<TreeNode*> adjacents = this->pGetAdjacents->operator()(current);   //find all adjacent nodes

                GetTreeAdjacents* Tree_adj = static_cast<GetTreeAdjacents*>(pGetAdjacents);

                for (TreeNode* i : adjacents)
                {
                    //std::cout << "value: " << value << "\n";
                    Tree_adj->setValue(i, value);
                    this->openlist.push(i);
                }
            }
         }
    };

} // end namespace

#endif