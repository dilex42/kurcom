#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <cassert>
#include <vector>

#include <ostream>
#include <cstdint>

#include "Token.h"
#include "Node.h"
#include "Error.h"

class Parser {
public:
    Parser(std::vector<Token> source, ErrorLoggerWrapper *logger) : 
        source(std::move(source)),
        logger(logger),
        next_index(0) {}
        
    node::Program *parseProgram();
    ErrorLoggerWrapper *getLogger() const {
        return logger;
    }
    
private:
    node::VarChapter *parseVarChapter();
    node::GoChapter *parseGoChapter();
    node::VarType *parseVarType();  
    std::vector<node::VarDecl *> *parseVarDeclList();

    std::vector<node::Statement *> *parseStatementList();
    node::Statement *parseIfStatement();
    node::Statement *parseWhileStatement();
    node::Statement *parseAssignmentStatement();    

    node::Expression *parseBooleanExpression();
    node::Expression *parseBooleanTerm();
    node::Expression *parseBooleanRelation();
    node::Expression *parseMathExpression();
    node::Expression *parseMathTerm();
    node::Expression *parseMathPow();
    node::Expression *parseParenthesesExpression();
    node::Expression *parseOperand();   
    
    Token next_token() {
        return source[next_index];
    }
    
    token::TokenKind next_kind() {
        return next_token().getKind();
    }
    
    bool is_eof() {
        return next_kind() == token::END;
    }
    
    void consume_token() {
        assert(!is_eof());
        next_index++;
    }
    
    SourceLocation point_token() {
        return next_token().getLocation();
    }
    
    std::string recognize_token() {
        return token::getKindString(next_kind());
    }
    
    std::vector<Token> source;
    size_t next_index;
    
    ErrorLoggerWrapper *logger;
};

#endif /* PARSER_H */
