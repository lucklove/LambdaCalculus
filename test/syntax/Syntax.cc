#include <boost/test/unit_test.hpp>
#include <sstream>
#include "syntax/Syntax.hh"
#include "parser/Parser.hh"

BOOST_AUTO_TEST_CASE(syntax_test0)
{
    std::stringstream ss{"x x x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    BOOST_REQUIRE(typeid(ast) == typeid(MultiExpr));
    MultiExpr& m = dynamic_cast<MultiExpr&>(ast);
    Expr& f = m.getFirstRef();
    Expr& s = m.getSecondRef();
    BOOST_REQUIRE(typeid(f) == typeid(MultiExpr));
    BOOST_REQUIRE(typeid(s) == typeid(IdentifierExpr));
}

BOOST_AUTO_TEST_CASE(syntax_test1)
{
    std::stringstream ss{"(x y) x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    BOOST_REQUIRE(typeid(ast) == typeid(MultiExpr));
    MultiExpr& m = dynamic_cast<MultiExpr&>(ast);
    Expr& f = m.getFirstRef();
    Expr& s = m.getSecondRef();
    BOOST_REQUIRE(typeid(f) == typeid(MultiExpr));
    BOOST_REQUIRE(typeid(s) == typeid(IdentifierExpr));
}

BOOST_AUTO_TEST_CASE(syntax_test3)
{
    std::stringstream ss{"lambda x.lambda y.x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    BOOST_REQUIRE(typeid(ast) == typeid(LambdaExpr));
    BOOST_REQUIRE(dynamic_cast<LambdaExpr&>(ast).getParamName() == "x");
}
