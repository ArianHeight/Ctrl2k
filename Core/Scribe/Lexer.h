#pragma once
#include "Core/OracleBone/obn.h"
#include "Core/GreatBath/FilePathProcessing.h"
#include <array>

namespace scb
{

typedef uint16_t TokenMetadata;

enum TokenType : uint16_t
{
    END_OF_FILE,
    END_OF_LINE,
    OPEN,
    CLOSE,
    NUMBER,
    SYMBOL,
    BOOL,
    STRING,
    OPERATOR,
    SEPERATOR,
    CUSTOM_START,
    INVALID = UINT16_MAX
};

struct Token
{
    TokenType type;
    TokenMetadata meta;
    obn::string_registry_id id;
};

typedef uint16_t LexerFSMState;
typedef LexerFSMState(*LexerNextStateFuncPtr)(obn::string_pool_chartype, LexerFSMState);

class LexerFSM
{
protected:
    LexerFSM() : m_currentState(0) {}

    LexerFSMState m_currentState;
    std::string m_currentToken;

public:
    virtual void reset() = 0;
    virtual bool updateState(obn::string_pool_chartype c, std::string& outString, TokenType& outType, TokenMetadata& outMeta) = 0;
    virtual bool goToEndState(std::string& outString, TokenType& outType, TokenMetadata& outMeta) = 0;
};

class DotConfigLexerFSM : public LexerFSM
{
private:
    enum State : LexerFSMState
    {
        START,
        COMMENT,
        KEY,
        BOOL_T,
        BOOL_R,
        BOOL_U,
        BOOL_E,//TODO do string, and number
        KEY_END,
        BOOL_END,
        NUMBER_END,
        STRING_END,
        INVALID_END,
        IGNORE_END,
        SIZE
    };

    std::array<LexerNextStateFuncPtr, State::SIZE> m_transitionFuncs;

    static LexerFSMState startTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState commentTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState keyTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState boolTTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState boolRTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState boolUTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState boolETransition(obn::string_pool_chartype c, LexerFSMState state);

public:
    DotConfigLexerFSM();

    void reset() override;
    bool updateState(obn::string_pool_chartype c, std::string& outString, TokenType& outType, TokenMetadata& outMeta) override;
    bool goToEndState(std::string& outString, TokenType& outType, TokenMetadata& outMeta) override;
};

compile_assert(sizeof(obn::string_pool_chartype) == sizeof(char));
class Lexer
{
private:
    std::unique_ptr<LexerFSM> m_fsm;
    
    bool tokenizeStream(std::istream& is, obn::string_registry& registry, std::vector<Token>& out);

public:
    Lexer(std::unique_ptr<LexerFSM> fsm);

    bool tokenizeFile(const gbt::FilePath& filepath, obn::string_registry& registry, std::vector<Token>& out);
    bool tokenizeString(const std::string& str, obn::string_registry& registry, std::vector<Token>& out);
};

}