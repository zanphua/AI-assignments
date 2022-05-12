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