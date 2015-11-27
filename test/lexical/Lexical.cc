#include "UnitTest.hh"
#include <sstream>
#include "lexical/Lexical.hh"

TEST_CASE(lexial_test)
{
    std::stringstream ss{"λ x.\n\r\t x"};
    Lexical lex{ss};
    TEST_CHECK(lex.getNextToken() == Token::LAMBDA);
    TEST_CHECK(lex.getCurToken() == Token::LAMBDA);
    TEST_CHECK(lex.getNextToken() == Token::ID);
    TEST_CHECK(lex.getIdentifier() == "x");
    TEST_CHECK(lex.getNextToken() == Token::BIND);
    TEST_CHECK(lex.getNextToken() == Token::ID);
    TEST_CHECK(lex.getIdentifier() == "x");
}
