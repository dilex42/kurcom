
#include "Node.h"
#include "Seman.h"

using namespace node;

void Program::accept(Seman::Visitor *v){
    v->checkProgram(this);
}

void VarChapter::accept(Seman::Visitor *v) {
    v->checkVarChapter(this);
}

void VarDecl::accept(Seman::Visitor *v) {
    v->checkVarDecl(this);
}

void GoChapter::accept(Seman::Visitor *v) {
    v->checkGoChapter(this);
}


void AssignmentStatement::accept(Seman::Visitor *v) {
    v->checkAssignmentStatement(this);
}

void IfStatement::accept(Seman::Visitor *v) {
    v->checkIfStatement(this);
}

void WhileStatement::accept(Seman::Visitor *v) {
    v->checkWhileStatement(this);
}

void BinaryExpression::accept(Seman::Visitor *v) {
    v->checkBinaryExpression(this);
}

void ParenthesesExpression::accept(Seman::Visitor *v) {
    v->checkParenthesesExpression(this);
}

void VarReferenceExpression::accept(Seman::Visitor *v) {
    v->checkVarReferenceExpression(this);
}

void IntLiteral::accept(Seman::Visitor *v){
    v->checkIntLiteral(this);
}
void FloatLiteral::accept(Seman::Visitor *v){
    v->checkFloatLiteral(this);
}

void BooleanLiteral::accept(Seman::Visitor *v) {
    v->checkBooleanLiteral(this);
}













