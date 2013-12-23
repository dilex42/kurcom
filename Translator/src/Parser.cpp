#include "Parser.h"
#include <iostream>

node::Program *Parser::parseProgram(){
    node::VarChapter *vC;
    node::GoChapter *gC;
    SourceLocation startLoc = source[next_index].getLocation();
    vC = parseVarChapter();
    gC = parseGoChapter();
    if(vC==NULL || gC==NULL){
        return NULL;
    }
    return new node::Program(vC,gC,startLoc);
}


node::VarChapter *Parser::parseVarChapter() {
    std::vector<node::VarDecl *> *vDl;
    SourceLocation startLoc = source[next_index].getLocation();
    
    if (next_kind() == token::VAR){
        consume_token();
        if(next_kind() == token::LF_CR_BRACKET) {
            consume_token();
            vDl = parseVarDeclList();

            if(vDl == NULL)
            return NULL;
        }
        else{
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
            return NULL;
        }
    }else{
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected VAR");
        return NULL;
    }
    if(next_kind() != token::RT_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
        return NULL;
    }
    consume_token();   
    
    return new node::VarChapter(*vDl, startLoc);
};

std::vector<node::VarDecl *> *Parser::parseVarDeclList() {
    std::vector<node::VarDecl *> *vDL = new std::vector<node::VarDecl *>;
    std::string name;
    
    while(next_kind() != token::RT_CR_BRACKET) {
        if(next_kind() != token::IDENTIFIER) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected IDENTIFIER");
            return NULL;
        }
        SourceLocation startLoc = source[next_index].getLocation();
        
        name = next_token().getID();
        consume_token();
        
        if(next_kind() != token::COLON) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected COLON");
            return NULL;
        }
        
        consume_token();
        
        if(next_kind() != token::TYPE) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected TYPE");
            return NULL;
        }
        
        node::VarType *vT = parseVarType();
        
        vDL->push_back(new node::VarDecl(name, vT, startLoc));
        
        if(next_kind() != token::SEMICOLON) {
            break;
        }
        
        consume_token();
    }
    
    return vDL;
};


node::VarType *Parser::parseVarType() {
    assert(next_kind() == token::TYPE);
    
    token::DataType type = next_token().getDataType();
    consume_token();
    SourceLocation startLoc = source[next_index].getLocation();    

    return new node::VarType(type, startLoc);
};

node::GoChapter *Parser::parseGoChapter() {
    std::vector<node::Statement *> *sl;
    SourceLocation startLoc = source[next_index].getLocation();
    
    if (next_kind() == token::GO){
        consume_token();
        if(next_kind() == token::LF_CR_BRACKET) {
            consume_token();
            sl = parseStatementList();

            if(sl == NULL)
            return NULL;
        }
        else{
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
            return NULL;
        }
    }else{
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected GO");
        return NULL;
    }
    if(next_kind() != token::RT_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
        return NULL;
    }
    consume_token();   
    if(!is_eof()) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected GO");
        return NULL;
    }   
    
    return new node::GoChapter(*sl, startLoc);
};
std::vector<node::Statement *> *Parser::parseStatementList() {
    node::Statement *s;
    std::vector<node::Statement *> *sL = new std::vector<node::Statement *>;

    while(next_kind() != token::RT_CR_BRACKET && !is_eof()) {
        switch(next_kind()) {
            case token::IF: {
                s = parseIfStatement();
                break;
            }
            case token::WHILE: {
                s = parseWhileStatement();
                break;
            }
            default: {
                s = parseAssignmentStatement();
            }
        }
        
        if(s == NULL)
            return NULL;
            
        sL->push_back(s);
    }
    
    return sL;  
};

node::Statement *Parser::parseIfStatement() {
    assert(next_kind() == token::IF);
    consume_token();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(next_kind() != token::LF_PARENTHESES) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
        return NULL;
    }
    
    consume_token();
    node::Expression *bE = parseBooleanExpression();
    
    if(bE == NULL)
        return NULL;
    
    if(next_kind() != token::RT_PARENTHESES) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
        return NULL;
    }
    
    consume_token();
    
    if(next_kind() != token::LF_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
        return NULL;
    }
    
    consume_token();
    std::vector<node::Statement *> *sLT = parseStatementList();
    
    if(sLT == NULL)
        return NULL;
    
    if(next_kind() != token::RT_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
        return NULL;
    }
    
    consume_token();
    
    if(next_kind() == token::ELSE) {
        consume_token();
        
        if(next_kind() != token::LF_CR_BRACKET) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
            return NULL;
        }
        
        consume_token();
        std::vector<node::Statement *> *sLF = parseStatementList();
        
        if(sLF == NULL)
            return NULL;
        
        if(next_kind() != token::RT_CR_BRACKET) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
            return NULL;
        }
        
        consume_token();
        return new node::IfStatement(bE, *sLT, *sLF, startLoc);
    }
    
    return new node::IfStatement(bE, *sLT, startLoc);
};

