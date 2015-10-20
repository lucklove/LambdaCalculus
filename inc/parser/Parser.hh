#pragma once
#include "syntax/Syntax.hh"

class Parser {
private:
    Syntax syn_;

    void replace_param(std::string name, std::unique_ptr<ExprAST>& body, const ExprAST& expr)
    {
        if(typeid(*body) == typeid(IdentifierExprAST)) 
        {
            if(dynamic_cast<IdentifierExprAST&>(*body).getName() == name)
                body = expr.copy();
        }
        else if(typeid(*body) == typeid(LambdaExprAST))
        {
            LambdaExprAST& l = dynamic_cast<LambdaExprAST&>(*body);
            if(l.getParamName() != name)
                replace_param(name, l.getBodyPtrRef(), expr);
        }
        else
        {
            MultiExprAST& m = dynamic_cast<MultiExprAST&>(*body);
            replace_param(name, m.getFirstPtrRef(), expr);
            replace_param(name, m.getSecondPtrRef(), expr);
        }
    }

    std::unique_ptr<ExprAST> calculate(std::unique_ptr<ExprAST> ptr)
    {
        if(typeid(*ptr) == typeid(IdentifierExprAST))
            return ptr;
        if(typeid(*ptr) == typeid(LambdaExprAST))
        {
            LambdaExprAST& l = dynamic_cast<LambdaExprAST&>(*ptr);
            l.getBodyPtrRef() = calculate(l.getBodyPtr());
            return ptr;
        }
        MultiExprAST& expr = dynamic_cast<MultiExprAST&>(*ptr);
        std::unique_ptr<ExprAST> first = calculate(expr.getFirstPtr());
        if(typeid(*first) == typeid(LambdaExprAST))
        {
            LambdaExprAST& l = dynamic_cast<LambdaExprAST&>(*first);
            replace_param(l.getParamName(), l.getBodyPtrRef(), expr.getSecondRef());
            return l.getBodyPtr();
        }
        expr.getFirstPtrRef() = std::move(first);
        expr.getSecondPtrRef() = calculate(expr.getSecondPtr());
        return ptr;
    }

public:
    void setContent(std::istream& is)
    {
        syn_.setLexer(std::make_unique<Lexical>(is));
    }

    std::unique_ptr<ExprAST> result()
    {
        std::unique_ptr<ExprAST> old_ptr = calculate(syn_.getASTPtr());
        std::unique_ptr<ExprAST> new_ptr = old_ptr->copy();
        new_ptr = calculate(std::move(new_ptr));
        int exhausted_loop = 100;
        while(!(*new_ptr == *old_ptr))
        {
            if(--exhausted_loop < 0)
            {
                throw std::logic_error{"Exhausted Recursive: expression can not be evaluated"};
            }
            old_ptr = new_ptr->copy();
            new_ptr = calculate(std::move(new_ptr));
        }
        return new_ptr;;
    }
 
    static void dumpAST(const ExprAST& tree, std::ostream& os) 
    {
        if(typeid(tree) == typeid(IdentifierExprAST))
        {
            os << dynamic_cast<const IdentifierExprAST&>(tree).getName();
        }
        else if(typeid(tree) == typeid(LambdaExprAST))
        {
            os << "(";
            os << "λ " << dynamic_cast<const LambdaExprAST&>(tree).getParamName() << "." << " ";
            dumpAST(dynamic_cast<const LambdaExprAST&>(tree).getBodyRef(), os);
            os << ")";
        }
        else if(typeid(tree) == typeid(MultiExprAST))
        {
            os << "(";
            dumpAST(dynamic_cast<const MultiExprAST&>(tree).getFirstRef(), os);
            os << " ";
            dumpAST(dynamic_cast<const MultiExprAST&>(tree).getSecondRef(), os);
            os << ")";
        }   
    }
};
