#pragma once
#include "NodeType.h"
#include <vector>
#include <string>

/// @brief Node object representing the nodes in the AST
class Node
{
public:
    /// @brief The type of the node
    NodeType type;
    /// @brief The value that the node contains
    std::string value;
    /// @brief Child nodes
    std::vector<Node>* children;
    /// @brief Base constructor with default parameters
    Node() { type = NodeType::undefined; children = new std::vector<Node>(); }
    /// @brief Constructor for creating a node by type 
    /// @param type The node type
    Node(NodeType type) : type(type) { children = new std::vector<Node>(); }
    /// @brief Constructor for creating a node by type and value
    /// @param type The node type
    /// @param value The node value
    Node(NodeType type, std::string value) : type(type), value(value) { children = new std::vector<Node>(); }

    /// @brief Operator == overloading for custom object comparison
    /// @param other The other node
    /// @return True if nodes are equal, otherwise false
    bool operator==(const Node& other) const
    {
        // We define equals operator, so we could use the node in hash tables later
        return (type == other.type
            && value == other.value
            && children == other.children);
    }
};

// Define hash struck for our Node class,
// because we want to use it in hash tables later
namespace std
{
    template <>
    struct hash<Node>
    {
        std::size_t operator()(const Node& node) const
        {
            // Compute hash values for node elements,
            // and combine them using XOR and bit shifting
            // (it is recomended for better hash values (i.e less collisions))
            return ((hash<int>()((int)node.type)
                ^ (hash<std::string>()(node.value) << 1)) >> 1)
                ^ (hash<std::vector<Node>*>()(node.children) << 1);
        }
    };
}