#pragma once

class Token 
{
private:
    int kind_;

public:
    enum
    {
        ID, LAMBDA, LBRACE, RBRACE, BIND, END
    };

    Token() = default;

    Token(int kind) : kind_{kind} {}

    bool operator==(const Token& t) const
    {
        return kind_ == t.kind_;
    }

    bool operator==(int kind) const
    {
        return kind_ == kind;
    }

    operator int()
    {
        return kind_;
    }
};
