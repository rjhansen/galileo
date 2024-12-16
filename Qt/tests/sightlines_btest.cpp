#define BOOST_TEST_MODULE ExampleTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ExampleSuite)

BOOST_AUTO_TEST_CASE( my_test )
{
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()