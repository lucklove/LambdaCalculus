#pragma once
#include "syntax/Syntax.hh"
#include <set>

class Parser {
private:
    Syntax syn_;
   
    std::unique_ptr<Term> evaluate(std::unique_ptr<Term> ptr)
    {
        std::unique_ptr<Term> beta = ptr->betaReduction();
        if(*beta != *ptr)
            return evaluate(std::move(beta));
        return ptr->etaConversion();
    }

public:
    void setContent(std::istream& is)
    {
        syn_.setLexer(std::make_unique<Lexical>(is));
    }

    std::unique_ptr<Term> result()
    {
        return evaluate(syn_.getPtr());
    }
 
    static void dump(const Term& tree, std::ostream& os) 
    {
        if(typeid(tree) == typeid(Variable))
        {
            os << dynamic_cast<const Variable&>(tree).getName();
        }
        else if(typeid(tree) == typeid(Lambda))
        {
            os << "(";
            os << "λ " << dynamic_cast<const Lambda&>(tree).getParamName() << "." << " ";
            dump(dynamic_cast<const Lambda&>(tree).getBodyRef(), os);
            os << ")";
        }
        else if(typeid(tree) == typeid(Application))
        {
            os << "(";
            dump(dynamic_cast<const Application&>(tree).getFirstRef(), os);
            os << " ";
            dump(dynamic_cast<const Application&>(tree).getSecondRef(), os);
            os << ")";
        }   
    }
};
