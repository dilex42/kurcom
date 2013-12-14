
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>

namespace token {

enum TokenKind {

    LF_CR_BRACKET = 1, //'{'
    RT_CR_BRACKET, //'}'

    SEMICOLON, //';'
    COLON, //':'
    COMMA, //','

    PLUS, //'+'
    MINUS, //'-'
    MULT, //'*'
    DIVIDE, //'/'
    POW, //'**'

    QUOTE, //'#'

    ASSIGN, //'='

    IDENTIFIER,

    INT_LITERAL,

    TYPE
};

enum DataType {
    TYPE_VOID = 0,
    TYPE_INT
};

inline std::string getKindString(TokenKind kind) {
    switch (kind) {
    case LF_CR_BRACKET:
        return std::string("LF_CR_BRACKET");
    case RT_CR_BRACKET:
        return std::string("RT_CR_BRACKET");
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
    case QUOTE:
        return std::string("QUOTE");
    case ASSIGN:
        return std::string("ASSIGN");
    case IDENTIFIER:
        return std::string("IDENTIFIER");
    case INT_LITERAL:
        return std::string("INT_LITERAL");
    case TYPE:
        return std::string("TYPE");
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
    }
    return "unknown";
}
;

inline std::string dataTypeString(DataType dT) {
    switch (dT) {
    case token::TYPE_INT:
        return "int";
    case token::TYPE_VOID:
        return "void";
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
                kind != token::ID &&
                kind != token::INT_LITERAL &&
                kind != token::TYPE 
        );
    }
    /**
     * For IDENTIFIER.
     **/
    Token(token::TokenKind kind, SourceLocation loc, std::string string_data) :
            kind(kind), loc(loc), id(string_data), stringData(string_data) {
        assert(kind == token::ID);
    }
    /**
     * For INT_LITERAL.
     **/
    Token(token::TokenKind kind, SourceLocation loc, int int_data) :
            kind(kind), loc(loc), id(""), intData(int_data) {
        assert(kind == token::INT_LITERAL);
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
        return stringData;
    }
    int getIntData() {
        return intData;
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
    token::DataType dataType;
};

#endif /* TOKEN_H_ */