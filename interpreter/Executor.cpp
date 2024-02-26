#include "Node.h"
#include <iostream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

/// @brief Class with methods that executes an AST
class Executor
{
public:
    /// @brief Executes an AST given by its root using the console output and input streams
    /// @param treeRoot The root node of the AST
    static void execute(Node treeRoot)
    {
        execute(treeRoot, std::cout, std::cin);
    }

	/// @brief Executes an AST given by its root using the given output and input streams
	/// @param treeRoot The root node of the AST
	/// @param out The output stream
	/// @param in The input stream
	static void execute(Node treeRoot, std::ostream& out, std::istream& in)
	{
        // Execution is done by traversing the AST with dfs iteratively

        // We need stacks for the currently executed node, the results of the execution and a stack for the funtion call parameters, 
        // hash map to track the visited nodes 
        std::stack<Node> executionStack;
        std::unordered_map<Node, int> visitedChildren;
        std::stack<long long> executionResults;
        std::stack<std::pair<std::string, long long>> functionParameterStack;
        
        // We need hash maps to store the cuurent values of variables, 
        // function definition nodes (for faster finding when called)
        // and for the function paremeters
        std::unordered_map<std::string, long long> variables;
        std::unordered_map<std::string, Node> functions;
        std::unordered_map<std::string, std::string> functionParametersMap;

        executionStack.push(treeRoot);
        visitedChildren.insert(std::pair<Node, int>(treeRoot, 0));

        while (!executionStack.empty())
        {
            Node currNode = executionStack.top();

            if (currNode.type == NodeType::root)
            {
                int nextChildIndex = visitedChildren[currNode];

                if (nextChildIndex < currNode.children->size())
                {
                    Node child = (*currNode.children)[nextChildIndex];
                    executionStack.push(child);
                    visitedChildren[child] = 0;
                    visitedChildren[currNode]++;
                }
                else
                {
                    executionStack.pop();
                }
            }
            else
            {
                // For the assign operator we execute the right child (which contains the expression of the assignment)
                // and set it to the variable in the left child
                if (currNode.type == NodeType::operation_assign)
                {
                    int nextChildIndex = visitedChildren[currNode];
                    if (nextChildIndex < currNode.children->size())
                    {
                        if (nextChildIndex == 0)
                        {
                            nextChildIndex++;
                            visitedChildren[currNode]++;
                        }
                        Node child = (*currNode.children)[nextChildIndex];
                        executionStack.push(child);
                        visitedChildren[child] = 0;
                        visitedChildren[currNode]++;
                    }
                    else
                    {
                        long long result = executionResults.top();
                        executionResults.pop();

                        std::string varName = (*currNode.children)[0].value;
                        if (variables.find(varName) == variables.end())
                        {
                            variables.insert(std::pair<std::string, long long>(varName, 0));
                        }
                        variables[varName] = result;

                        executionStack.pop();
                    }
                }
                // For variables, when a function is called (function params stack is not empty)
                // we need to check if the variable is from the function and use its value
                // or a global variable and retrieve it from the map
                else if (currNode.type == NodeType::variable)
                {
                    if (functionParameterStack.empty())
                    {
                        if (variables.find(currNode.value) == variables.end())
                        {
                            throw std::invalid_argument("Use of undefined variable '" + currNode.value + "'");
                        }
                        executionResults.push(variables[currNode.value]);
                    }
                    else
                    {
                        std::pair<std::string, long long> funcParamPair = functionParameterStack.top();
                        if (functionParametersMap.find(funcParamPair.first) == functionParametersMap.end())
                        {
                            throw std::invalid_argument("Incorrect function call");
                        }

                        if (functionParametersMap[funcParamPair.first] == currNode.value)
                        {
                            executionResults.push(funcParamPair.second);
                        }
                        else
                        {
                            if (variables.find(currNode.value) == variables.end())
                            {
                                throw std::invalid_argument("Use of undefined variable '" + currNode.value + "'");
                            }
                            executionResults.push(variables[currNode.value]);
                        }
                    }

                    executionStack.pop();
                }
                else if (currNode.type == NodeType::number)
                {
                    long long num;
                    try
                    {
                        num = std::stoul(currNode.value, nullptr, 10);
                    }
                    catch (const std::exception&)
                    {
                        throw std::invalid_argument("Invalid number");
                    }

                    executionResults.push(num);

                    executionStack.pop();
                }
                // For operator nodes, we execute its child nodes and then execute the operator from the results
                // (which are in the results stack)
                else if (currNode.type == NodeType::operation_add
                    || currNode.type == NodeType::operation_subtract
                    || currNode.type == NodeType::operation_multipy
                    || currNode.type == NodeType::operation_divide
                    || currNode.type == NodeType::operation_modulo)
                {
                    int nextChildIndex = visitedChildren[currNode];
                    if (nextChildIndex < currNode.children->size())
                    {
                        Node child = (*currNode.children)[nextChildIndex];
                        executionStack.push(child);
                        visitedChildren[child] = 0;
                        visitedChildren[currNode]++;
                    }
                    else
                    {
                        long long right = executionResults.top();
                        executionResults.pop();
                        long long left = executionResults.top();
                        executionResults.pop();


                        long long result;
                        switch (currNode.type)
                        {
                        case NodeType::operation_add:
                            result = left + right;
                            break;
                        case NodeType::operation_subtract:
                            result = left - right;
                            break;
                        case NodeType::operation_multipy:
                            result = left * right;
                            break;
                        case NodeType::operation_divide:
                            result = left / right;
                            break;
                        case NodeType::operation_modulo:
                            result = left % right;
                            break;
                        default:
                            break;
                        }
                        executionResults.push(result);

                        executionStack.pop();
                    }
                }
                // For printing we execute its single node (which callculates the expression)
                // and output it to the ouput stream
                else if (currNode.type == NodeType::operation_print)
                {
                    int nextChildIndex = visitedChildren[currNode];
                    if (nextChildIndex < currNode.children->size())
                    {
                        Node child = (*currNode.children)[nextChildIndex];
                        executionStack.push(child);
                        visitedChildren[child] = 0;
                        visitedChildren[currNode]++;
                    }
                    else
                    {
                        long long result = executionResults.top();
                        executionResults.pop();

                        out << result << std::endl;

                        executionStack.pop();
                    }
                }
                // Read has a single child with the variable which we set from the input stream
                else if (currNode.type == NodeType::operation_read)
                {
                    int nextChildIndex = visitedChildren[currNode];
                    if (visitedChildren[currNode] == 0)
                    {
                        visitedChildren[currNode]++;
                    }

                    long long result;

                    std::string varName = (*currNode.children)[0].value;
                    if (variables.find(varName) == variables.end())
                    {
                        variables.insert(std::pair<std::string, long long>(varName, 0));
                    }

                    std::string input;
                    in >> input;
                    try
                    {
                        result = stoul(input, nullptr, 10);
                    }
                    catch (std::exception ex)
                    {
                        throw std::invalid_argument("Invalid number is entered");
                    }

                    variables[varName] = result;

                    executionStack.pop();
                }
                // For define function nodes we just save the node in the functions map for execution latter on a function call
                else if (currNode.type == NodeType::define_function)
                {
                    std::string funcName = currNode.value;
                    if (functions.find(funcName) == functions.end())
                    {
                        functions.insert(std::pair<std::string, Node>(funcName, currNode));
                    }
                    else
                    {
                        throw std::invalid_argument("Function " + funcName + " already defined!");
                    }

                    executionStack.pop();
                }
                // For fuction call, we first execute the single child (with the expression of the function parameter)
                // then we find the function def and execute its expression by providing the value for its parameter
                else if (currNode.type == NodeType::function)
                {
                    int nextChildIndex = visitedChildren[currNode];
                    if (nextChildIndex < currNode.children->size())
                    {
                        Node child = (*currNode.children)[nextChildIndex];
                        executionStack.push(child);
                        visitedChildren[child] = 0;
                        visitedChildren[currNode]++;
                    }
                    else
                    {
                        if (functionParameterStack.empty()
                            || functionParameterStack.top().first != currNode.value)
                        {
                            long long result = executionResults.top();
                            executionResults.pop();

                            if (functions.find(currNode.value) == functions.end())
                            {
                                throw std::invalid_argument("Function " + currNode.value + " is not defined!");
                            }

                            Node functionDefNode = functions[currNode.value];
                            functionParametersMap[functionDefNode.value] = (*functionDefNode.children)[0].value;

                            functionParameterStack.push(std::pair<std::string, long long>(functionDefNode.value, result));

                            Node child = (*functionDefNode.children)[1];
                            executionStack.push(child);
                            visitedChildren[child] = 0;
                        }
                        else
                        {
                            functionParameterStack.pop();

                            executionStack.pop();
                        }

                    }
                }
            }
        }
	}

    /// @brief Deletes the AST (deletes the children vector)
    /// @param treeRoot The root of the tree
    static void deleteTree(Node& treeRoot)
    {
        // Delete the children vector pointers for every node with a simple dfs

        std::stack<Node> deleteStack;
        std::unordered_map<Node, int> visitedChildren;

        deleteStack.push(treeRoot);
        visitedChildren[treeRoot] = 0;
        while (!deleteStack.empty())
        {
            Node currentNode = deleteStack.top();
            int nextChildIndex = visitedChildren[currentNode];

            if (nextChildIndex < currentNode.children->size())
            {
                Node child = (*currentNode.children)[nextChildIndex];
                deleteStack.push(child);
                visitedChildren[child] = 0;
                visitedChildren[currentNode]++;
            }
            else
            {
                delete currentNode.children;
                deleteStack.pop();
            }
        }
    }
};