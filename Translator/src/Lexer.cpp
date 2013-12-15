#include "Lexer.h"

#include <stddef.h>
#include <iostream>
#include <sstream>
int strToInt(std::string s)
{
    int res;
    std::stringstream ss;
    ss << s;
    ss >> res;
    return res;
}
float strToFloat(std::string s)
{
    float res;
    std::stringstream ss;
    ss << s;
    ss >> res;
    return res;
}
Lexer::Lexer(std::string source) :
        col(0), line(1), charsDone(-1), file_buffer(source) {
    file_length = source.size();
}

Token* Lexer::scan() {
    //for multisymbol tokens
    std::string nts;
    int c = getNextChar();
    SourceLocation sl = SourceLocation(line, col);
    switch (c) {
    case -1 :
        return new Token(token::END, SourceLocation(line, col));
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        bool isFloat = false;
        nts.push_back(c);
        c = lookNextChar();
        while((c >= '0' && c <= '9') || c == '.')
        {
            c = getNextChar();
            if(c == '.')
                isFloat = true;
            nts.push_back(c);
            c = lookNextChar();
        }
        if(isFloat)
            return new Token(token::FLOAT_LITERAL, sl, strToFloat(nts));
        else
            return new Token(token::INT_LITERAL, sl, strToInt(nts));
    }
    //case '#':
    //    return new Token(token::QUOTE,sl);
    case '{':
        return new Token(token::LF_CR_BRACKET, sl);
    case '}':
        return new Token(token::RT_CR_BRACKET, sl);
    case ';':
        return new Token(token::SEMICOLON, sl);
    case ':':
        return new Token(token::COLON, sl);
    case ',':
        return new Token(token::COMMA, sl);
    case '+':
        return new Token(token::PLUS, sl);
    case '-':
        return new Token(token::MINUS, sl);
    case '*':
        if (lookNextChar() == '*') {
            getNextChar();
            return new Token(token::POW, sl);
        } else
            return new Token(token::MULT, sl);
    case '/':
        return new Token(token::DIVIDE, sl);
    case '=':
        return new Token(token::ASSIGN, sl);
    default: {
        if(isIdentifierChar(c))
        {
            nts.push_back(c);
            c = lookNextChar(false);
        }
        while (isIdentifierChar(c)) {
            c = getNextChar(false);
            nts.push_back(c);
            c = lookNextChar(false);
        }
        //check if it keyword
        if (nts.compare("VAR") == 0) {
            return new Token(token::VAR, sl);
        }
        if (nts.compare("GO") == 0) {
            return new Token(token::GO, sl);
        }
        //types
        if (nts.compare("int") == 0) {
            return new Token(token::TYPE, sl, token::TYPE_INT);
        }
        if (nts.compare("float") == 0) {
            return new Token(token::TYPE, sl, token::TYPE_FLOAT);
        }
        //non keyword(id)
        return new Token(token::IDENTIFIER, sl, nts);
        break;
    }
    }
    return NULL;
}

int Lexer::lookNextChar(bool remSpace) {
    int c;
    bool done = true;
    int localcharsDone = charsDone;
    //read character
    while (done) {
        localcharsDone++;
        if (localcharsDone >= file_length) {
            //end of file
            return -1;
        }
        //for DOS line endings
        if (file_buffer[localcharsDone] == '\r'
                && file_buffer[localcharsDone + 1] == '\n') {
            localcharsDone = localcharsDone + 2;
        }
        //for *nix line endings
        if (file_buffer[localcharsDone] == '\n') {
            localcharsDone++;
        }
        c = file_buffer[localcharsDone];
        //remove whitespace
        if(remSpace)
        {
            if (c != ' ')
                done = false;
        }
        else
            done = false;

    }
    return c;
}

int Lexer::getNextChar(bool remSpace) {
    int c;
    bool done = true;
    //read character
    while (done) {
        charsDone++;
        col++;
        if (charsDone >= file_length) {
            //end of file
            return -1;
        }
        //for DOS line endings
        if (file_buffer[charsDone] == '\r'
                && file_buffer[charsDone + 1] == '\n') {
            line++;
            col = 1;
            charsDone = charsDone + 2;
        }
        //for *nix line endings
        if (file_buffer[charsDone] == '\n') {
            line++;
            col = 1;
            charsDone++;
        }
        c = file_buffer[charsDone];
        //remove whitespace
        if(remSpace)
        {
            if (c != ' ')
                done = false;
        }
        else
            done = false;
    }
    return c;
}

