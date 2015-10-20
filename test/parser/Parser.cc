#include <boost/test/unit_test.hpp>
#include <sstream>
#include "parser/Parser.hh"

BOOST_AUTO_TEST_CASE(parser_test0)
{
    Parser p;
    std::stringstream ss{"(λ n. λ f. λ x. f (n f x))(λ f. λ x. f x)"};
    std::stringstream out;
    p.setContent(ss);
    p.dumpAST(*p.result(), out);
    BOOST_CHECK(out.str() == "(λ f. (λ x. (f (f x))))");
}

BOOST_AUTO_TEST_CASE(parser_test1)
{
    Parser p;
    std::stringstream ss{"(λ m. λ n. λ f. m (n f))(λ f. λ x. f (f x))(λ f. λ x. f (f (f x)))"};
    std::stringstream out;
    p.setContent(ss);
    p.dumpAST(*p.result(), out);
    BOOST_CHECK(out.str() == "(λ f. (λ x. (f (f (f (f (f (f x))))))))");
}
