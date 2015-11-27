#include "UnitTest.hh"
#include <sstream>
#include "syntax/Syntax.hh"
#include "parser/Parser.hh"

TEST_CASE(syntax_test0)
{
    std::stringstream ss{"x x x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    TEST_REQUIRE(typeid(ast) == typeid(Application));
    Application& m = dynamic_cast<Application&>(ast);
    const Term& f = m.getFirstRef();
    const Term& s = m.getSecondRef();
    TEST_REQUIRE(typeid(f) == typeid(Application));
    TEST_REQUIRE(typeid(s) == typeid(Variable));
}

TEST_CASE(syntax_test1)
{
    std::stringstream ss{"(x y) x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    TEST_REQUIRE(typeid(ast) == typeid(Application));
    Application& m = dynamic_cast<Application&>(ast);
    const Term& f = m.getFirstRef();
    const Term& s = m.getSecondRef();
    TEST_REQUIRE(typeid(f) == typeid(Application));
    TEST_REQUIRE(typeid(s) == typeid(Variable));
}

TEST_CASE(syntax_test3)
{
    std::stringstream ss{"lambda x.lambda y.x"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto p = syn.getPtr();
    auto& ast = *p;
    TEST_REQUIRE(typeid(ast) == typeid(Lambda));
    TEST_REQUIRE(dynamic_cast<Lambda&>(ast).getParamName() == "x");
}
