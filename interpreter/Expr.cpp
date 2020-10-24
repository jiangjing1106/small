#include <stdio.h>
#include "Expr.h"

//==========ASTreeEx============
int ASTreeEx::eval(Context cxt) {
    return 0;
}

//==========AstLeafEx============
int AstLeafEx::eval(Context cxt) {
    return 0;
}

//==========AstListEx============
int AstListEx::eval(Context cxt) {
    return 0;
}

//==========IntNumberNodeEx============
int IntNumberNodeEx::eval(Context cxt) {
    return value();
}

//==========FloatNumberNodeEx============
int FloatNumberNodeEx::eval(Context cxt) {
    return (int)value();
}

//==========IdentifyNodeEx============
int IdentifyNodeEx::eval(Context cxt) {
    IdentifyNode* var = (IdentifyNode*)(cxt.get("var", value()));
    try {
        if (var == NULL) throw "is undefined";
        return ((IdentifyNodeEx*)var)->eval(cxt);
    } catch (const char* msg) {
        printf("%s %s\n", var->value().c_str(), msg);
        exit(1);
    }
}

//==========UnitaryNodeEx============
int UnitaryNodeEx::eval(Context cxt) {
    if (operatorType().type == TT_PLUS) {
        return ((UnitaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_MINUS) {
        return 0 - ((UnitaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_NOT) {
        return !(((UnitaryNodeEx*)rightNode())->eval(cxt));
    } else {
        printf("error %s, %d\n", __func__, __LINE__);
    }
    return 0;
}

//==========BinaryNodeEx============
int BinaryNodeEx::eval(Context cxt) {
    if (operatorType().type == TT_PLUS) {
        return ((BinaryNodeEx*)leftNode())->eval(cxt) + ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_MINUS) {
        return ((BinaryNodeEx*)leftNode())->eval(cxt) - ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_MUL) {
        return ((BinaryNodeEx*)leftNode())->eval(cxt) * ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_DIV) {
        try {
            if (((BinaryNodeEx*)rightNode())->eval(cxt) == 0) throw "Divisor cannot be 0.";
        } catch (const char* msg) {
            printf("%d: %s\n", operatorType().lineno, msg);
            exit(1);
        }
        return ((BinaryNodeEx*)leftNode())->eval(cxt) / ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_ASSIGN) {
        if (!rightNode()->getObjectName().compare("CallNode")) {
            //Context new_context = *(cxt.getParentContext()); 
            //TODO 函数调用结束，需要还原到parent context
            //TODO: 取出parent context需要执行的下一个指令
        } else {
            cxt.put("var", leftNode()->toString(), rightNode());
        }
        return 0;
    } else if (operatorType().type == TT_EQ) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) == ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_NEQ) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) != ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_GT) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) > ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_LT) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) < ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_GTOREQ) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) >= ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_LTOREQ) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) <= ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_LAND) {
        return ((BinaryNodeEx*)leftNode())->eval(cxt) & ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_LOR) {
        return ((BinaryNodeEx*)leftNode())->eval(cxt) | ((BinaryNodeEx*)rightNode())->eval(cxt);
    } else if (operatorType().type == TT_AND) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) && ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else if (operatorType().type == TT_OR) {
        return (((BinaryNodeEx*)leftNode())->eval(cxt) || ((BinaryNodeEx*)rightNode())->eval(cxt))? 1 : 0;
    } else {
        printf("error %s, %d\n", __func__, __LINE__);
        return 0;
    }
}

//==========BlockNodeEx============
int BlockNodeEx::eval(Context cxt) {
    return 0;
}

//==========IfNodeEx============
int IfNodeEx::eval(Context cxt) {
    if (((IfNodeEx*)condition())->eval(cxt)) {
        return ((IfNodeEx*)body())->eval(cxt);
    } else {
        return ((IfNodeEx*)elsebody())->eval(cxt);
    }
}

//==========WhileNodeEx============
int WhileNodeEx::eval(Context cxt) {
    int res = 0;
    for(;;) {
        if (((WhileNodeEx*)condition())->eval(cxt)) {
            res = ((WhileNodeEx*)body())->eval(cxt);
        } else {
            break;
        }
    }
    return res;
}

//==========ParamsListNodeEx============
int ParamsListNodeEx::eval(Context cxt) {
    return 0;
}

//==========DefNodeEx============
int DefNodeEx::eval(Context cxt) {
    return 0;
}

//==========ArgsNodeEx============
int ArgsNodeEx::eval(Context cxt) {
    return 0;
}

//==========CallNodeEx============
int CallNodeEx::eval(Context cxt) {
    DefNode* defNode = (DefNode*)(cxt.get("func", name()));
    try {
        if (defNode == NULL) throw "is undefined";
        if (args()->numChildren() != defNode->paramlist()->numChildren()) {
            throw "args and params are not matched";
        }
        // 函数调用， new Context
        Context new_context = cxt;
        new_context.setParentContext(&cxt);
        for (int i=0; i<numChildren(); i++) {
            new_context.put("var", defNode->paramlist()->name(i), args()->child(i));
        }
        return ((DefNodeEx*)(defNode->body()))->eval(new_context);
    } catch (const char* msg) {
        printf("%s %s\n", name().c_str(), msg);
        exit(1);
    }
}

