#include <cstddef>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

#include "Lexer.h"
#include "Parser.h"
#include "Seman.h"

namespace {

std::vector<Token> tokenize(std::string source) {
    Lexer l(source);
    Token* t = NULL;
    std::vector<Token> result;

    while ((t = l.scan())->getKind() != token::END) {
        result.push_back(*t);
    }

    result.push_back(*t);

    return result;
}

node::Program *parse(std::vector<Token> toks, ErrorLoggerWrapper *eLW) {
    Parser p(toks, eLW);
    return p.parseProgram();
}

::testing::AssertionResult parseResult_isPass(node::Program *program, CapturingErrorLogger *eL, std::string source) {
    if(program == NULL) {   
        LoggerRecord lR = eL->getRecords()[0];  
        return testing::AssertionFailure() << "AST is broken: "
            << lR.message << " at " << lR.sl.getLine() << ":" << lR.sl.getColumn();
    }

    Seman::Sema sema;
    sema.checkAll(program);
    if(source.compare(sema.getSyntaxDumperResults()) != 0) {
        return testing::AssertionFailure() << "Source code and reconstructed code do not match!"
            << "Here is recostructed code: " << sema.getSyntaxDumperResults() << "\n"
            << "Here is source code: " << source;
    }
    
    return testing::AssertionSuccess();
}

::testing::AssertionResult parseResult_isFail(node::Program *program, CapturingErrorLogger *eL, unsigned line, unsigned column) {
    if(program != NULL) {       
        return testing::AssertionFailure() << "AST is not broken!";
    }
    
    SourceLocation sl = eL->getRecords()[0].sl;
    
    if(sl.getColumn() != column || sl.getLine() != line) {
        return testing::AssertionFailure() << "Unexpected error! "
            << "Expected at " << line << ":" << column << ". "
            << "Got at " << sl.getLine() << ":" << sl.getColumn();
    }
    
    return testing::AssertionSuccess();
}

::testing::AssertionResult tok_is(const Token &tok, token::TokenKind kind,
        unsigned line, unsigned column) {
    if (tok.getKind() != kind)
        return testing::AssertionFailure() << "Expected token kind " << kind
                << ", got " << tok.getKind();

    if (tok.getLocation().getLine() != line)
        return testing::AssertionFailure() << "Expected line " << line
                << ", got " << tok.getLocation().getLine();

    if (tok.getLocation().getColumn() != column)
        return testing::AssertionFailure() << "Expected column " << column
                << ", got " << tok.getLocation().getColumn();

    return testing::AssertionSuccess();
}

}

TEST(Lexer, DefaultTest) {
    std::string source =
        "VAR{\n"
            "i:int;\n"
            "i:float;\n"
            "}\n"
            "GO{\n"
            "i=1;\n"
            "}";
    std::vector<Token> toks = tokenize(source);
    ASSERT_TRUE(tok_is(toks[0], token::VAR, 1, 1));
    ASSERT_TRUE(tok_is(toks[1], token::LF_CR_BRACKET, 1, 4));
    ASSERT_TRUE(tok_is(toks[2], token::IDENTIFIER, 2, 1));
    ASSERT_TRUE(tok_is(toks[3], token::COLON, 2, 2));
    ASSERT_TRUE(tok_is(toks[4], token::TYPE, 2, 3));
    ASSERT_TRUE(tok_is(toks[5], token::SEMICOLON, 2, 6));
    ASSERT_TRUE(tok_is(toks[6], token::IDENTIFIER, 3, 1));
    ASSERT_TRUE(tok_is(toks[7], token::COLON, 3, 2));
    ASSERT_TRUE(tok_is(toks[8], token::TYPE, 3, 3));
    ASSERT_TRUE(tok_is(toks[9], token::SEMICOLON, 3, 8));
    ASSERT_TRUE(tok_is(toks[10], token::RT_CR_BRACKET, 4, 1));
    ASSERT_TRUE(tok_is(toks[11], token::GO, 5, 1));
    ASSERT_TRUE(tok_is(toks[12], token::LF_CR_BRACKET, 5, 3));
    ASSERT_TRUE(tok_is(toks[13], token::IDENTIFIER, 6, 1));
    ASSERT_TRUE(tok_is(toks[14], token::ASSIGN, 6, 2));
    ASSERT_TRUE(tok_is(toks[15], token::INT_LITERAL, 6, 3));
    ASSERT_TRUE(tok_is(toks[16], token::SEMICOLON, 6, 4));
    ASSERT_TRUE(tok_is(toks[17], token::RT_CR_BRACKET, 7, 1));
    ASSERT_TRUE(tok_is(toks[18], token::END, 7, 2));
}

TEST(Parser, DefaultTest) {
    std::string source = "VAR{\n"
            "i:int;\n"
            "i2:float;\n"
            "}\n"
            "GO{\n"
            "i = 1;\n"
            "}";

    CapturingErrorLogger *eL = new CapturingErrorLogger();
    ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);

    std::vector<Token> tokens = tokenize(source);
    node::Program *program = parse(tokens, eLW);
    ASSERT_TRUE(parseResult_isPass(program, eL, source));
}

TEST(Parser, FailTest) {
    std::string source = "VARR{\n"
            "i:int;\n"
            "}\n"
            "GO{\n"
            "i = 1;\n"
            "}";
    
    CapturingErrorLogger *eL = new CapturingErrorLogger();
    ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);
    
    std::vector<Token> tokens = tokenize(source);
    node::Program *program = parse(tokens, eLW);
    ASSERT_TRUE(parseResult_isFail(program, eL, 1, 1));
}