node::Statement *Parser::parseWhileStatement() {
    assert(next_kind() == token::WHILE);
    consume_token();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(next_kind() != token::LF_PARENTHESES) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES");
        return NULL;
    }
    
    consume_token();
    node::Expression *bE = parseBooleanExpression();
    
    if(bE == NULL)
        return NULL;
    
    if(next_kind() != token::RT_PARENTHESES) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
        return NULL;
    }
    
    consume_token();
    
    if(next_kind() != token::LF_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_CR_BRACKET");
        return NULL;
    }
    
    consume_token();
    std::vector<node::Statement *> *sL = parseStatementList();
    
    if(next_kind() != token::RT_CR_BRACKET) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_CR_BRACKET");
        return NULL;
    }
    
    consume_token();    
    return new node::WhileStatement(bE, *sL, startLoc);
};

node::Statement *Parser::parseAssignmentStatement() {
    assert(next_kind() == token::IDENTIFIER);
    SourceLocation startLoc = source[next_index].getLocation();
    node::VarReferenceExpression *vR = new node::VarReferenceExpression(next_token().getID(),startLoc);
    consume_token();
    
    if(next_kind() != token::ASSIGN) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ASSIGN");
        return NULL;
    }
    consume_token();
    node::Expression *bE = parseBooleanExpression();
        
    if(bE == NULL)
        return NULL;
        
    if(next_kind() != token::SEMICOLON) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
        return NULL;
    }
        
    consume_token();        
    return new node::AssignmentStatement(vR, bE, startLoc);   
};

node::Expression *Parser::parseBooleanExpression() {
    node::Expression *leftExpr = parseBooleanTerm();
    node::Expression *rightExpr;
    token::TokenKind tK;
    SourceLocation startLoc = source[next_index].getLocation();
    
    while(next_kind() == token::OR) {
        tK = next_kind();
        consume_token();
        rightExpr = parseBooleanTerm();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};
node::Expression *Parser::parseBooleanTerm() {
    node::Expression *leftExpr = parseBooleanRelation();
    node::Expression *rightExpr;
    token::TokenKind tK;
    SourceLocation startLoc = source[next_index].getLocation();
    
    while(next_kind() == token::AND) {
        
        tK = next_kind();
        consume_token();
        rightExpr = parseBooleanRelation();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};

node::Expression *Parser::parseBooleanRelation() {
    node::Expression *leftExpr = parseMathExpression();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(leftExpr == NULL)
        return NULL;
    
    node::Expression *rightExpr;
    token::TokenKind tK;
    
    while(next_kind() == token::EQUALS || next_kind() == token::NEQUALS ||
        next_kind() == token::LESS_EQUALS || next_kind() == token::MORE_EQUALS ||
        next_kind() == token::MORE || next_kind() == token::LESS) {
        tK = next_kind();
        consume_token();
        rightExpr = parseMathExpression();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};

node::Expression *Parser::parseMathExpression() {
    node::Expression *leftExpr = parseMathTerm();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(leftExpr == NULL)
        return NULL;
    
    node::Expression *rightExpr;
    token::TokenKind tK;
    
    while(next_kind() == token::PLUS || next_kind() == token::MINUS) {
        tK = next_kind();
        consume_token();
        rightExpr = parseMathTerm();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};

node::Expression *Parser::parseMathTerm() {
    node::Expression *leftExpr = parseMathPow();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(leftExpr == NULL)
        return NULL;
    
    node::Expression *rightExpr;
    token::TokenKind tK;
    
    if(next_kind() == token::MULT || next_kind() == token::DIVIDE) {
        tK = next_kind();
        consume_token();
        rightExpr = parseMathPow();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};

node::Expression *Parser::parseMathPow() {
    node::Expression *leftExpr = parseOperand();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(leftExpr == NULL)
        return NULL;
    
    node::Expression *rightExpr;
    token::TokenKind tK;
    
    if(next_kind() == token::POW) {
        tK = next_kind();
        consume_token();
        rightExpr = parseOperand();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};


node::Expression *Parser::parseParenthesesExpression() {
    assert(next_kind() == token::LF_PARENTHESES);
    consume_token();
    SourceLocation startLoc = source[next_index].getLocation();
    
    node::Expression *e = parseBooleanExpression();
    
    if(e == NULL)
        return NULL;

    if(next_kind() != token::RT_PARENTHESES) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected RT_PARENTHESES");
        return NULL;
    }
    
    consume_token();
    return new node::ParenthesesExpression(e, startLoc);
};

node::Expression *Parser::parseOperand() {
    int int_value;
    float float_value;
    bool boolean_value;
    std::string id_value;
    SourceLocation startLoc = source[next_index].getLocation();
    
    switch(next_kind()) {
        case token::LF_PARENTHESES:
            return parseParenthesesExpression();
        case token::INT_LITERAL: {
            int_value = next_token().getIntData();
            consume_token();
            return new node::IntLiteral(int_value, startLoc);
        }
        case token::FLOAT_LITERAL: {
            float_value = next_token().getFloatData();
            consume_token();
            return new node::FloatLiteral(float_value, startLoc);
        }
        case token::BOOLEAN_LITERAL: {
            boolean_value = next_token().getIntData() != 0;
            consume_token();
            return new node::BooleanLiteral(boolean_value, startLoc);
        }
        case token::IDENTIFIER: {
            id_value = next_token().getID();
            consume_token();
            return new node::VarReferenceExpression(id_value, startLoc);
        }
        default:
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES or INT ",
                "or FLOAT or BOOLEAN or IDENTIFIER");
            return NULL;
    }
};

