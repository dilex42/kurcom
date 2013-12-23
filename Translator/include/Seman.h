#ifndef SEMAN_H_
#define SEMAN_H_

#include <string>
#include <sstream>
#include <vector>
#include "Error.h"

namespace node {
class Program;
class VarChapter;
class VarDecl;
class VarType;
class GoChapter;
class Statement;
class AssignmentStatement;
class IfStatement;
class WhileStatement;
class Expression;
class BinaryExpression;
class ParenthesesExpression;
class IntLiteral;
class FloatLiteral;
class BooleanLiteral;
class VarReferenceExpression;
}

namespace Seman {

class Visitor {
public:
    Visitor();
    virtual void checkProgram(node::Program *p);
    virtual void checkVarChapter(node::VarChapter *vC);
    virtual void checkGoChapter(node::GoChapter *gC);
    virtual void checkVarDecl(node::VarDecl *vD);

    virtual void checkAssignmentStatement(node::AssignmentStatement *s);
    virtual void checkIfStatement(node::IfStatement *s);
    virtual void checkWhileStatement(node::WhileStatement *s);

    virtual void checkBinaryExpression(node::BinaryExpression *e);
    virtual void checkParenthesesExpression(node::ParenthesesExpression *e);
    virtual void checkIntLiteral(node::IntLiteral *e);
    virtual void checkFloatLiteral(node::FloatLiteral *e);
    virtual void checkBooleanLiteral(node::BooleanLiteral *e);
    virtual void checkVarReferenceExpression(node::VarReferenceExpression *e);

    void checkStatementList(const std::vector<node::Statement *> &sL);
    void checkVarDeclList(const std::vector<node::VarDecl *> &vDL);
    std::string getResults();
    virtual ~Visitor();
private:
    std::stringstream ss;
};

inline Visitor::~Visitor() {
}

class DuplicatesCheck: public Visitor {
public:
    DuplicatesCheck() :
            Visitor() {
        StdoutErrorLogger *eL = new StdoutErrorLogger();
        logger = new ErrorLoggerWrapper(eL);
    }
    virtual void checkProgram(node::Program *p);
     void checkVarChapter(node::VarChapter *vC);
    void checkVarDeclList(const std::vector<node::VarDecl *> &vDL);
private:
    std::stringstream ss;
    ErrorLoggerWrapper *logger;
};


class Sema {
public:
    void checkAll(node::Program *p);
    std::string getSyntaxDumperResults();
private:
    Visitor *sD;
    DuplicatesCheck *dC;
};
}

#endif /* SEMAN_H_ */
