#include "Context.h"

Context::Context(SymbolTable* var, SymbolTable* func) {
    m_var = new SymbolTable("var");
    m_var->setParent(var);
    m_func = func;
}

void Context::put(std::string table, std::string item, ASTree* ast) {
    if (!table.compare("var")) {
        m_var->put(item, ast);
    } else if (!table.compare("func")) {
        m_func->put(item, ast);
    } else {
        printf("%s, %d, error\n", __func__, __LINE__);
    }
}

ASTree* Context::get(std::string table, std::string item) {
    if (!table.compare("var")) {
        return m_var->get(item);
    } else if (!table.compare("func")) {
        return m_func->get(item);
    } else {
        printf("%s, %d, error\n", __func__, __LINE__);
        return NULL;
    }
}

void Context::setParentContext(Context* cxt) {
    m_parent = cxt;
}

Context* Context::getParentContext() {
    return m_parent;
}

ASTree* Context::getNext() {
    //TODO
    return NULL;
}
