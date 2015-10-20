#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <cassert>

struct ExprAST 
{
    virtual ~ExprAST() = default;
    virtual std::unique_ptr<ExprAST> copy() const = 0;
    virtual bool operator==(const ExprAST&) const = 0;
};

class IdentifierExprAST : public ExprAST 
{
private:
    std::string name_;
public:
    IdentifierExprAST(std::string name) : name_{name} {}

    std::unique_ptr<ExprAST> copy() const override
    {
        return std::make_unique<IdentifierExprAST>(name_);
    }

    bool operator==(const ExprAST& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const IdentifierExprAST&>(expr).name_ == name_;
    }

    std::string getName() const
    {
        return name_;
    }
};

class LambdaExprAST : public ExprAST
{
private:
    std::string param_name_;
    std::unique_ptr<ExprAST> body_;
public:
    LambdaExprAST(std::string param, std::unique_ptr<ExprAST> body)
        : param_name_{param}, body_{std::move(body)} {}

    std::unique_ptr<ExprAST> copy() const override
    {
        return std::make_unique<LambdaExprAST>(param_name_, body_->copy());
    }

    bool operator==(const ExprAST& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return dynamic_cast<const LambdaExprAST&>(expr).param_name_ == param_name_ &&
            *dynamic_cast<const LambdaExprAST&>(expr).body_ == *body_;
    }

    std::string getParamName() const
    {
        return param_name_;
    }

    ExprAST& getBodyRef()
    {
        return *body_;
    }

    const ExprAST& getBodyRef() const
    {
        return *body_;
    }

    std::unique_ptr<ExprAST> getBodyPtr()
    {
        return std::move(body_);
    }

    std::unique_ptr<ExprAST>& getBodyPtrRef()
    {
        return body_;
    }
};

class MultiExprAST : public ExprAST
{
private:
    std::unique_ptr<ExprAST> first_;
    std::unique_ptr<ExprAST> second_;
public:
    MultiExprAST(std::unique_ptr<ExprAST> first, std::unique_ptr<ExprAST> second)
        : first_{std::move(first)}, second_{std::move(second)} {}

    std::unique_ptr<ExprAST> copy() const override
    {
        return std::make_unique<MultiExprAST>(first_->copy(), second_->copy());
    }
    
    bool operator==(const ExprAST& expr) const override
    {
        if(typeid(expr) != typeid(*this)) return false;
        return *dynamic_cast<const MultiExprAST&>(expr).first_ == *first_ &&
            *dynamic_cast<const MultiExprAST&>(expr).second_ == *second_;
    }

    const ExprAST& getFirstRef() const
    {
        return *first_;
    }

    ExprAST& getFirstRef()
    {
        return *first_;
    }

    std::unique_ptr<ExprAST> getFirstPtr()
    {
        return std::move(first_);
    }

    std::unique_ptr<ExprAST>& getFirstPtrRef()
    {
        return first_;
    }

    const ExprAST& getSecondRef() const
    {
        return *second_;
    }

    ExprAST& getSecondRef()
    {
        return *second_;
    }

    std::unique_ptr<ExprAST> getSecondPtr()
    {
        return std::move(second_);
    }

    std::unique_ptr<ExprAST>& getSecondPtrRef()
    {
        return second_;
    }
};

