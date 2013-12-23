
#include "Seman.h"
#include "Node.h"
#include <iostream>

using namespace Seman;

Visitor::Visitor() :
        ss() {
}

void Visitor::checkProgram(node::Program *p){
    checkVarChapter(p->getVarChapter());
    checkGoChapter(p->getGoChapter());
}
void Visitor::checkVarChapter(node::VarChapter *vC) {
    std::vector<node::VarDecl *> vDL = vC->getVarDeclList();
    ss << "VAR{";
    checkVarDeclList(vDL);
    ss << "\n}\n";
}

void Visitor::checkVarDeclList(const std::vector<node::VarDecl *> &vDL) {
    for (std::vector<node::VarDecl *>::const_iterator it = vDL.begin();
            it != vDL.end(); ++it) {
        ss << "\n";
        checkVarDecl(*it);
        ss << ";";
    }
}

void Visitor::checkVarDecl(node::VarDecl *vD) {
    ss << vD->getName() << ":"
            << token::dataTypeString(vD->getVarType()->getDataType());
}

void Visitor::checkGoChapter(node::GoChapter *gC) {
    std::vector<node::Statement *> sL = gC->getStatementList();
    ss << "GO{\n";
    checkStatementList(sL);
    ss << "\n}";
}

void Visitor::checkStatementList(const std::vector<node::Statement *> &sL) {
    for (std::vector<node::Statement *>::const_iterator it = sL.begin();
            it != sL.end(); ++it) {
        sL.at(it - sL.begin())->accept(this);
    }
}

void Visitor::checkIfStatement(node::IfStatement *s) {
    ss << "if(";
    s->getCondition()->accept(this);
    ss << ") {";
    checkStatementList(s->getStatementsListTrue());
    if (s->hasElseBody()) {
        ss << "} else {";
        checkStatementList(s->getStatementsListFalse());
    }
    ss << "}";
}
void Visitor::checkWhileStatement(node::WhileStatement *s) {
    ss << "while(";
    s->getCondition()->accept(this);
    ss << ") {";
    checkStatementList(s->getStatementsList());
    ss << "}";
}

void Visitor::checkAssignmentStatement(node::AssignmentStatement *s) {
    s->getVarReferenceExpression()->accept(this);
    ss << " = ";
    s->getExpression()->accept(this);
    ss << ";";
}

void Visitor::checkBinaryExpression(node::BinaryExpression *e) {
    node::Expression *eL = e->getLeftExpression();
    node::Expression *eR = e->getRightExpression();

    if (eL != NULL)
        eL->accept(this);

    ss << token::getSourceString(e->getOperationType());
    eR->accept(this);
}
void Visitor::checkParenthesesExpression(node::ParenthesesExpression *e) {
    ss << "(";
    e->getExpression()->accept(this);
    ss << ")";
}
void Visitor::checkIntLiteral(node::IntLiteral *e) {
    ss << e->getValue();
}
void Visitor::checkFloatLiteral(node::FloatLiteral *e) {
    ss << e->getValue();
}

void Visitor::checkBooleanLiteral(node::BooleanLiteral *e) {
    ss << e->getValue();
}
void Visitor::checkVarReferenceExpression(node::VarReferenceExpression *e) {
    ss << e->getName();
}


std::string Visitor::getResults() {
    return ss.str();
}

void DuplicatesCheck::checkProgram(node::Program *p){
    checkVarChapter(p->getVarChapter());
}

void DuplicatesCheck::checkVarChapter(node::VarChapter *vC){
    checkVarDeclList(vC->getVarDeclList());
}

void DuplicatesCheck::checkVarDeclList(
        const std::vector<node::VarDecl *> &vDL) {
    for (std::vector<node::VarDecl *>::const_iterator it1 = vDL.begin();
            it1 != vDL.end(); ++it1) {
        std::string vN = (*it1)->getName();
        for (std::vector<node::VarDecl *>::const_iterator it2 = it1;
                it2 != vDL.end(); ++it2) {
            if (it1 != it2 && (*it2)->getName() == vN) {
                logger->error((*it2)->getSourceLocation(), "Duplicate variable name found: ", vN);
            }
        }
    }
}


void Sema::checkAll(node::Program *p) {
    sD = new Visitor();
    dC = new DuplicatesCheck();
    p->accept(sD);
    p->accept(dC);
}

std::string Sema::getSyntaxDumperResults() {
    return sD->getResults();
}

