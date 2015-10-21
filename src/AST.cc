#include "syntax/AST.hh"

std::unique_ptr<Expr> LambdaExpr::etaConversion()
{
    Expr& body = *body_;
    if(typeid(body) == typeid(MultiExpr))
    {
        MultiExpr& m = dynamic_cast<MultiExpr&>(*body_);
        Expr& s = m.getSecondRef();
        if(typeid(s) == typeid(IdentifierExpr) &&
            dynamic_cast<IdentifierExpr&>(m.getSecondRef()).getName() == param_name_)
        {
            if(m.getFirstRef().freeVars().find(param_name_) == m.getFirstRef().freeVars().end())
                return m.getFirstPtr(); 
        }
    }
    return copy();
}
