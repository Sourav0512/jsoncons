// Copyright 2013 Daniel Parker
// Distributed under Boost license

#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <new>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/json_query.hpp>

using namespace jsoncons;
using namespace jsoncons::jsonpath;

BOOST_AUTO_TEST_SUITE(jsonpath_filter_tests)

struct jsonpath_filter_fixture
{
    static const char* store_text()
    {
        static const char* text = "{ \"store\": {\"book\": [ { \"category\": \"reference\",\"author\": \"Nigel Rees\",\"title\": \"Sayings of the Century\",\"price\": 8.95},{ \"category\": \"fiction\",\"author\": \"Evelyn Waugh\",\"title\": \"Sword of Honour\",\"price\": 12.99},{ \"category\": \"fiction\",\"author\": \"Herman Melville\",\"title\": \"Moby Dick\",\"isbn\": \"0-553-21311-3\",\"price\": 8.99},{ \"category\": \"fiction\",\"author\": \"J. R. R. Tolkien\",\"title\": \"The Lord of the Rings\",\"isbn\": \"0-395-19395-8\",\"price\": 22.99}],\"bicycle\": {\"color\": \"red\",\"price\": 19.95}}}";
        return text;
    }
    static const char* book_text()
    {
        static const char* text = "{ \"category\": \"reference\",\"author\": \"Nigel Rees\",\"title\": \"Sayings of the Century\",\"price\": 8.95}";
        return text;
    }

    json book()
    {
        json root = json::parse(jsonpath_filter_fixture::store_text());
        json book = root["store"]["book"];
        return book;
    }

