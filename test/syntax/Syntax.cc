#include <boost/test/unit_test.hpp>
#include <sstream>
#include "syntax/Syntax.hh"

BOOST_AUTO_TEST_CASE(syntax_test0)
{
    std::stringstream ss{"x x x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto ast = syn.getASTPtr();
    BOOST_REQUIRE(typeid(*ast) == typeid(MultiExprAST));
    BOOST_REQUIRE(typeid(dynamic_cast<MultiExprAST&>(*ast).getFirstRef()) == typeid(MultiExprAST));
    BOOST_REQUIRE(typeid(dynamic_cast<MultiExprAST&>(*ast).getSecondRef()) == typeid(IdentifierExprAST));
}

BOOST_AUTO_TEST_CASE(syntax_test1)
{
    std::stringstream ss{"(x y) x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto ast = syn.getASTPtr();
    BOOST_REQUIRE(typeid(*ast) == typeid(MultiExprAST));
    BOOST_REQUIRE(typeid(dynamic_cast<MultiExprAST&>(*ast).getFirstRef()) == typeid(MultiExprAST));
    BOOST_REQUIRE(typeid(dynamic_cast<MultiExprAST&>(*ast).getSecondRef()) == typeid(IdentifierExprAST));
}

BOOST_AUTO_TEST_CASE(syntax_test3)
{
    std::stringstream ss{"lambda x.lambda y.x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto ast = syn.getASTPtr();
    BOOST_REQUIRE(typeid(*ast) == typeid(LambdaExprAST));
    BOOST_REQUIRE(dynamic_cast<LambdaExprAST&>(*ast).getParamName() == "x");
}
