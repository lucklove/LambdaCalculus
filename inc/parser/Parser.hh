#pragma once
#include "syntax/Syntax.hh"
#include <set>

class Parser {
private:
    Syntax syn_;
   
    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> ptr)
    {
        std::unique_ptr<Expr> beta = ptr->betaReduction();
        if(*beta != *ptr)
            return evaluate(std::move(beta));
        return ptr->etaConversion();
    }

public:
    void setContent(std::istream& is)
    {
        syn_.setLexer(std::make_unique<Lexical>(is));
    }

    std::unique_ptr<Expr> result()
    {
        return evaluate(syn_.getPtr());
    }
 
    static void dump(const Expr& tree, std::ostream& os) 
    {
        if(typeid(tree) == typeid(IdentifierExpr))
        {
            os << dynamic_cast<const IdentifierExpr&>(tree).getName();
        }
        else if(typeid(tree) == typeid(LambdaExpr))
        {
            os << "(";
            os << "λ " << dynamic_cast<const LambdaExpr&>(tree).getParamName() << "." << " ";
            dump(dynamic_cast<const LambdaExpr&>(tree).getBodyRef(), os);
            os << ")";
        }
        else if(typeid(tree) == typeid(MultiExpr))
        {
            os << "(";
            dump(dynamic_cast<const MultiExpr&>(tree).getFirstRef(), os);
            os << " ";
            dump(dynamic_cast<const MultiExpr&>(tree).getSecondRef(), os);
            os << ")";
        }   
    }
};
