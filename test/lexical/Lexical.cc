#include <boost/test/unit_test.hpp>
#include <sstream>
#include "lexical/Lexical.hh"

BOOST_AUTO_TEST_CASE(lexial_test)
{
    std::stringstream ss{"λ x.\n\r\t x"};
    Lexical lex{ss};
    BOOST_CHECK(lex.getNextToken() == Token::LAMBDA);
    BOOST_CHECK(lex.getCurToken() == Token::LAMBDA);
    BOOST_CHECK(lex.getNextToken() == Token::ID);
    BOOST_CHECK(lex.getIdentifier() == "x");
    BOOST_CHECK(lex.getNextToken() == Token::BIND);
    BOOST_CHECK(lex.getNextToken() == Token::ID);
    BOOST_CHECK(lex.getIdentifier() == "x");
}
