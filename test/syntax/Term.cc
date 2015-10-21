#include <boost/test/unit_test.hpp>
#include <sstream>
#include "syntax/Syntax.hh"

BOOST_AUTO_TEST_CASE(syntax_test4)
{
    std::stringstream ss{" λ f. ((λ x. f (x x)) (λ x. f (x x)))"};
    Syntax syn{std::make_unique<Lexical>(ss)};
    auto ast = syn.getPtr();
    auto copy = ast->clone();
    /** if some thing goes wrong, SIGSEGV will be emit */
}
