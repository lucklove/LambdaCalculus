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
    BOOST_REQUIRE(typeid(ast) == typeid(Application));
    Application& m = dynamic_cast<Application&>(ast);
    const Term& f = m.getFirstRef();
    const Term& s = m.getSecondRef();
    BOOST_REQUIRE(typeid(f) == typeid(Application));
    BOOST_REQUIRE(typeid(s) == typeid(Variable));
}

BOOST_AUTO_TEST_CASE(syntax_test1)
{
    std::stringstream ss{"(x y) x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    BOOST_REQUIRE(typeid(ast) == typeid(Application));
    Application& m = dynamic_cast<Application&>(ast);
    const Term& f = m.getFirstRef();
    const Term& s = m.getSecondRef();
    BOOST_REQUIRE(typeid(f) == typeid(Application));
    BOOST_REQUIRE(typeid(s) == typeid(Variable));
}

BOOST_AUTO_TEST_CASE(syntax_test3)
{
    std::stringstream ss{"lambda x.lambda y.x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    BOOST_REQUIRE(typeid(ast) == typeid(Lambda));
    BOOST_REQUIRE(dynamic_cast<Lambda&>(ast).getParamName() == "x");
}
