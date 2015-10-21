#include "syntax/Term.hh"

std::unique_ptr<Term> Lambda::alphaConversion(std::set<std::string> conflicting) const
{
    if(conflicting.find(param_name_) != conflicting.end())
    {
        auto bounds = boundVars();
        conflicting.insert(bounds.begin(), bounds.end());
        std::string new_param_name_ = prime(param_name_, conflicting);
        return std::make_unique<Lambda>(new_param_name_, body_->substitute(param_name_, Variable{new_param_name_}));
    }
    else
    {
        return std::make_unique<Lambda>(param_name_, body_->alphaConversion(conflicting));
    }
}

std::unique_ptr<Term> Lambda::etaConversion() const
{
    Term& body = *body_;
    if(typeid(body) == typeid(Application))
    {
        Application& m = dynamic_cast<Application&>(*body_);
        const Term& s = m.getSecondRef();
        if(typeid(s) == typeid(Variable) &&
            dynamic_cast<const Variable&>(m.getSecondRef()).getName() == param_name_)
        {
            if(m.getFirstRef().freeVars().find(param_name_) == m.getFirstRef().freeVars().end())
                return m.getFirstRef().clone(); 
        }
    }
    return clone();
}


std::unique_ptr<Term> Lambda::substitute(const std::string& name, const Term& expr) const
{
    if(name == param_name_) return clone();
    std::set<std::string> frees = expr.freeVars();
    if(frees.find(param_name_) != frees.end())
    {
        return alphaConversion(frees)->substitute(name, expr);
    }
    else
    {
        return std::make_unique<Lambda>(param_name_, body_->substitute(name, expr));
    }
}

std::unique_ptr<Term> Application::betaReduction() const
{
    Term& first = *first_;
    if(typeid(first) == typeid(Lambda))
    {
        Lambda& l = dynamic_cast<Lambda&>(*first_);
        return l.getBodyRef().substitute(l.getParamName(), *second_);
    }
    else
    {
        std::unique_ptr<Term> left = std::make_unique<Application>(
                first_->clone()->betaReduction(), second_->clone());
        if(*left != *this) return left;
        return std::make_unique<Application>(
                first_->clone(), second_->clone()->betaReduction());
    }
}
