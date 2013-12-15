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

node::VarChapter *parse(std::vector<Token> toks, ErrorLoggerWrapper *eLW) {
    Parser p(toks, eLW);
    return p.parseVarChapter();
}

::testing::AssertionResult parseResult_isPass(node::VarChapter *var_chapter, CapturingErrorLogger *eL, std::string source) {
    if(var_chapter == NULL) {   
        LoggerRecord lR = eL->getRecords()[0];  
        return testing::AssertionFailure() << "AST is broken: "
            << lR.message << " at " << lR.sl.getLine() << ":" << lR.sl.getColumn();
    }
    
    Seman::Sema sema;
    sema.checkAll(var_chapter);

    if(source.compare(sema.getSyntaxDumperResults()) != 0) {
        return testing::AssertionFailure() << "Source code and reconstructed code do not match!"
            << "Here is recostructed code: " << sema.getSyntaxDumperResults() << "\n"
            << "Here is source code: " << source;
    }
    
    return testing::AssertionSuccess();
}

::testing::AssertionResult parseResult_isFail(node::VarChapter *var_chapter, CapturingErrorLogger *eL, unsigned line, unsigned column) {
    if(var_chapter != NULL) {       
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

::testing::AssertionResult tok_is(const Token &tok, token::TokenKind kind) {
    if (tok.getKind() != kind)
        return testing::AssertionFailure() << "Expected token kind " << kind
                << ", got " << tok.getKind();

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
            "}";
    std::vector<Token> toks = tokenize(source);
    ASSERT_TRUE(tok_is(toks[0], token::VAR, 1, 1));
    ASSERT_TRUE(tok_is(toks[1], token::LF_CR_BRACKET, 1, 4));
    ASSERT_TRUE(tok_is(toks[2], token::IDENTIFIER, 2, 1));
    ASSERT_TRUE(tok_is(toks[3], token::COLON, 2, 2));
    ASSERT_TRUE(tok_is(toks[4], token::TYPE, 2, 3));
    ASSERT_TRUE(tok_is(toks[5], token::SEMICOLON, 2, 6));
    ASSERT_TRUE(tok_is(toks[6], token::RT_CR_BRACKET, 3, 1));
    ASSERT_TRUE(tok_is(toks[7], token::END, 3, 2));
}

TEST(Parser, DefaultTest) {
    std::string source = "VAR{\n"
            "i:int;\n"
            "}";

    CapturingErrorLogger *eL = new CapturingErrorLogger();
    ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);

    std::vector<Token> tokens = tokenize(source);
    node::VarChapter *var_chapter = parse(tokens, eLW);
    ASSERT_TRUE(parseResult_isPass(var_chapter, eL, source));
}

TEST(Parser, FailTest) {
    std::string source = "VARR{\n"
            "i:int;\n"
            "}";
    
    CapturingErrorLogger *eL = new CapturingErrorLogger();
    ErrorLoggerWrapper *eLW = new ErrorLoggerWrapper(eL);
    
    std::vector<Token> tokens = tokenize(source);
    node::VarChapter *var_chapter = parse(tokens, eLW);
    ASSERT_TRUE(parseResult_isFail(var_chapter, eL, 1, 1));
}

