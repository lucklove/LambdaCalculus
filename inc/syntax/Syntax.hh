#pragma once

#include "Term.hh"
#include "lexical/Lexical.hh"

class Syntax 
{
private:
    std::unique_ptr<Lexical> lexer_;

    std::unique_ptr<Term> parseVariable()
    {
        if(lexer_->getCurToken() != Token::ID)
            throw std::logic_error{"expect identifier in identifer-expr"};
        auto ptr = std::make_unique<Variable>(lexer_->getIdentifier());
        lexer_->getNextToken();      /**< eat id */
        return std::move(ptr);
    }

    std::unique_ptr<Term> parseTerm()
    {
        std::unique_ptr<Term> first_ptr = nullptr;
        switch(lexer_->getCurToken()) 
        {
            case Token::ID: first_ptr = parseVariable(); break;
            case Token::LAMBDA: first_ptr = parseLambda(); break;
            case Token::LBRACE: first_ptr = parseParenTerm(); break;
            case Token::END:
                throw std::logic_error{"unexpected end"};
            default:
                throw std::logic_error{"expect identifier, lambda, or '(' in at begin of expr"}; 
        } 

        while(lexer_->getCurToken() != Token::RBRACE && lexer_->getCurToken() != Token::END)
        {
            switch(lexer_->getCurToken()) 
            {
                case Token::ID:
                    first_ptr = std::make_unique<Application>(std::move(first_ptr), parseVariable());
                    break;
                case Token::LAMBDA:
                    first_ptr = std::make_unique<Application>(std::move(first_ptr), parseLambda());
                    break;
                case Token::LBRACE:
                    first_ptr = std::make_unique<Application>(std::move(first_ptr), parseParenTerm());
                    break;
                default:
                    throw std::logic_error{"expect identifier, lambda, or '(' in multi-expr"};
            }
        }
        return first_ptr;
    }

    std::unique_ptr<Term> parseLambda()
    {
        if(lexer_->getCurToken() != Token::LAMBDA)
            throw std::logic_error{"expect lambda at begin of lambda-expr"};
        if(lexer_->getNextToken() != Token::ID)
            throw std::logic_error{"expect identifer in lambda-expr"};
        std::string param_name = lexer_->getIdentifier();
        if(lexer_->getNextToken() != Token::BIND)
            throw std::logic_error{"expect '.' in lambda-expr"};
        lexer_->getNextToken();      /**< eat '.' */
        return std::make_unique<Lambda>(param_name, parseTerm()); 
    }

    std::unique_ptr<Term> parseParenTerm()
    {
        if(lexer_->getCurToken() != Token::LBRACE)         
            throw std::logic_error{"expect '(' at begin of paren-expr"};
        if(lexer_->getNextToken() == Token::RBRACE)
            throw std::logic_error{"expect something between '(' and ')'"};
        std::unique_ptr<Term> ptr = parseTerm();
        if(lexer_->getCurToken() != Token::RBRACE) 
            throw std::logic_error{"expect ')' at end of paren-expr"};
        lexer_->getNextToken();      /**< eat ')' */
        return ptr;
    }
public:
    Syntax(std::unique_ptr<Lexical> lexer = nullptr) : lexer_{std::move(lexer)} {}
    void setLexer(std::unique_ptr<Lexical> lexer)
    {
        lexer_ = std::move(lexer);
    }

    std::unique_ptr<Term> getPtr()
    {
        if(lexer_ == nullptr)
            throw std::logic_error{"lexer not set"};
        lexer_->getNextToken();
        std::unique_ptr<Term> ptr = parseTerm();
        switch(lexer_->getCurToken())
        {
            case Token::END: break;
            case Token::RBRACE: throw std::logic_error{"unexpect ')'"};
            default: throw std::logic_error{"unexpect token at end of input stream"};
        }
        return ptr;
    }
};
