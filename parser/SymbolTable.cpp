#include "SymbolTable.h"

SymbolTable::SymbolTable(std::string type): m_type(type) {
}

void SymbolTable::put(std::string item, ASTree* ast) {
    for (std::map<std::string, ASTree*>::iterator it=m_symbol.begin(); it!=m_symbol.end(); ++it) {
        try {
            if (it->first.compare(item) == 0) {
                throw "is redefined";
            }
        } catch (const char* msg) {
            printf("%s %s %s\n", m_type.c_str(), it->first.c_str(), msg);
            exit(1);
        }
    }
    m_symbol.insert(std::pair<std::string, ASTree*>(item, ast));
}

ASTree* SymbolTable::get(std::string item) {
    try {
        for (std::map<std::string, ASTree*>::iterator it=m_symbol.begin(); it!=m_symbol.end(); ++it) {
            if (it->first.compare(item) == 0) {
                return it->second;
            }
        }
        for (std::map<std::string, ASTree*>::iterator it=m_parent.begin(); it!=m_parent.end(); ++it) {
            if (it->first.compare(item) == 0) {
                return it->second;
            }
        }
        throw "is undefined";
    } catch (const char* msg) {
        printf("%s %s %s\n", m_type.c_str(), it->first.c_str(), msg);
        exit(1);
    }
}

void SymbolTable::setParent(SymbolTable* parent) {
    m_parent = parent;
}
