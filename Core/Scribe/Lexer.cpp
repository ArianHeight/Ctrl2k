#include "Lexer.h"
#include <sstream>
#include <fstream>

using namespace scb;

Lexer::Lexer(std::unique_ptr<LexerFSM> fsm) : m_fsm(std::move(fsm))
{
}

bool Lexer::tokenizeStream(std::istream& is, obn::string_registry& registry, std::vector<Token>& out)
{
    TokenType currentType;
    TokenMetadata currentMeta;
    std::string currentToken = "";

    while(!is.eof())
    {
        obn::string_pool_chartype c = is.get();
        if(m_fsm->updateState(c, currentToken, currentType, currentMeta)) // add char failed, end token
        {
            out.push_back({ currentType, currentMeta, registry.register_string(currentToken.c_str(), currentToken.length()) });

            m_fsm.reset();
            m_fsm->updateState(c, currentToken, currentType, currentMeta); // add char again as first char of a new token
        }
    }

    if(m_fsm->goToEndState(currentToken, currentType, currentMeta))
    {
        out.push_back({ currentType, currentMeta, registry.register_string(currentToken.c_str(), currentToken.length()) });
    }

    out.push_back({ TokenType::END_OF_FILE, 0, obn::INVALID_STRING_REGISTRY_ID });

    return true;
}

bool Lexer::tokenizeFile(const gbt::FilePath& filepath, obn::string_registry& registry, std::vector<Token>& out)
{
    std::ifstream file(filepath.path(), std::ios::in);
    return tokenizeStream(file, registry, out);
}

bool Lexer::tokenizeString(const std::string& str, obn::string_registry& registry, std::vector<Token>& out)
{
    std::istringstream issm(str);
    return tokenizeStream(issm, registry, out);
}

static LexerFSMState selfTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    return state;
}

static obn::string_pool_chartype escapeConversion(obn::string_pool_chartype c)
{
    switch(c)
    {
    case '\'':
    case '\"':
        return c;
    case '?':
        return '\?';
    case '\\':
        return c;
    case 'a':
        return '\a';
    case 'b':
        return '\b';
    case 'f':
        return '\f';
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    default:
        break;
    }
    return '\0';
}

// Make sure to rewrite the transition functions if string pool chartype changes
compile_assert(sizeof(obn::string_pool_chartype) == sizeof(char));
LexerFSMState DotConfigLexerFSM::startTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(obn::char_isblank(c))
    {
        return state;
    }
    else if(obn::char_isalpha(c) || c == '_')
    {
        return State::SYMBOL;
    }
    else if(c == '0')
    {
        return State::ZERO;
    }
    else if(obn::char_isdigit(c))
    {
        return State::NUMBER;
    }
    
    switch(c)
    {
    case '#':
        return State::COMMENT;
    case '-':
        return State::NEGATIVE;
    case '\"':
        return State::STRING_HEAD;
    case '\n':
    case '\r':
        return State::END_LINE;
    }
    return state; // TODO invalid
}

LexerFSMState DotConfigLexerFSM::commentTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    return c == '\n' ? State::IGNORE_END : state;
}

LexerFSMState DotConfigLexerFSM::endlineTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == '\n' || c == '\r')
    {
        return state;
    }
    return END_LINE_END;
}

LexerFSMState DotConfigLexerFSM::symbolTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(!(obn::char_isalnum(c) || c == '_'))
    {
        return State::SYMBOL_END;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::numberTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == '.')
    {
        return State::DECIMAL;
    }
    else if(!obn::char_isdigit(c))
    {
        return State::NUMBER_END;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::negativeTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(obn::char_isblank(c))
    {
        return state;
    }
    else if(obn::char_isdigit(c))
    {
        return State::NUMBER;
    }
    return State::INVALID_END;
}

LexerFSMState DotConfigLexerFSM::decimalTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(!obn::char_isdigit(c))
    {
        return State::DECIMAL_END;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::zeroTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == 'x' || c == 'X')
    {
        return State::HEXADECIMAL;
    }
    else if(c == 'b' || c == 'B')
    {
        return State::BINARY;
    }
    else if(obn::char_isdigit(c))
    {
        return State::NUMBER;
    }
    return State::NUMBER_END;
}

LexerFSMState DotConfigLexerFSM::binaryTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == '0' || c == '1')
    {
        return state;
    }
    return State::BINARY_END;
}

LexerFSMState DotConfigLexerFSM::hexadecimalTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(obn::char_isxdigit(c))
    {
        return state;
    }
    return State::HEXADECIMAL_END;
}

LexerFSMState DotConfigLexerFSM::stringheadTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == '\"')
    {
        return State::STRING_TAIL;
    }
    else if(c == '\n' || c == '\r')
    {
        return State::INVALID_END;
    }
    else if(c == '\\')
    {
        return State::STRING_ESCAPE;
    }
    return State::STRING_BODY;
}

