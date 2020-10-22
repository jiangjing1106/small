#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <map>
#include <string>
#include "AST.h"

class SymbolTable {
public:
    SymbolTable(std::string type);
    void add(std::string item, ASTree* ast);
    void remove(std::string item);
    void setParent(SymbolTable* parent);
    std::string getType() {return m_type;}

protected:
    std::map<std::string, ASTree*> m_symbol;
    SymbolTable* m_parent;
    std::string m_type;
};

#endif // _SYMBOLTABLE_H_
