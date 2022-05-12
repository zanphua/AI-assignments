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
            return os;
        }

        

        // Deserialization

        //"a {1 aa {1 aaa {0 } } }"
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            is >> rhs.value; // Just for test, you can change it
            
            std::string input = rhs.value;  //store value into temp variable

            size_t n = input.find_first_of(' ');    //find space

            if (n == std::string::npos) return is;//if space not found return
                
            rhs.value = input.substr(0, n); //get substr (first object)

            input = input.substr(n + 1, std::string::npos); //get remaining string

            if (input[0] != '{') return is; //if no more child, return

            size_t x = input.find(' '); //find next space

            if(x == std::string::npos) return is;//if space not found return

            int len = stoi(input.substr(0, x)); //get length of substr remaining

            for (int i{ 0 }; i < len; ++i)
            {
                Node* child = new Node("", this);
                input << *child;
                rhs.children.push_back(child);
            }

            size_t y = input.find('}');
            if(y == std::string::npos) return is;//if } not found return

            input = input.substr(n + 1, std::string::npos);

            is = input;

            return is;
        }

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            std::vector<T> r;
            // ...
            return r;
        }
    };

    // Implementation of the Breadth-First Search algorithm
    template<typename T>
    Node<T>* BFS(Node<T> & node, const T & lookingfor)
    {
        UNUSED(node)
        UNUSED(lookingfor)
        // ...
        return nullptr;
    }

    // Implementation of the Depth-First Search algorithm
    template<typename T>
    Node<T>* DFS(Node<T> & node, const T & lookingfor)
    {
        UNUSED(node)
        UNUSED(lookingfor)
        // ...
        return nullptr;
    }

} // end namespace

#endif