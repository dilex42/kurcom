
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>

namespace token {

enum TokenKind {

    LF_CR_BRACKET = 1, //'{'
    RT_CR_BRACKET, //'}'
    LF_PARENTHESES, //'('
    RT_PARENTHESES, //')'

    SEMICOLON, //';'
    COLON, //':'
    COMMA, //','

    PLUS, //'+'
    MINUS, //'-'
    MULT, //'*'
    DIVIDE, //'/'
    POW, //'**'

    LESS, //'<'
    MORE, //'>'

    LESS_EQUALS, //'<='
    MORE_EQUALS, //'>='

    EQUALS, //'=='
    NEQUALS, //'!='
    AND, //'and'
    OR, //'or'

    //QUOTE, //'#'

    ASSIGN, //'='

    IDENTIFIER,

    INT_LITERAL,
    FLOAT_LITERAL,
    BOOLEAN_LITERAL,

    WHILE,
    IF,
    ELSE,

    TYPE,
    VAR,
    GO,
    END
};

enum DataType {
    TYPE_INT = 1,
    TYPE_FLOAT,
    TYPE_BOOLEAN
};

inline std::string getKindString(TokenKind kind) {
    switch (kind) {
    case LF_CR_BRACKET:
        return std::string("LF_CR_BRACKET");
    case RT_CR_BRACKET:
        return std::string("RT_CR_BRACKET");
    case LF_PARENTHESES:
        return std::string("LF_PARENTHESES");
    case RT_PARENTHESES:
        return std::string("RT_PARENTHESES");
    case SEMICOLON:
        return std::string("SEMICOLON");
    case COLON:
        return std::string("COLON");
    case COMMA:
        return std::string("COMMA");
    case PLUS:
        return std::string("PLUS");
    case MINUS:
        return std::string("MINUS");
    case MULT:
        return std::string("MULT");
    case DIVIDE:
        return std::string("DIVIDE");
    case POW:
        return std::string("POW");
    case LESS:
        return std::string("LESS");
    case MORE:
        return std::string("MORE");
    case LESS_EQUALS:
        return std::string("LESS_EQUALS");
    case MORE_EQUALS:
        return std::string("MORE_EQUALS");
    case EQUALS:
        return std::string("EQUALS");
    case NEQUALS:
        return std::string("NEQUALS");
    case AND:
        return std::string("AND");
    case OR:
        return std::string("OR");  
    //case QUOTE:
    //    return std::string("QUOTE");
    case ASSIGN:
        return std::string("ASSIGN");
    case IDENTIFIER:
        return std::string("IDENTIFIER");
    case INT_LITERAL:
        return std::string("INT_LITERAL");
    case FLOAT_LITERAL:
        return std::string("FLOAT_LITERAL");
    case BOOLEAN_LITERAL:
        return std::string("BOOLEAN_LITERAL");
    case WHILE:
        return std::string("WHILE");
    case ELSE:
        return std::string("ELSE");
    case IF:
        return std::string("IF");
    case TYPE:
        return std::string("TYPE");
    case VAR:
        return std::string("VAR");
    case GO:
        return std::string("GO");
    case END:
        return std::string("END");
    }
    return "unknown";
}
;

inline std::string getSourceString(TokenKind kind) {
    switch (kind) {
    case PLUS:
        return "+";
    case MINUS:
        return "-";
    case MULT:
        return "*";
    case DIVIDE:
        return "/";
    case POW:
        return "**";
    case LESS:
        return "<";
    case MORE:
        return ">";
    case LESS_EQUALS:
        return "<=";
    case MORE_EQUALS:
        return ">=";
    case EQUALS:
        return "==";
    case NEQUALS:
        return "!=";
    case AND:
        return "and";
    case OR:
        return "or";
    }
    return "unknown";
}
;

inline std::string dataTypeString(DataType dT) {
    switch (dT) {
    case token::TYPE_INT:
        return "int";
    case token::TYPE_FLOAT:
        return "float";
    case token::TYPE_BOOLEAN:
        return "boolean";
    }
    return "unknown";
}

}

struct SourceLocation {
    int line;
    int col;
    SourceLocation(int line, int col) :
            line(line), col(col) {
    }
    SourceLocation() :
            line(0), col(0) {
    }
    unsigned getLine() const {
        return line;
    }
    unsigned getColumn() const {
        return col;
    }
    std::string toString() {
        std::stringstream ss;
        ss << "line:" << line << "; col:" << col;
        return ss.str();
    }
};

class Token {
public:
    /**
     *  Constructor for simple tokens(like {}) and keywords.
     **/
    Token(token::TokenKind kind, SourceLocation loc) :
            kind(kind), loc(loc), id("") {
        assert(
                kind != token::IDENTIFIER &&
                kind != token::INT_LITERAL &&
                kind != token::FLOAT_LITERAL &&
                kind != token::BOOLEAN_LITERAL &&
                kind != token::TYPE 
        );
    }
    /**
     * For IDENTIFIER.
     **/
    Token(token::TokenKind kind, SourceLocation loc, std::string string_data) :
            kind(kind), loc(loc), id(string_data) {
        assert(kind == token::IDENTIFIER);
    }
    /**
     * For INT_LITERAL.
     **/
    Token(token::TokenKind kind, SourceLocation loc, int int_data) :
            kind(kind), loc(loc), id(""), intData(int_data) {
        assert(kind == token::INT_LITERAL);
    }
    /**
     * For FLOAT_LITERAL.
     **/
    Token(token::TokenKind kind, SourceLocation loc, float float_data) :
            kind(kind), loc(loc), id(""), floatData(float_data) {
        assert(kind == token::FLOAT_LITERAL);
    }
    /**
     * For BOOLEAN_LITERAL.
     */
    Token(token::TokenKind kind, SourceLocation loc, bool dataBoolean) :
            kind(kind), loc(loc), dataBoolean(dataBoolean) {
        assert(kind == token::BOOLEAN_LITERAL);
    }
    /**
     * For TYPE.
     **/
    Token(token::TokenKind kind, SourceLocation loc, token::DataType dataType) :
            kind(kind), loc(loc), id(""), dataType(dataType) {
        assert(kind == token::TYPE);
    }
    token::TokenKind getKind() const {
        return kind;
    }
    std::string getID() {
        return id;
    }
    int getIntData() {
        return intData;
    }
    float getFloatData() {
        return floatData;
    }
    token::DataType getDataType() const {
        return dataType;
    }
    SourceLocation getLocation() const {
        return loc;
    }
    ~Token() {
    }
private:
    token::TokenKind kind;
    SourceLocation loc;
    //id for ID tokens
    std::string id;
    //data for tokens with some data
    int intData;
    float floatData;
    bool dataBoolean;
    token::DataType dataType;
};

#endif /* TOKEN_H_ */
