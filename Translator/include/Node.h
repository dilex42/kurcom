#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cassert>

#include "Token.h"

namespace Seman {
class Visitor;
}

namespace node{

/*class Expression {
public:
    //virtual void accept(Seman::ASTVisitor *v) = 0;

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

    //virtual void accept(Seman::ASTVisitor *v);

private:
    Expression *leftExpr;
    Expression *rightExpr;
    token::TokenKind operation;
    SourceLocation loc;
};

class IntLiteral: public Expression {
public:
    IntLiteral(uint32_t value, SourceLocation loc) :
            value(value), loc(loc) {
    }

    uint32_t getValue() const {
        return value;
    }

    //virtual void accept(Seman::ASTVisitor *v);

private:
    uint32_t value;
    SourceLocation loc;
};

class FloatLiteral: public Expression {
public:
    FloatLiteral(double value, SourceLocation loc) :
            value(value), loc(loc) {
    }

    double getValue() const {
        return value;
    }

    //virtual void accept(Seman::ASTVisitor *v);

private:
    double value;
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

    //virtual void accept(Seman::ASTVisitor *v);

private:
    std::string name;
    SourceLocation loc;
};

class Statement {
public:
    //virtual void accept(Seman::ASTVisitor *v) = 0;

    SourceLocation getSourceLocation() {
        return loc;
    }

    virtual ~Statement() {
    }
private:
    SourceLocation loc;
};

class AssignmentStatement: public Statement {
public:
    AssignmentStatement(VarReferenceExpression *varExpr, Expression *rightExpr,
            SourceLocation loc) :
            varExpr(varExpr), rightExpr(rightExpr), loc(loc) {
    }

    VarReferenceExpression *getVarExpression() const {
        return varExpr;
    }

    Expression *getRightExpression() const {
        return rightExpr;
    }

    //virtual void accept(Seman::ASTVisitor *v);

private:
    Expression *varExpr;
    Expression *rightExpr;
    SourceLocation loc;
};*/

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
/*class Go {
public:
    Program(std::vector<Statement *> statements, SourceLocation loc) :
            statements(std::move(statements)), loc(loc) {
    }

    const std::vector<Statement *> &getFuncDeclList() const {
        return fDL;
    }

    void accept(Seman::ASTVisitor *v);

    SourceLocation getSourceLocation() {
        return loc;
    }

private:
    std::vector<FuncDecl*> fDL; // function declaration list
    SourceLocation loc;
};*/

}

#endif /* NODE_H */