
#include "Seman.h"
#include "Node.h"
#include <iostream>

using namespace Seman;

Visitor::Visitor() :
        ss() {
}

void Visitor::checkVarChapter(node::VarChapter *vC) {
    std::vector<node::VarDecl *> vDL = vC->getVarDeclList();
    ss << "VAR{\n";
    checkVarDeclList(vDL);
    ss << "\n}";
}

void Visitor::checkVarDeclList(const std::vector<node::VarDecl *> &vDL) {
    for (std::vector<node::VarDecl *>::const_iterator it = vDL.begin();
            it != vDL.end(); ++it) {
        checkVarDecl(*it);
        ss << ";";
    }
}

void Visitor::checkVarDecl(node::VarDecl *vD) {
    ss << vD->getName() << ":"
            << token::dataTypeString(vD->getVarType()->getDataType());
}

std::string Visitor::getResults() {
    return ss.str();
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


void Sema::checkAll(node::VarChapter *vC) {
    sD = new Visitor();
    dC = new DuplicatesCheck();
    vC->accept(sD);
    vC->accept(dC);
}

std::string Sema::getSyntaxDumperResults() {
    return sD->getResults();
}

