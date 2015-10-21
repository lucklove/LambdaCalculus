#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <cassert>
#include <set>

inline std::string prime(std::string name, const std::set<std::string>& conflicting)
{
    while(conflicting.find(name) != conflicting.end())
        name += "λ";
    return name;
}

struct Term 
{
    virtual ~Term() = default;
    virtual std::unique_ptr<Term> clone() const = 0;
    virtual bool operator==(const Term&) const = 0;
    virtual std::unique_ptr<Term> substitute(const std::string&, const Term&) const = 0;
    virtual std::unique_ptr<Term> alphaConversion(std::set<std::string> conflicting) const
    {
        return clone();
    }
    virtual std::unique_ptr<Term> betaReduction() const 
    {
        return clone();
    }

    virtual std::unique_ptr<Term> etaConversion() const
    {
        return clone();
    }
    virtual std::set<std::string> freeVars() const = 0;
    virtual std::set<std::string> boundVars() const = 0;
    bool operator!=(const Term& e) const 
    {
        return !operator==(e);
    }
};

class Variable : public Term 
{
private:
    std::string name_;
public:
    Variable(std::string name) : name_{name} {}

    std::unique_ptr<Term> clone() const override
    {
        return std::make_unique<Variable>(name_);
    }

    bool operator==(const Term& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const Variable&>(expr).name_ == name_;
    }

    std::unique_ptr<Term> substitute(const std::string& name, const Term& expr) const override
    {
        return name == name_ ? expr.clone() : clone();   
    }

    std::set<std::string> freeVars() const override
    {
        return std::set<std::string>{name_};
    }

    std::set<std::string> boundVars() const override
    {
        return std::set<std::string>{};
    }

    std::string getName() const
    {
        return name_;
    }
};

class Lambda : public Term
{
private:
    std::string param_name_;
    std::unique_ptr<Term> body_;
public:
    Lambda(std::string param, std::unique_ptr<Term> body)
        : param_name_{param}, body_{std::move(body)} {}

    std::unique_ptr<Term> clone() const override
    {
        return std::make_unique<Lambda>(param_name_, body_->clone());
    }

    bool operator==(const Term& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const Lambda&>(expr).param_name_ == param_name_ &&
            *dynamic_cast<const Lambda&>(expr).body_ == *body_;
    }

    std::set<std::string> freeVars() const override
    {
        std::set<std::string> frees = body_->freeVars();
        frees.erase(param_name_);
        return frees;
    }

    std::set<std::string> boundVars() const override
    {
        std::set<std::string> bounds = body_->boundVars();
        bounds.insert(param_name_);
        return bounds;
    }

    std::unique_ptr<Term> alphaConversion(std::set<std::string> conflicting) const override;

    std::unique_ptr<Term> substitute(const std::string&, const Term&) const override;

    virtual std::unique_ptr<Term> betaReduction() const override
    {
        return std::make_unique<Lambda>(param_name_, body_->betaReduction());
    }

    std::unique_ptr<Term> etaConversion() const override;

    std::string getParamName() const
    {
        return param_name_;
    }

    const Term& getBodyRef() const
    {
        return *body_;
    }

    std::unique_ptr<Term> getBodyPtr()
    {
        return std::move(body_);
    }
};

class Application : public Term
{
private:
    std::unique_ptr<Term> first_;
    std::unique_ptr<Term> second_;
public:
    Application(std::unique_ptr<Term> first, std::unique_ptr<Term> second)
        : first_{std::move(first)}, second_{std::move(second)} {}

    std::unique_ptr<Term> clone() const override
    {
        return std::make_unique<Application>(first_->clone(), second_->clone());
    }
    
    bool operator==(const Term& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return *dynamic_cast<const Application&>(expr).first_ == *first_ &&
            *dynamic_cast<const Application&>(expr).second_ == *second_;
    }

    std::unique_ptr<Term> substitute(const std::string& name, const Term& expr) const override
    {
        return std::make_unique<Application>(first_->substitute(name, expr), second_->substitute(name, expr));
    }

    std::unique_ptr<Term> alphaConversion(std::set<std::string> conflicting) const override
    {
        return std::make_unique<Application>(
                        first_->alphaConversion(conflicting),
                        second_->alphaConversion(conflicting));        
    }

    std::unique_ptr<Term> betaReduction() const override;

    std::set<std::string> freeVars() const override
    {
        auto ffrees = first_->freeVars();
        auto sfrees = second_->freeVars();
        ffrees.insert(sfrees.begin(), sfrees.end());
        return ffrees;
    }

    std::set<std::string> boundVars() const override
    {
        auto fbounds = first_->boundVars();
        auto sbounds = second_->boundVars();
        fbounds.insert(sbounds.begin(), sbounds.end());
        return fbounds;
    }

    const Term& getFirstRef() const
    {
        return *first_;
    }

    const Term& getSecondRef() const
    {
        return *second_;
    }
};
