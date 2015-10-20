#pragma once

#include "AST.hh"
#include "lexical/Lexical.hh"

class Syntax {
private:
    std::unique_ptr<Lexical> lexer_;

    std::unique_ptr<ExprAST> parseIdentifierExpr()
    {
        if(lexer_->getCurToken() != Token::ID)
            throw std::logic_error{"expect identifier in identifer-expr"};
        auto ptr = std::make_unique<IdentifierExprAST>(lexer_->getIdentifier());
        lexer_->getNextToken();      /**< eat id */
        return std::move(ptr);
    }

    std::unique_ptr<ExprAST> parseExpr()
    {
        std::unique_ptr<ExprAST> first_ptr = nullptr;
        switch(lexer_->getCurToken()) 
        {
            case Token::ID: first_ptr = parseIdentifierExpr(); break;
            case Token::LAMBDA: first_ptr = parseLambdaExpr(); break;
            case Token::LBRACE: first_ptr = parseParenExpr(); break;
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
                    first_ptr = std::make_unique<MultiExprAST>(std::move(first_ptr), parseIdentifierExpr());
                    break;
                case Token::LAMBDA:
                    first_ptr = std::make_unique<MultiExprAST>(std::move(first_ptr), parseLambdaExpr());
                    break;
                case Token::LBRACE:
                    first_ptr = std::make_unique<MultiExprAST>(std::move(first_ptr), parseParenExpr());
                    break;
                default:
                    throw std::logic_error{"expect identifier, lambda, or '(' in multi-expr"};
            }
        }
        return first_ptr;
    }

    std::unique_ptr<ExprAST> parseLambdaExpr()
    {
        if(lexer_->getCurToken() != Token::LAMBDA)
            throw std::logic_error{"expect lambda at begin of lambda-expr"};
        if(lexer_->getNextToken() != Token::ID)
            throw std::logic_error{"expect identifer in lambda-expr"};
        std::string param_name = lexer_->getIdentifier();
        if(lexer_->getNextToken() != Token::BIND)
            throw std::logic_error{"expect '.' in lambda-expr"};
        lexer_->getNextToken();      /**< eat '.' */
        return std::make_unique<LambdaExprAST>(param_name, parseExpr()); 
    }

    std::unique_ptr<ExprAST> parseParenExpr()
    {
        if(lexer_->getCurToken() != Token::LBRACE)         
            throw std::logic_error{"expect '(' at begin of paren-expr"};
        if(lexer_->getNextToken() == Token::RBRACE)
            throw std::logic_error{"expect something between '(' and ')'"};
        std::unique_ptr<ExprAST> ptr = parseExpr();
        if(lexer_->getCurToken() != Token::RBRACE) 
            throw std::logic_error{"expect ')' at end of paren-expr"};
        lexer_->getNextToken();      /**< eat ')' */
        return std::move(ptr);
    }
public:
    Syntax(std::unique_ptr<Lexical> lexer = nullptr) : lexer_{std::move(lexer)} {}
    void setLexer(std::unique_ptr<Lexical> lexer)
    {
        lexer_ = std::move(lexer);
    }

    std::unique_ptr<ExprAST> getASTPtr()
    {
        if(lexer_ == nullptr)
            throw std::logic_error{"lexer not set"};
        lexer_->getNextToken();
        std::unique_ptr<ExprAST> ptr = parseExpr();
        switch(lexer_->getCurToken())
        {
            case Token::END: break;
            case Token::RBRACE: throw std::logic_error{"unexpect ')'"};
            default: throw std::logic_error{"unexpect token at end of input stream"};
        }
        return std::move(ptr);
    }
};
