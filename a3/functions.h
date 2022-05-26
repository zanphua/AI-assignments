#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <algorithm>

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

        // Serialization
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
            if (n == std::string::npos) return is;//if space not found return

            int len = stoi(input.substr(0, n)); //get length of substr remaining

            if (isnan(static_cast<float>(len)))  return is; //if number not found, return

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

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            std::vector<T> r = std::vector<T>();

            // if (this == nullptr)
            // {
            //     return r;
            // }

            r.insert(r.begin(), this->value);

            Node<T>* node = this->parent;

            while (node)
            {
                r.insert(r.begin(), node->value);

                //std::cout << "parent value: " << node->parent->value << "\n";

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

        std::vector<TreeNode*> operator()(TreeNode* pNode)
        {
           UNUSED(pNode)

            std::vector<AI::TreeNode*> list = {};

            // Push to the list all children of pNode excluding 
            // those with value not equal "x"

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
            UNUSED(pNode)

            std::vector<TreeNode*> adjacents;

            // Use the base class operator() and then shuffle the result

            return adjacents;
        }
    };

    // Wrappers that provide same interface for queue and stack

    struct Interface
    {
        virtual void clear() = 0;

        virtual void push(TreeNode* pNode) = 0;

        virtual TreeNode* pop() = 0;
    };

    struct Queue : Interface //...
    {
        void clear()
        {
            //...
        }

        void push(TreeNode* pNode)
        {
           UNUSED(pNode)
           //...
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
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

        void push(TreeNode* pNode)
        {
            UNUSED(pNode)
            //...
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
            //...
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
            UNUSED(pNode)
            UNUSED(value)

            // Implement the flood fill
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
            UNUSED(pNode)
            UNUSED(value)

            // Implement the flood fill
         }
    };

} // end namespace

#endif