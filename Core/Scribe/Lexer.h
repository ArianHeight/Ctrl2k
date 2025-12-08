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
        END_LINE,
        SYMBOL,
        NUMBER,
        NEGATIVE,
        DECIMAL,
        ZERO,
        BINARY,
        HEXADECIMAL,
        STRING_HEAD,
        STRING_BODY,
        STRING_TAIL,
        STRING_ESCAPE,
        STRING_ESCAPE_CHAR,
        END_LINE_END,
        SYMBOL_END,
        NUMBER_END,
        DECIMAL_END,
        BINARY_END,
        HEXADECIMAL_END,
        STRING_END,
        INVALID_END,
        IGNORE_END,
        SIZE
    };

    enum Metadata : TokenMetadata
    {
        DEFAULT,
        IS_FLOAT,
        IS_BINARY,
        IS_HEX,
        META_SIZE
    };

    std::array<LexerNextStateFuncPtr, State::SIZE> m_transitionFuncs;

    static LexerFSMState startTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState commentTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState endlineTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState symbolTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState numberTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState negativeTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState decimalTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState zeroTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState binaryTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState hexadecimalTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState stringheadTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState stringbodyTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState stringtailTransition(obn::string_pool_chartype c, LexerFSMState state);
    static LexerFSMState stringescapeTransition(obn::string_pool_chartype c, LexerFSMState state);

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