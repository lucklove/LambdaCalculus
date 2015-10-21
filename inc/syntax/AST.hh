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

struct Expr 
{
    virtual ~Expr() = default;
    virtual std::unique_ptr<Expr> copy() const = 0;
    virtual bool operator==(const Expr&) const = 0;
    virtual std::unique_ptr<Expr> substitute(const std::string&, const Expr&) = 0;
    virtual std::unique_ptr<Expr> alphaConversion(std::set<std::string> conflicting)
    {
        return copy();
    }
    virtual std::unique_ptr<Expr> betaReduction()
    {
        return copy();
    }
    virtual std::unique_ptr<Expr> etaConversion()
    {
        return copy();
    }
    virtual std::set<std::string> freeVars() const = 0;
    virtual std::set<std::string> boundVars() const = 0;
    bool operator!=(const Expr& e) const 
    {
        return !operator==(e);
    }
};

class IdentifierExpr : public Expr 
{
private:
    std::string name_;
public:
    IdentifierExpr(std::string name) : name_{name} {}

    std::unique_ptr<Expr> copy() const override
    {
        return std::make_unique<IdentifierExpr>(name_);
    }

    bool operator==(const Expr& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const IdentifierExpr&>(expr).name_ == name_;
    }

    std::unique_ptr<Expr> substitute(const std::string& name, const Expr& expr) override
    {
        return name == name_ ? expr.copy() : copy();   
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

class LambdaExpr : public Expr
{
private:
    std::string param_name_;
    std::unique_ptr<Expr> body_;
public:
    LambdaExpr(std::string param, std::unique_ptr<Expr> body)
        : param_name_{param}, body_{std::move(body)} {}

    std::unique_ptr<Expr> copy() const override
    {
        return std::make_unique<LambdaExpr>(param_name_, body_->copy());
    }

    bool operator==(const Expr& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const LambdaExpr&>(expr).param_name_ == param_name_ &&
            *dynamic_cast<const LambdaExpr&>(expr).body_ == *body_;
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

    std::unique_ptr<Expr> substitute(const std::string& name, const Expr& expr) override
    {
        if(name == param_name_) return copy();
        std::set<std::string> frees = expr.freeVars();
        if(frees.find(param_name_) != frees.end())
        {
            return alphaConversion(frees)->substitute(name, expr);
        }
        else
        {
            return std::make_unique<LambdaExpr>(param_name_, body_->substitute(name, expr));
        }
    }

    std::unique_ptr<Expr> alphaConversion(std::set<std::string> conflicting) override
    {
        if(conflicting.find(param_name_) != conflicting.end())
        {
            auto bounds = boundVars();
            conflicting.insert(bounds.begin(), bounds.end());
            std::string new_param_name_ = prime(param_name_, conflicting);
            return std::make_unique<LambdaExpr>(new_param_name_, body_->substitute(param_name_, IdentifierExpr{new_param_name_}));
        }
        else
        {
            return std::make_unique<LambdaExpr>(param_name_, body_->alphaConversion(conflicting));
        }
    }

    virtual std::unique_ptr<Expr> betaReduction() override
    {
        return std::make_unique<LambdaExpr>(param_name_, body_->betaReduction());
    }

    std::unique_ptr<Expr> etaConversion() override;

    std::string getParamName() const
    {
        return param_name_;
    }

    void setParamName(const std::string& new_name)
    {
        param_name_ = new_name;
    }

    Expr& getBodyRef()
    {
        return *body_;
    }

    const Expr& getBodyRef() const
    {
        return *body_;
    }

    std::unique_ptr<Expr> getBodyPtr()
    {
        return std::move(body_);
    }

    std::unique_ptr<Expr>& getBodyPtrRef()
    {
        return body_;
    }
};

class MultiExpr : public Expr
{
private:
    std::unique_ptr<Expr> first_;
    std::unique_ptr<Expr> second_;
public:
    MultiExpr(std::unique_ptr<Expr> first, std::unique_ptr<Expr> second)
        : first_{std::move(first)}, second_{std::move(second)} {}

    std::unique_ptr<Expr> copy() const override
    {
        return std::make_unique<MultiExpr>(first_->copy(), second_->copy());
    }
    
    bool operator==(const Expr& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return *dynamic_cast<const MultiExpr&>(expr).first_ == *first_ &&
            *dynamic_cast<const MultiExpr&>(expr).second_ == *second_;
    }

    std::unique_ptr<Expr> substitute(const std::string& name, const Expr& expr) override
    {
        first_ = first_->substitute(name, expr);
        second_ = second_->substitute(name, expr);
        return copy();
    }

    std::unique_ptr<Expr> alphaConversion(std::set<std::string> conflicting) override
    {
        return std::make_unique<MultiExpr>(
                        first_->alphaConversion(conflicting),
                        second_->alphaConversion(conflicting));        
    }

    std::unique_ptr<Expr> betaReduction() override
    {
        Expr& first = *first_;
        if(typeid(first) == typeid(LambdaExpr))
        {
            LambdaExpr& l = dynamic_cast<LambdaExpr&>(*first_);
            return l.getBodyRef().substitute(l.getParamName(), *second_);
        }
        else
        {
            std::unique_ptr<Expr> left = std::make_unique<MultiExpr>(
                    first_->copy()->betaReduction(), second_->copy());
            if(*left != *this) return left;
            return std::make_unique<MultiExpr>(
                    first_->copy(), second_->copy()->betaReduction());
        }
    }

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

    const Expr& getFirstRef() const
    {
        return *first_;
    }

    Expr& getFirstRef()
    {
        return *first_;
    }

    std::unique_ptr<Expr> getFirstPtr()
    {
        return std::move(first_);
    }

    std::unique_ptr<Expr>& getFirstPtrRef()
    {
        return first_;
    }

    const Expr& getSecondRef() const
    {
        return *second_;
    }

    Expr& getSecondRef()
    {
        return *second_;
    }

    std::unique_ptr<Expr> getSecondPtr()
    {
        return std::move(second_);
    }

    std::unique_ptr<Expr>& getSecondPtrRef()
    {
        return second_;
    }
};
