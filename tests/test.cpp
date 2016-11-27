#include "easycond.h"

#include <cassert>
#include <iostream>
#include <vector>


#define CHECK(test) \
if (!test) { std::cerr << "Test " #test " failed" << std::endl; return 1; }


#define COMPARE_IMPL(actual, exp, line) \
if (actual != exp) { \
    std::cerr << "Compare Fail (line " << line << "): " #actual " == " #exp "" << std::endl; \
    return false; \
}

#define COMPARE(actual, exp) \
COMPARE_IMPL(actual, exp, __LINE__)

struct A
{
};

struct B
{
    void voidMethod() {}
    bool boolMethod() { return false; }
    void voidMethodArg(int /*i*/) {};
};

bool testAny()
{

    {
        const bool val = EasyCond::any<std::true_type,
                                       std::true_type>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::any<std::true_type,
                                       std::false_type>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::any<std::false_type,
                                       std::false_type>::value;
        COMPARE(val, false);
    }

    return true;
}

bool testAll()
{

    {
        const bool val = EasyCond::all<std::true_type>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::all<std::true_type,
                                       std::true_type>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::all<std::false_type,
                                       std::true_type>::value;
        COMPARE(val, false);
    }

    {
        const bool val = EasyCond::all<std::is_same<A, A>,
                                       std::is_same<B, B>>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::all<EasyCond::any<std::true_type,
                                                     std::false_type>,
                                       EasyCond::any<std::true_type,
                                                     std::false_type>>::value;
        COMPARE(val, true);
    }

    return true;
}

bool testIsIterable()
{
    {
        const bool val = EasyCond::IsIterable<std::vector<int>>::value;
        COMPARE(val, true);
    }

    {
        const bool val = EasyCond::IsIterable<int>::value;
        COMPARE(val, false);
    }

    {
        const bool val = EasyCond::IsIterable<B>::value;
        COMPARE(val, false);
    }

    return true;
}

int main(int, char **)
{
    CHECK(testAny());
    CHECK(testAll());
    CHECK(testIsIterable());

    std::cout << "** Test Sucess" << std::endl;
    return 0;
}
