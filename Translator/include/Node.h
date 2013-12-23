#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cassert>

#include "Token.h"

namespace Seman {
class Visitor;
}

namespace node{

class Expression {
public:
    virtual void accept(Seman::Visitor *v) = 0;

    SourceLocation getSourceLocation() {
        return loc;
    }

    virtual ~Expression() = 0;
private:
    SourceLocation loc;
};

inline Expression::~Expression() {
}



class BinaryExpression: public Expression {
public:
    BinaryExpression(Expression *leftExpr, Expression *rightExpr,
            token::TokenKind operation, SourceLocation loc) :
            operation(operation), leftExpr(leftExpr), rightExpr(rightExpr), loc(
                    loc) {
    }

    Expression *getLeftExpression() const {
        return leftExpr;
    }

    Expression *getRightExpression() const {
        return rightExpr;
    }

    token::TokenKind getOperationType() const {
        return operation;
    }

    virtual void accept(Seman::Visitor *v);

private:
    Expression *leftExpr;
    Expression *rightExpr;
    token::TokenKind operation;
    SourceLocation loc;
};

class ParenthesesExpression: public Expression {
public:
    ParenthesesExpression(Expression *expr, SourceLocation loc) :
            expr(expr), loc(loc) {
    }

    Expression *getExpression() const {
        return expr;
    }

    virtual void accept(Seman::Visitor *v);

private:
    Expression *expr;
    SourceLocation loc;
};

class IntLiteral: public Expression {
public:
    IntLiteral(int value, SourceLocation loc) :
            value(value), loc(loc) {
    }

    int getValue() const {
        return value;
    }

    virtual void accept(Seman::Visitor *v);

private:
    int value;
    SourceLocation loc;
};

class FloatLiteral: public Expression {
public:
    FloatLiteral(float value, SourceLocation loc) :
            value(value), loc(loc) {
    }

    float getValue() const {
        return value;
    }

    virtual void accept(Seman::Visitor *v);

private:
    float value;
    SourceLocation loc;
};

class BooleanLiteral: public Expression {
public:
    BooleanLiteral(bool value, SourceLocation loc) :
            value(value), loc(loc) {
    }

    bool getValue() const {
        return value;
    }

    virtual void accept(Seman::Visitor *v);

private:
    bool value;
    SourceLocation loc;
};

class VarReferenceExpression: public Expression {
public:
    VarReferenceExpression(std::string name, SourceLocation loc) :
            name(std::move(name)), loc(loc) {
    }

    const std::string &getName() const {
        return name;
    }

    virtual void accept(Seman::Visitor *v);

private:
    std::string name;
    SourceLocation loc;
};

class Statement {
public:
    virtual void accept(Seman::Visitor *v) = 0;

    SourceLocation getSourceLocation() {
        return loc;
    }

    virtual ~Statement() {
    }
private:
    SourceLocation loc;
};



class IfStatement: public Statement {
public:
    IfStatement(Expression *condition, std::vector<node::Statement *> sLT,
            std::vector<node::Statement *> sLF, SourceLocation loc) :
            condition(condition), sLT(std::move(sLT)), sLF(std::move(sLF)), elseBody(
                    true), loc(loc) {
    }

    IfStatement(Expression *condition, std::vector<node::Statement *> sLT,
            SourceLocation loc) :
            condition(condition), sLT(std::move(sLT)), elseBody(false), loc(loc) {
    }

    bool hasElseBody() const {
        return elseBody;
    }

    Expression *getCondition() const {
        return condition;
    }

    const std::vector<node::Statement *> &getStatementsListTrue() const {
        return sLT;
    }

    const std::vector<node::Statement *> &getStatementsListFalse() const {
        return sLF;
    }

    virtual void accept(Seman::Visitor *v);

private:
    Expression *condition;
    std::vector<node::Statement *> sLT;
    std::vector<node::Statement *> sLF;
    bool elseBody;
    SourceLocation loc;
};

class WhileStatement: public Statement {
public:
    WhileStatement(Expression *condition, std::vector<node::Statement *> sL,
            SourceLocation loc) :
            condition(condition), sL(std::move(sL)), loc(loc) {
    }

    Expression *getCondition() const {
        return condition;
    }

    const std::vector<node::Statement *> &getStatementsList() const {
        return sL;
    }

    virtual void accept(Seman::Visitor *v);

private:
    Expression *condition;
    std::vector<node::Statement *> sL;
    SourceLocation loc;
};

class AssignmentStatement: public Statement {
public:
    AssignmentStatement(VarReferenceExpression *varRefExpr, Expression *Expr,
            SourceLocation loc) :
            varRefExpr(varRefExpr), Expr(Expr), loc(loc) {
    }

    VarReferenceExpression *getVarReferenceExpression() const {
        return varRefExpr;
    }

    Expression *getExpression() const {
        return Expr;
    }

    virtual void accept(Seman::Visitor *v);

private:
    VarReferenceExpression *varRefExpr;
    Expression *Expr;
    SourceLocation loc;
};

class VarType {
public:
    VarType(token::DataType data_type, SourceLocation loc) :
            data_type(data_type), loc(loc) {
    }

    token::DataType getDataType() {
        return data_type;
    }

    SourceLocation getSourceLocation() {
        return loc;
    }

    virtual ~VarType() {
    }
private:
    token::DataType data_type;
    SourceLocation loc;
};

class VarDecl {
public:
    VarDecl(std::string name, VarType *vT, SourceLocation loc) :
            name(std::move(name)), vT(vT), loc(loc) {
    }

    const std::string &getName() const {
        return name;
    }

    VarType *getVarType() const {
        return vT;
    }

    void accept(Seman::Visitor *v);

    SourceLocation getSourceLocation() {
        return loc;
    }

private:
    std::string name;
    VarType *vT; // var type
    SourceLocation loc;
};

class VarChapter {
public:
    VarChapter(std::vector<VarDecl *> vDl, SourceLocation loc) :
            vDl(std::move(vDl)), loc(loc) {
    }

    const std::vector<VarDecl *> &getVarDeclList() const {
        return vDl;
    }

    void accept(Seman::Visitor *v);

    SourceLocation getSourceLocation() {
        return loc;
    }

private:
    std::vector<VarDecl*> vDl; // var declaration list
    SourceLocation loc;
};

class GoChapter {
public:
    GoChapter(std::vector<Statement *> sl, SourceLocation loc) :
            sl(std::move(sl)), loc(loc) {
    }

    const std::vector<Statement *> &getStatementList() const {
        return sl;
    }

    void accept(Seman::Visitor *v);

    SourceLocation getSourceLocation() {
        return loc;
    }

private:
    std::vector<Statement*> sl; // statements list
    SourceLocation loc;
};
class Program{
public:
    Program(VarChapter *vC,GoChapter *gC, SourceLocation loc) :
            vC(vC),gC(gC),loc(loc) {
    }

    VarChapter *getVarChapter() const {
        return vC;
    }

    GoChapter *getGoChapter() const {
        return gC;
    }

    void accept(Seman::Visitor *v);

    SourceLocation getSourceLocation() {
        return loc;
    }

private:
    VarChapter *vC;
    GoChapter *gC;
    SourceLocation loc;  
};

}

#endif /* NODE_H */