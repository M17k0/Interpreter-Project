#pragma once

/// @brief Enumeration with the possible Node types for the AST nodes
enum class NodeType
{
    undefined,
    root,
    function,
    variable,
    number,
    define_function,
    operation_read,
    operation_print,
    operation_assign,
    operation_add,
    operation_subtract,
    operation_multipy,
    operation_divide,
    operation_modulo,

};