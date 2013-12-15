
#include "Node.h"
#include "Seman.h"

using namespace node;

void VarDecl::accept(Seman::Visitor *v) {
    v->checkVarDecl(this);
}

void VarChapter::accept(Seman::Visitor *v) {
    v->checkVarChapter(this);
}

