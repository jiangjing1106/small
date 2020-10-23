#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <map>
#include <string>
#include "AST.h"

class SymbolTable {
public:
    SymbolTable(std::string type);
    void put(std::string item, ASTree* ast);
    ASTree* get(std::string item);
    void setParent(SymbolTable* parent);
    std::string getType() {return m_type;}

protected:7
    std::map<std::string, ASTree*> m_symbol;
    SymbolTable* m_parent;
    std::string m_type;
};

#endif // _SYMBOLTABLE_H_
