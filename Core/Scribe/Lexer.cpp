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

// Make sure to rewrite the transition functions if string pool chartype changes
compile_assert(sizeof(obn::string_pool_chartype) == sizeof(char));
LexerFSMState DotConfigLexerFSM::startTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == 'T' || c == 't')
    {
        return State::BOOL_T;
    }
    else if(obn::char_isalpha(c) || c == '_')
    {
        return State::KEY;
    }
    switch(c)
    {
    case '#':
        return State::COMMENT;
    case ' ':
    case '\t':
        return state;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::commentTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    return c == '\n' ? State::IGNORE_END : state;
}

LexerFSMState DotConfigLexerFSM::keyTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(!(obn::char_isalnum(c) || c == '_'))
    {
        return State::KEY_END;
    }
    return state;
}

LexerFSMState DotConfigLexerFSM::boolTTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == 'r')
    {
        return State::BOOL_R;
    }
    else if(obn::char_isalpha(c) || c == '_')
    {
        return State::KEY;
    }
    return State::KEY_END;
}

LexerFSMState DotConfigLexerFSM::boolRTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == 'u')
    {
        return State::BOOL_U;
    }
    else if(obn::char_isalpha(c) || c == '_')
    {
        return State::KEY;
    }
    return State::KEY_END;
}

LexerFSMState DotConfigLexerFSM::boolUTransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(c == 'e')
    {
        return State::BOOL_R;
    }
    else if(obn::char_isalpha(c) || c == '_')
    {
        return State::KEY;
    }
    return State::KEY_END;
}

LexerFSMState DotConfigLexerFSM::boolETransition(obn::string_pool_chartype c, LexerFSMState state)
{
    if(obn::char_isalpha(c) || c == '_')
    {
        return State::KEY;
    }
    return State::BOOL_END;
}

DotConfigLexerFSM::DotConfigLexerFSM()
{
    m_transitionFuncs[State::START] = startTransition;
    m_transitionFuncs[State::COMMENT] = commentTransition;
    m_transitionFuncs[State::KEY] = keyTransition;
    m_transitionFuncs[State::BOOL_T] = boolTTransition;
    m_transitionFuncs[State::BOOL_R] = boolRTransition;
    m_transitionFuncs[State::BOOL_U] = boolUTransition;
    m_transitionFuncs[State::BOOL_E] = boolETransition;
    m_transitionFuncs[State::KEY_END] = selfTransition;
    m_transitionFuncs[State::BOOL_END] = selfTransition;
    m_transitionFuncs[State::NUMBER_END] = selfTransition;
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
        break;
    case State::KEY_END:
        outType = TokenType::SYMBOL;
        outString = m_currentToken;
        return true;
    case State::BOOL_END:
        outType = TokenType::BOOL;
        outString = m_currentToken;
        return true;
    case State::NUMBER_END:
        outType = TokenType::NUMBER;
        outString = m_currentToken;
        return true;
    case State::STRING_END:
        outType = TokenType::STRING;
        outString = m_currentToken;
        return true;
    case State::INVALID_END:
        outType = TokenType::INVALID;
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
    return false;
}
