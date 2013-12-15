#include "Parser.h"

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
    if(!is_eof()) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected VAR");
        return NULL;
    }   
    
    return new node::VarChapter(*vDl, startLoc);
};

std::vector<node::VarDecl *> *Parser::parseVarDeclList() {
    std::vector<node::VarDecl *> *vDL = new std::vector<node::VarDecl *>;
    std::string name;
    
    while(next_kind() != token::RT_CR_BRACKET) {
        if(next_kind() != token::IDENTIFIER) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected ID");
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



/*std::vector<node::Statement *> *Parser::parseStatementList() {
    node::Statement *s;
    std::vector<node::Statement *> *sL = new std::vector<node::Statement *>;

    while(next_kind() != token::RT_CR_BRACKET && !is_eof()) {
        s = parseAssignmentStatement();
        }
        
        if(s == NULL)
            return NULL;
            
        sL->push_back(s);
    }
    
    return sL;  
};



node::Statement *Parser::parseAssignmentStatement() {
    node::Expression *bE = parseBooleanExpression();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(bE == NULL)
        return NULL;

    if(next_kind() == token::EQUALS) {
        consume_token();
        node::Expression *bEV = parseBooleanExpression();
        
        if(bEV == NULL)
            return NULL;
        
        if(next_kind() != token::SEMICOLON) {
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
            return NULL;
        }
        
        consume_token();        
        return new node::AssignmentStatement(bE, bEV, startLoc);
    }
    
    if(next_kind() != token::SEMICOLON) {
        logger->error(point_token(), "Unexpected ", recognize_token(), ", expected SEMICOLON");
        return NULL;
    }
    
    consume_token();    
    return new node::ExpressionStatement(bE, startLoc);
};

node::Expression *Parser::parseBooleanExpression() {
    node::Expression *leftExpr = parseBooleanTerm();
    node::Expression *rightExpr;
    token::TokenKind tK;
    SourceLocation startLoc = source[next_index].getLocation();
    
    while(next_kind() == token::DOR) {
        tK = next_kind();
        consume_token();
        rightExpr = parseBooleanTerm();
        
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
    node::Expression *leftExpr = parseMathSignedFactor();
    SourceLocation startLoc = source[next_index].getLocation();
    
    if(leftExpr == NULL)
        return NULL;
    
    node::Expression *rightExpr;
    token::TokenKind tK;
    
    if(next_kind() == token::POW) {
        tK = next_kind();
        consume_token();
        rightExpr = parseMathSignedFactor();
        
        if(rightExpr == NULL)
            return NULL;
        
        leftExpr = new node::BinaryExpression(leftExpr, rightExpr, tK, startLoc);
    }
    
    return leftExpr;
};


node::Expression *Parser::parseOperand() {
    uint32_t int_value;
    double float_value;
    std::string str_value;
    bool boolean_value;
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
        case token::STRING_LITERAL: {
            str_value = next_token().getStringData();
            consume_token();
            return new node::StringLiteral(str_value, startLoc);
        }
        case token::BOOLEAN_LITERAL: {
            boolean_value = next_token().getIntData() != 0;
            consume_token();
            return new node::BooleanLiteral(boolean_value, startLoc);
        }
        case token::ID: {
            str_value = next_token().getStringData();
            consume_token();
            
            switch(next_kind()) {
                case token::LF_PARENTHESES:
                    return parseFuncCallExpression(str_value);
                case token::LF_SQ_BRACKET:
                    return parseArrayAccessExpression(str_value);
                default:
                    return new node::VarReferenceExpression(str_value, startLoc);
            }
        }
        default:
            logger->error(point_token(), "Unexpected ", recognize_token(), ", expected LF_PARENTHESES or INT ",
                "or FLOAT or STRING or BOOLEAN or ID");
            return NULL;
    }
};

*/