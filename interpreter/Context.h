#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "../parser/SymbolTable.h"

class Context {
public:
    Context(SymbolTable* var, SymbolTable* func);
    void put(std::string table, std::string item, ASTree* ast);
    ASTree* get(std::string table, std::string item);
    void setParentContext(Context* cxt);
    Context* getParentContext();
    ASTree* getNext();

private:
    SymbolTable* m_var;
    SymbolTable* m_func;
    Context* m_parent;
};

#endif // _CONTEXT_H_
