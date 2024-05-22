#pragma once

#include "Node.h"
#include <iostream>
#include <algorithm>

/// @brief Class with methods that executes an AST
class Executor
{
public:
    /// @brief Executes an AST given by its root using the console output and input streams
    /// @param treeRoot The root node of the AST
    static void execute(Node treeRoot);

    /// @brief Executes an AST given by its root using the given output and input streams
    /// @param treeRoot The root node of the AST
    /// @param out The output stream
    /// @param in The input stream
    static void execute(Node treeRoot, std::ostream& out, std::istream& in);

    /// @brief Deletes the AST (deletes the children vector)
    /// @param treeRoot The root of the tree
    static void deleteTree(Node& treeRoot);
};
