#pragma once
#include "Core/Monument/Monument.h"

namespace scb
{

struct indexed_string_view
{
    size_t index;
    size_t size;
};

typedef size_t token_id;

struct token
{
    token_id id;
};

constexpr size_t LEXER_MAX_SYMBOL_CAPACITY = 128;
constexpr size_t LEXER_MAX_SYMBOL_LENGTH = LEXER_MAX_SYMBOL_CAPACITY - 1; // -1 for null terminator

class Lexer
{
private:
};

}