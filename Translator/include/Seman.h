/*
 * Seman.h
 *
 *  Created on: 06.12.2012
 *      Author: Fors
 */

#ifndef SEMAN_H_
#define SEMAN_H_

#include <string>
#include <sstream>
#include <vector>
#include "Error.h"

namespace node {
class VarChapter;
class VarDecl;
class VarType;
}

namespace Seman {

class Visitor {
public:
    Visitor();
    virtual void checkVarChapter(node::VarChapter *vC);
    virtual void checkVarDecl(node::VarDecl *vD);
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
    void checkVarDeclList(const std::vector<node::VarDecl *> &vDL);
private:
    std::stringstream ss;
    ErrorLoggerWrapper *logger;
};


class Sema {
public:
    void checkAll(node::VarChapter *vC);
    std::string getSyntaxDumperResults();
private:
    Visitor *sD;
    DuplicatesCheck *dC;
};
}

#endif /* SEMAN_H_ */