LexerFSMState DotConfigLexerFSM::stringbodyTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == '\"')
    {
        return State::STRING_TAIL;
    }
    else if(c == '\n' || c == '\r')
    {
        return State::INVALID_END;
    }
    else if(c == '\\')
    {
        return State::STRING_ESCAPE;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::stringtailTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    return State::STRING_END;
}

LexerFSMState DotConfigLexerFSM::stringescapeTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    switch(c)
    {
    case '\'':
    case '\"':
    case '\\':
    case 'n':
    case 'r':
    case 't':
        return State::STRING_ESCAPE_CHAR;
    default:
        return State::INVALID_END;
    }

    return state;
}

DotConfigLexerFSM::DotConfigLexerFSM()
{
    m_transitionFuncs[State::START] = startTransition;
    m_transitionFuncs[State::COMMENT] = commentTransition;
    m_transitionFuncs[State::END_LINE] = endlineTransition;
    m_transitionFuncs[State::SYMBOL] = symbolTransition;
    m_transitionFuncs[State::NUMBER] = numberTransition;
    m_transitionFuncs[State::NEGATIVE] = negativeTransition;
    m_transitionFuncs[State::DECIMAL] = decimalTransition;
    m_transitionFuncs[State::ZERO] = zeroTransition;
    m_transitionFuncs[State::BINARY] = binaryTransition;
    m_transitionFuncs[State::HEXADECIMAL] = hexadecimalTransition;
    m_transitionFuncs[State::STRING_HEAD] = stringheadTransition;
    m_transitionFuncs[State::STRING_BODY] = stringbodyTransition;
    m_transitionFuncs[State::STRING_TAIL] = stringtailTransition;
    m_transitionFuncs[State::STRING_ESCAPE] = stringescapeTransition;
    m_transitionFuncs[State::STRING_ESCAPE_CHAR] = stringheadTransition;
    m_transitionFuncs[State::END_LINE_END] = selfTransition;
    m_transitionFuncs[State::SYMBOL_END] = selfTransition;
    m_transitionFuncs[State::NUMBER_END] = selfTransition;
    m_transitionFuncs[State::DECIMAL_END] = selfTransition;
    m_transitionFuncs[State::BINARY_END] = selfTransition;
    m_transitionFuncs[State::HEXADECIMAL_END] = selfTransition;
    m_transitionFuncs[State::STRING_END] = selfTransition;
    m_transitionFuncs[State::INVALID_END] = selfTransition;
    m_transitionFuncs[State::IGNORE_END] = selfTransition;
}

void DotConfigLexerFSM::reset()
{
    m_currentState = 0;
    m_currentToken.clear();
}

bool DotConfigLexerFSM::updateState(
    obn::string_pool_chartype c,
    std::string& outString,
    TokenType& outType,
    TokenMetadata& outMeta)
{
    m_currentState = m_transitionFuncs[m_currentState](c, m_currentState);

    switch(m_currentState)
    {
    case State::START:
    case State::COMMENT:
    case State::STRING_HEAD:
    case State::STRING_TAIL:
    case State::STRING_ESCAPE:
        break;
    case State::STRING_ESCAPE_CHAR:
        m_currentToken.push_back(escapeConversion(c));
        break;
    case State::END_LINE_END:
        outType = TokenType::END_OF_LINE;
        outMeta = Metadata::DEFAULT;
        outString = m_currentToken;
        return true;
    case State::SYMBOL_END:
        outType = TokenType::SYMBOL;
        outMeta = Metadata::DEFAULT;
        outString = m_currentToken;
        return true;
    case State::NUMBER_END:
        outType = TokenType::NUMBER;
        outMeta = Metadata::DEFAULT;
        outString = m_currentToken;
        return true;
    case State::DECIMAL_END:
        outType = TokenType::NUMBER;
        outMeta = Metadata::IS_FLOAT;
        outString = m_currentToken;
        return true;
    case State::BINARY_END:
        outType = TokenType::NUMBER;
        outMeta = Metadata::IS_BINARY;
        outString = m_currentToken;
        return true;
    case State::HEXADECIMAL_END:
        outType = TokenType::NUMBER;
        outMeta = Metadata::IS_HEX;
        outString = m_currentToken;
        return true;
    case State::STRING_END:
        outType = TokenType::STRING;
        outMeta = Metadata::DEFAULT;
        outString = m_currentToken;
        return true;
    case State::INVALID_END:
        outType = TokenType::INVALID;
        outMeta = Metadata::DEFAULT;
        outString = m_currentToken;
        return true;
    case State::IGNORE_END:
        reset();
        return false;
    default:
        m_currentToken.push_back(c);
    }

    return false;
}

bool DotConfigLexerFSM::goToEndState(std::string& outString, TokenType& outType, TokenMetadata& outMeta)
{
    return updateState('\n', outString, outType, outMeta);
}
