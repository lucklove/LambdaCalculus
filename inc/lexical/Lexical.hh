#pragma once

#include <string>
#include "Token.hh"

class Lexical 
{
private:
    std::string token_buf_;
    std::istream& input_stream_;
    Token cur_token_;

    Token token_select(int token)
    {
        if(token_buf_.empty()) 
        {
            return cur_token_ = token;
        } 
        else 
        {
            input_stream_.unget();
            if(token_buf_ == "\\" || token_buf_ == "λ") 
            {
                return cur_token_ = Token::LAMBDA;
            }
            else
            {
                return cur_token_ = Token::ID;
            }
        }
    }
    
public:
    Lexical(std::istream& is) : input_stream_{is} {}

    /**
     * \brief discard current token and update it to next token in stream
     * \return current token(after update).
     */ 
    Token getNextToken()
    {
        token_buf_.clear();
        while(true) 
        {
            int c = 0;
            switch(c = input_stream_.get()) 
            {
                case EOF: return token_select(Token::END);
                case '(': return token_select(Token::LBRACE);
                case ')': return token_select(Token::RBRACE);
                case '.': return token_select(Token::BIND);
                case ' ': case '\t': case '\n': case '\r':
                    if(token_buf_.empty())
                    {
                        continue;
                    }
                    else
                    {
                        return token_select(Token::ID);
                    }
                default:
                    token_buf_.push_back(c);
            }
        }
    }

    /**
     * \brief get current token.
     * \return current token.
     */ 
    Token getCurToken() 
    {
        return cur_token_;
    }

    std::string getIdentifier() 
    {
        return token_buf_;
    }
};
