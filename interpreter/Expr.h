#ifndef _EXPR_H_
#define _EXPR_H_

#include "../parser/AST.h"
#include "Context.h"

class ASTreeEx : public ASTree {
public:
    virtual int numChildren() = 0;
    virtual ASTree* child(int i) = 0;
    virtual std::string getObjectName() = 0;
    virtual std::string toString() = 0;
    virtual int location() = 0;
    virtual int eval(Context cxt) ;
};

class AstLeafEx : public AstLeaf {
public:
    virtual int eval(Context cxt) ;
};

class AstListEx : public AstList {
public:
    virtual int eval(Context cxt) ;
};

class IntNumberNodeEx : public IntNumberNode {
public:
    virtual int eval(Context cxt) ;
};

class FloatNumberNodeEx : public FloatNumberNode {
public:
    virtual int eval(Context cxt) ;
};

class IdentifyNodeEx : public IdentifyNode {
public:
    virtual int eval(Context cxt) ;
};

class UnitaryNodeEx : public UnitaryNode {
public:
    virtual int eval(Context cxt) ;
};

class BinaryNodeEx : public BinaryNode {
public:
    virtual int eval(Context cxt) ;
};

class BlockNodeEx : public BlockNode {
public:
    virtual int eval(Context cxt) ;
};

class IfNodeEx : public IfNode {
public:
    virtual int eval(Context cxt) ;
};

class WhileNodeEx : public WhileNode {
public:
    virtual int eval(Context cxt) ;
};

class ParamsListNodeEx : public ParamsListNode {
public:
    virtual int eval(Context cxt) ;
};

class DefNodeEx : public DefNode {
public:
    virtual int eval(Context cxt) ;
};

class ArgsNodeEx : public ArgsNode {
public:
    virtual int eval(Context cxt) ;
};

class CallNodeEx : public CallNode {
public:
    virtual int eval(Context cxt) ;
};
#endif // _EXPR_H_
