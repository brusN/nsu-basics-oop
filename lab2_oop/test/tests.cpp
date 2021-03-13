//
// Created by mbpc on 24.11.2020.
//

#include "tests.h"
using namespace workflow_executor;

TEST(main_test, test_replace) {
    string t = "1234567 s1234567 sss1234567 1234567ss 1234567 1234567 1234567";
    t = std::regex_replace(t, std::regex("1234567"), "abs");
    std::cout << t;
    SUCCEED();
}

TEST(main_test, test_grep) {
    string word = "cat";
    string line = "cat pidorcat catblack castcat cat cat cat aaaaa aaa aa";
    std::regex word_regex("(cat)");
    std::cmatch result;
    std::regex_match(line.c_str(), result, word_regex);
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i];

}