    json bicycle()
    {
        json root = json::parse(jsonpath_filter_fixture::store_text());
        json bicycle = root["store"]["bicycle"];
        return bicycle;
    }
};
#if 0
BOOST_AUTO_TEST_CASE(test_evaluate)
{
    try
    {
        std::string expr1 = "(1 + 1)";

        std::vector<token<json>> tokens;
        tokens.push_back(token<json>(token_types::lparen));
        tokens.push_back(token<json>(token_types::term,std::make_shared<value_term<json>>(json::parse("1"))));
        tokens.push_back(token<json>(token_types::plus));
        tokens.push_back(token<json>(token_types::term, std::make_shared<value_term<json>>(json::parse("1"))));
        tokens.push_back(token<json>(token_types::rparen));

        json context;

        auto result = evaluate(context, tokens);

        BOOST_CHECK(result->evaluate_single_node() == json(2));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(test_evaluate2)
{
    try
    {
        std::string expr1 = "(@.length - 1)";

        std::vector<token<json>> tokens;
        tokens.push_back(token<json>(token_types::lparen));
        tokens.push_back(token<json>(token_types::term,std::make_shared<path_term<json>>("@.length")));
        tokens.push_back(token<json>(token_types::minus));
        tokens.push_back(token<json>(token_types::term,std::make_shared<value_term<json>>(json::parse("1"))));
        tokens.push_back(token<json>(token_types::rparen));

        json context = json::array();
        context.add(1);
        context.add(2);

        auto result = evaluate(context, tokens);
        BOOST_CHECK(result->evaluate_single_node() == json(1));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
BOOST_AUTO_TEST_CASE(test_evaluate3)
{
    try
    {
        std::string expr1 = "!1";

        std::vector<token<json>> tokens;
        tokens.push_back(token<json>(token_types::exclaim));
        tokens.push_back(token<json>(token_types::term,std::make_shared<value_term<json>>(json::parse("1"))));

        json context;

        auto result = evaluate(context, tokens);
        BOOST_CHECK(result->evaluate_single_node() == json(false));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(test_evaluate4)
{
    try
    {
        std::string expr1 = "(-1 + 1)";

        jsonpath_filter_parser<json> parser;
        const char* pend;
        //auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
        
        std::vector<token<json>> tokens;
        tokens.push_back(token<json>(token_types::lparen));
        tokens.push_back(token<json>(token_types::unary_minus));
        tokens.push_back(token<json>(token_types::term,std::make_shared<value_term<json>>(json::parse("1"))));
        tokens.push_back(token<json>(token_types::plus));
        tokens.push_back(token<json>(token_types::term, std::make_shared<value_term<json>>(json::parse("1"))));
        tokens.push_back(token<json>(token_types::rparen));

        json context;

        auto result = evaluate(context, tokens);
        json j = result->evaluate_single_node();
        std::cout << j << std::endl;
        BOOST_CHECK(j == json(0));
        
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}

BOOST_AUTO_TEST_CASE(test_jsonpath_filter)
{
    std::string expr1 = "(1 + 1)";
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
    auto result1 = res1.eval(parent);
    BOOST_CHECK_EQUAL(json(2),result1);

    std::string expr2 = "(1 - 1)";
    auto res2 = parser.parse(expr2.c_str(), expr2.c_str()+ expr2.length(), &pend);
    auto result2 = res2.eval(parent);
    BOOST_CHECK_EQUAL(json(0), result2);

    std::string expr3 = "(@.length - 1)";
    auto res3 = parser.parse(expr3.c_str(), expr3.c_str()+ expr3.length(), &pend);
    auto result3 = res3.eval(parent);
    BOOST_CHECK_EQUAL(json(1), result3);

}
BOOST_AUTO_TEST_CASE(test_jsonpath_filter_exclaim)
{
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    std::string expr1 = "(!(1 + 1))";
    auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
    auto result1 = res1.eval(parent);
    BOOST_CHECK_EQUAL(json(false),result1);

    std::string expr2 = "(!0)";
    auto res2 = parser.parse(expr2.c_str(), expr2.c_str()+ expr2.length(), &pend);
    auto result2= res2.eval(parent);
    BOOST_CHECK_EQUAL(json(true),result2);
}


BOOST_AUTO_TEST_CASE(test_jsonpath_index_expression)
{
    json root = json::parse(jsonpath_filter_fixture::store_text());
    //std::cout << pretty_print(root) << std::endl;
    //std::cout << "$..book[(@.length-1)]" << std::endl;

    json result = json_query(root,"$..book[(@.length-1)]");

    BOOST_CHECK_EQUAL(1,result.size());
    BOOST_CHECK_EQUAL(root["store"]["book"][3],result[0]);

    //    std::cout << pretty_print(result) << std::endl;
}

BOOST_AUTO_TEST_CASE(test_jsonpath_filter_negative_numbers)
{
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    std::string expr1 = "(-1 + 1)";
    auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
    auto result1 = res1.eval(parent);
    BOOST_CHECK_EQUAL(json(0),result1);

    std::string expr2 = "(1 + -1)";
    auto res2 = parser.parse(expr2.c_str(), expr2.c_str()+ expr2.length(), &pend);
    auto result2 = res2.eval(parent);
    BOOST_CHECK_EQUAL(json(0), result2);

    std::string expr3 = "(-1 - -1)";
    auto res3 = parser.parse(expr3.c_str(), expr3.c_str()+ expr3.length(), &pend);
    auto result3 = res3.eval(parent);
    BOOST_CHECK_EQUAL(json(0), result3);

    std::string expr4 = "(-1 - -3)";
    auto res4 = parser.parse(expr4.c_str(), expr4.c_str()+ expr4.length(), &pend);
    auto result4 = res4.eval(parent);
    BOOST_CHECK_EQUAL(json(2), result4);

    std::string expr5 = "((-2 < -1) && (-3 > -4))";
    auto res5 = parser.parse(expr5.c_str(), expr5.c_str()+ expr5.length(), &pend);
    auto result5 = res5.eval(parent);
    BOOST_CHECK_EQUAL(json(true), result5);

    std::string expr6 = "((-2 < -1) || (-4 > -3))";
    auto res6 = parser.parse(expr6.c_str(), expr6.c_str()+ expr6.length(), &pend);
    auto result6 = res6.eval(parent);
    BOOST_CHECK_EQUAL(json(true), result6);
}

BOOST_AUTO_TEST_CASE(test_jsonpath_filter_uni)
{
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    std::string expr1 = "(0)";
    auto res = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
    auto result1 = res.eval(parent);

    //std::cout << (int)result1.type_id() << std::endl;
    //std::cout << result1 << std::endl;
    BOOST_CHECK(json(0) == 0);
    BOOST_CHECK(result1 == 0);

    BOOST_CHECK_EQUAL(json(0),result1);
}

#if defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
// GCC 4.8 has broken regex support: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53631
BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(test_jsonpath_filter_regex, 2)
#endif
BOOST_AUTO_TEST_CASE(test_jsonpath_filter_regex)
{
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    std::string expr1 = "('today I go' =~ /today.*?/)";
    auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
    auto result1 = res1.eval(parent);
    BOOST_CHECK_EQUAL(json(true),result1);

    std::string expr2 = "('today I go' =~ /Today.*?/)";
    auto res2 = parser.parse(expr2.c_str(), expr2.c_str()+ expr2.length(), &pend);
    auto result2 = res2.eval(parent);
    BOOST_CHECK_EQUAL(json(false),result2);

    std::string expr3 = "('today I go' =~ /Today.*?/i)";
    auto res3 = parser.parse(expr3.c_str(), expr3.c_str()+ expr3.length(), &pend);
    auto result3 = res3.eval(parent);
    BOOST_CHECK_EQUAL(json(true),result3);
}

BOOST_AUTO_TEST_CASE(test_precedence)
{
    const char* pend;
    jsonpath_filter_parser<json> parser;
    json parent = json::array();
    parent.add(1);
    parent.add(2);

    //BOOST_CHECK(false || true && false);

    //std::string expr5 = "false || true && true";
    //auto res = parser.parse(expr5.c_str(), expr5.c_str()+ expr5.length());
    //auto result5 = res.eval(parent);
    //BOOST_CHECK(result5);
}

BOOST_AUTO_TEST_CASE(test_evaluate5)
{
    try
    {
        const char* pend;
        jsonpath_filter_parser<json> parser;

        json context;

        std::string expr5 = "(-2 < -1)";
        auto res5 = parser.parse(expr5.c_str(), expr5.c_str() + expr5.length(), &pend);
        auto result5 = res5.eval(context);
        BOOST_CHECK_EQUAL(json(true), result5);

        std::string expr1 = "((-2 < -1))";
        auto res1 = parser.parse(expr1.c_str(), expr1.c_str()+ expr1.length(), &pend);
        auto result1 = res1.eval(context);
        BOOST_CHECK_EQUAL(json(true), result1);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}
#endif
BOOST_AUTO_TEST_CASE(test_evaluate6)
{
    try
    {
        const char* pend;
        jsonpath_filter_parser<json> parser;

        jsonpath_filter_fixture fixture;

        json context = json::parse(jsonpath_filter_fixture::store_text());

        std::string expr5 = "(@.category != @.category)";
        auto res5 = parser.parse(expr5.c_str(), expr5.c_str() + expr5.length(), &pend);
        auto result5 = res5.eval(context);
        BOOST_CHECK_EQUAL(json(false), result5);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}
BOOST_AUTO_TEST_SUITE_END()

