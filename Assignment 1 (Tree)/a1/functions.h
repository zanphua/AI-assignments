#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include <math.h>

#include "data.h"

namespace AI 
{
    #define UNUSED(expr) (void)expr;

    

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

//"a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } "

/*
                a

        aa,     ooo,    ac

aaa aab ooo, aba abb abc, aca ooo acc

*/



        // Serialization
        friend std::ostream& operator<<(std::ostream& os, Node const& rhs)
        {
            //os << rhs.value; // Just for test, you can change it

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
            std::cout << "test string serialization: \n" << temp << "\n";

            return os;
        }
        

        // Deserialization
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
            if(n == std::string::npos) return is;//if space not found return

            int len = stoi(input.substr(0, n)); //get length of substr remaining

            if (isnan(static_cast<float>(len)))  return is; //if number not found, return

            if (len > 0)
            {
                input = input.substr(n + 1, std::string::npos);
            }
            else
            {
                input = input.substr(n + 3, std::string::npos);
            }

            for (int i{ 0 }; i < len; ++i)  //loop for each child
            {
                Node<std::string>* child = new Node();
                child->parent = &rhs;

                std::istringstream current{ input };
                current >> *child;

                rhs.children.emplace_back(child);

                n = input.find('}');
                if (n == std::string::npos) return is;//if } not found return

                input = input.substr(n + 2, std::string::npos);
            }

            n = input.find('}');
            if (n == std::string::npos) return is;//if } not found return

            //std::cout << "test string deserialization: \n" << input << "\n";

            return is;
        }

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            std::vector<T> r;
            r.emplace_back(this->value);

            auto* node = this->parent;

            while (node)
            {
                r.emplace_back(node->value);
                node = node->parent;
            }

            return r;
        }
    };

    // Implementation of the Breadth-First Search algorithm
    template<typename T>
    Node<T>* BFS(Node<T> & node, const T & lookingfor)
    {
        std::queue<Node<T>>openlist;   //create empty queue
        openlist.push(node);    //push root node into queue

        while (!openlist.empty())   //while queue is not empty
        {
            Node<T> current = openlist.front(); //get the first item in the queue
             
            openlist.pop(); //remove the visited item from the queue

            if (current.value == lookingfor)    //if its what we are looking for
            {
                node = current;
                return &node; //return that node
            }

            for (auto child : current.children) //if item not found, 
            {
                openlist.push(*child);//push in all children of the current visited node
            }
        }

        return nullptr;
    }

    // Implementation of the Depth-First Search algorithm
    template<typename T>
    Node<T>* DFS(Node<T> & node, const T & lookingfor)
    {
        std::stack< Node<T> >openlist;
        openlist.push(node);

        while (!openlist.empty())
        {
            auto current = openlist.top();
            openlist.pop();

            if (current.value == lookingfor)
            {
                node = current;
                return &node;
            }

            for (auto child : current.children)
            {
                openlist.push(*child);
            }
        }
        return nullptr;
    }

} // end namespace

#endif