#include "test_set.h"

using namespace tritClass;

TEST(TritClass_Tests, logicalNOT_TritValue) {
    ASSERT_EQ(TRUE, !FALSE); // operator! for TritValue
    ASSERT_EQ(FALSE, !TRUE);
    ASSERT_EQ(UNKNOWN, !UNKNOWN);
}

TEST(TritClass_Tests, logicalAND_TritValue) {
    ASSERT_EQ(FALSE, FALSE & FALSE); // operator& for TritValue
    ASSERT_EQ(FALSE, FALSE & UNKNOWN);
    ASSERT_EQ(FALSE, FALSE & TRUE);

    ASSERT_EQ(FALSE, UNKNOWN & FALSE);
    ASSERT_EQ(UNKNOWN, UNKNOWN & UNKNOWN);
    ASSERT_EQ(UNKNOWN, UNKNOWN & TRUE);

    ASSERT_EQ(FALSE, TRUE & FALSE);
    ASSERT_EQ(UNKNOWN, TRUE & UNKNOWN);
    ASSERT_EQ(TRUE, TRUE & TRUE);
}

TEST(TritClass_Tests, logicalOR_TritValue) {
    ASSERT_EQ(FALSE, FALSE | FALSE); // operator| for TritValue
    ASSERT_EQ(UNKNOWN, FALSE | UNKNOWN);
    ASSERT_EQ(TRUE, FALSE | TRUE);

    ASSERT_EQ(UNKNOWN, UNKNOWN | FALSE);
    ASSERT_EQ(UNKNOWN, UNKNOWN | UNKNOWN);
    ASSERT_EQ(TRUE, UNKNOWN | TRUE);

    ASSERT_EQ(TRUE, TRUE | FALSE);
    ASSERT_EQ(TRUE, TRUE | UNKNOWN);
    ASSERT_EQ(TRUE, TRUE | TRUE);
}

TEST(TritSet_tests, getSize_test) {
    TritSet set(1000);

    set[999] = UNKNOWN;
    ASSERT_EQ(set.getSize(), 1000);

    set[1000] = UNKNOWN;
    ASSERT_EQ(set.getSize(), 1000);

    set[2000] = UNKNOWN;
    ASSERT_EQ(set.getSize(), 1000);

    set[999] = TRUE;
    ASSERT_EQ(set.getSize(), 1000);

    set[1000] = TRUE;
    ASSERT_EQ(set.getSize(), 1001);

    set[2000] = FALSE;
    ASSERT_EQ(set.getSize(), 2001);
}

TEST(TritSet_tests, getInitSize_test) {
    TritSet set(1000);

    set[999] = UNKNOWN;
    ASSERT_EQ(set.getInitSize(), 1000);

    set[1000] = UNKNOWN;
    ASSERT_EQ(set.getInitSize(), 1000);

    set[2000] = UNKNOWN;
    ASSERT_EQ(set.getInitSize(), 1000);

    set[999] = TRUE;
    ASSERT_EQ(set.getInitSize(), 1000);

    set[1000] = TRUE;
    ASSERT_EQ(set.getInitSize(), 1000);

    set[2000] = FALSE;
    ASSERT_EQ(set.getInitSize(), 1000);

    set.shrink();
    ASSERT_EQ(set.getInitSize(), 1000);
}

TEST(TritSet_tests, getLastIndex_test) {
    TritSet set(1000);
    ASSERT_EQ(-1, set.getLastIndex());

    set[400] = TRUE;
    ASSERT_EQ(400, set.getLastIndex());

    set[200] = FALSE;
    ASSERT_EQ(400, set.getLastIndex());

    set[600] = TRUE;
    ASSERT_EQ(600, set.getLastIndex());

    set.shrink();
    ASSERT_EQ(600, set.getLastIndex());
}

TEST(TritSet_tests, copyConstructor_test) {
    TritSet set1(1000);
    for (size_t i = 0; i < 10; ++i)
        set1[i] = TRUE;
    set1[200] = FALSE;
    set1[600] = TRUE;

    TritSet set2 = set1;
    ASSERT_EQ(set1.getSize(), set2.getSize());
    ASSERT_EQ(set1.getLastIndex(), set2.getLastIndex());
    ASSERT_EQ(set1.getArrayLength(), set2.getArrayLength());
    ASSERT_EQ(set1.capacity(), set2.capacity());
    ASSERT_EQ(set1.getInitSize(), set2.getInitSize());
    for (size_t i = 0; i < set2.getSize(); ++i) {
        if (set1[i] != set2[i])
            FAIL();
    }
}

TEST(TritSet_tests, assigmentOperator_test) {
    TritSet set1(1000);
    for (size_t i = 0; i < 10; ++i)
        set1[i] = TRUE;
    set1[200] = FALSE;
    set1[600] = TRUE;

    TritSet set2(100);
    set2[3] = TRUE;// set
    set2[99] = FALSE;
    set2[86] = TRUE;

    set2 = set1;

    ASSERT_EQ(set1.getSize(), set2.getSize());
    ASSERT_EQ(set1.getLastIndex(), set2.getLastIndex());
    ASSERT_EQ(set1.getArrayLength(), set2.getArrayLength());
    ASSERT_EQ(set1.capacity(), set2.capacity());
    for (size_t i = 0; i < set2.getSize(); ++i) {
        if (set1[i] != set2[i])
            FAIL();
    }
}

TEST(TritSet_tests, setAndGetTrit_test) {
    TritSet set(1000);

    set[1000] = UNKNOWN;
    ASSERT_EQ(set[1000], UNKNOWN);
    ASSERT_EQ(set.getSize(), 1000);
    ASSERT_EQ(set.getLastIndex(), -1);

    set[400] = TRUE;
    ASSERT_EQ(set[400], TRUE);
    ASSERT_EQ(set.getSize(), 1000);
    ASSERT_EQ(set.getLastIndex(), 400);

    set[1000] = TRUE;
    ASSERT_EQ(set[1000], TRUE);
    ASSERT_EQ(set.getSize(), 1001);
    ASSERT_EQ(set.getLastIndex(), 1000);

    set[500] = TRUE;
    ASSERT_EQ(set[500], TRUE);
    ASSERT_EQ(set.getSize(), 1001);
    ASSERT_EQ(set.getLastIndex(), 1000);

    set[2000] = TRUE;
    ASSERT_EQ(set[2000], TRUE);
    ASSERT_EQ(set.getSize(), 2001);
    ASSERT_EQ(set.getLastIndex(), 2000);
}

TEST(TritSet_tests, squareBracketsOperator_test) {
    TritSet set(10);
    ASSERT_EQ(typeid(set[0]), typeid(TritSet::Trit));
}

TEST(TritSet_tests, setOperatorNOT_test) {
    TritSet set1(1000);
    for (size_t i = 0; i < 3; ++i) {
        set1[i * 3 + 1] = TRUE; // [ U, T, F, U, T, F, U, T, F ]
        set1[i * 3 + 2] = FALSE;
    }

    TritSet set2 = !set1;
    ASSERT_EQ(1000, set2.getSize());

    for (size_t i = 0; i < 9; ++i)
        ASSERT_EQ(set2[i], !set1[i]);
}

TEST(TritSet_tests, setOperatorAND_test) {
    TritSet set1(9);
    TritSet set2(1000);
    for (size_t i = 0; i < 3; ++i) {
        set1[i * 3 + 1] = TRUE; // [ U, T, F, U, T, F, U, T, F ]
        set1[i * 3 + 2] = FALSE;

        set2[i + 3] = TRUE; // [ U, U, U, T, T, T, F, F, F, U]
        set2[i + 6] = FALSE;
    }

    TritSet set3 = set1 & set2;
    ASSERT_EQ(1000, set3.getSize());
    ASSERT_EQ(8, set3.getLastIndex());
    for (size_t i = 0; i < 9; ++i)
        ASSERT_EQ(set3[i], set1[i] & set2[i]);
}

TEST(TritSet_tests, setOperatorOR_test) {
    TritSet set1(9);
    TritSet set2(1000);
    for (size_t i = 0; i < 3; ++i) {
        set1[i * 3 + 1] = TRUE; // [ U, T, F, U, T, F, U, T, F ]
        set1[i * 3 + 2] = FALSE;

        set2[i + 3] = TRUE; // [ U, U, U, T, T, T, F, F, F, U]
        set2[i + 6] = FALSE;
    }

    TritSet set3 = set1 | set2;
    ASSERT_EQ(1000, set3.getSize());
    ASSERT_EQ(8, set3.getLastIndex());
    for (size_t i = 0; i < 9; ++i)
        ASSERT_EQ(set3[i], set1[i] | set2[i]);
}

TEST(TritSet_tests, shrink_test) {
    TritSet set(1000);
    set.shrink();
    ASSERT_EQ(1000, set.getSize());

    set[2000] = TRUE;
    set[1500] = TRUE;
    set[2000] = UNKNOWN;
    set.shrink();
    ASSERT_EQ(set.getSize(), 1501);

    set[100] = TRUE;
    set[1500] = UNKNOWN;
    set.shrink();
    ASSERT_EQ(set.getSize(), 101);
}

TEST(TritSet_tests, cardinatily_test) {
    TritSet set(1000);

    size_t countUnknown = set.cardinatily(UNKNOWN);
    size_t countFalse = set.cardinatily(FALSE);
    size_t countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(1000, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(0, countTrue);

    set[999] = TRUE;
    countUnknown = set.cardinatily(UNKNOWN);
    countFalse = set.cardinatily(FALSE);
    countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(999, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(1, countTrue);

    set[100] = TRUE;
    countUnknown = set.cardinatily(UNKNOWN);
    countFalse = set.cardinatily(FALSE);
    countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(998, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(2, countTrue);

    for (size_t i = 0; i < 10; ++i)
        set[i] = FALSE;
    countUnknown = set.cardinatily(UNKNOWN);
    countFalse = set.cardinatily(FALSE);
    countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(988, countUnknown);
    ASSERT_EQ(10, countFalse);
    ASSERT_EQ(2, countTrue);
}

TEST(TritSet_tests, cardinatily_map_test) {
    TritSet set(1000);
    std::unordered_map<TritValue, size_t, std::hash<TritValue>> map = set.cardinatily();

    size_t countUnknown = map[UNKNOWN];
    size_t countTrue = map[TRUE];
    size_t countFalse = map[FALSE];
    ASSERT_EQ(1000, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(0, countTrue);

    set[999] = TRUE;
    map = set.cardinatily();
    countUnknown = map[UNKNOWN];
    countTrue = map[TRUE];
    countFalse = map[FALSE];
    ASSERT_EQ(999, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(1, countTrue);

    set[100] = TRUE;
    map = set.cardinatily();
    countUnknown = map[UNKNOWN];
    countTrue = map[TRUE];
    countFalse = map[FALSE];
    ASSERT_EQ(998, countUnknown);
    ASSERT_EQ(0, countFalse);
    ASSERT_EQ(2, countTrue);

    for (size_t i = 0; i < 10; ++i)
        set[i] = FALSE;
    map = set.cardinatily();
    countUnknown = map[UNKNOWN];
    countTrue = map[TRUE];
    countFalse = map[FALSE];
    ASSERT_EQ(988, countUnknown);
    ASSERT_EQ(10, countFalse);
    ASSERT_EQ(2, countTrue);
}

TEST(TritSet_tests, trim_test) {
    TritSet set(1000);
    for (size_t i = 0; i < 20; ++i)
        set[i] = TRUE;
    size_t countTrue = set.cardinatily(TRUE);

    set.trim(20);
    ASSERT_EQ(set.getLastIndex(), 19);
    ASSERT_EQ(countTrue, 20);

    set.trim(19);
    countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(set.getLastIndex(), 18);
    ASSERT_EQ(countTrue, 19);

    set.trim(0);
    countTrue = set.cardinatily(TRUE);
    ASSERT_EQ(set.getLastIndex(), -1);
    ASSERT_EQ(countTrue, 0);
}

TEST(Trit_tests, assigmentOperator_TritValue_test) {
    TritSet set(10);
    TritSet::Trit trit(set, 1);

    trit = FALSE;
    ASSERT_EQ(set[1], FALSE);

    trit = TRUE;
    ASSERT_EQ(set[1], TRUE);

    trit = UNKNOWN;
    ASSERT_EQ(set[1], UNKNOWN);

}

TEST(Trit_tests, assigmentOperator_Trit_test) {
    TritSet set(10);
    TritSet::Trit trit1(set, 1);
    TritSet::Trit trit2(set, 2);

    trit1 = FALSE;
    trit2 = trit1;
    ASSERT_EQ(set[2], FALSE);

    trit1 = TRUE;
    trit2 = trit1;
    ASSERT_EQ(set[2], TRUE);

    trit1 = UNKNOWN;
    trit2 = trit1;
    ASSERT_EQ(set[2], UNKNOWN);
}

TEST(Trit_tests, logicOperatorAND_Trit_test) {
    TritSet set(10);
    set[1] = FALSE;
    set[2] = TRUE;
    TritSet::Trit tritUnknown(set, 0);
    TritSet::Trit tritFalse(set, 1);
    TritSet::Trit tritTrue(set, 2);

    tritFalse = FALSE;
    tritTrue = TRUE;

    ASSERT_EQ(FALSE, tritFalse & tritFalse);
    ASSERT_EQ(FALSE, tritFalse & tritUnknown);
    ASSERT_EQ(FALSE, tritFalse & tritTrue);

    ASSERT_EQ(FALSE, tritUnknown & tritFalse);
    ASSERT_EQ(UNKNOWN, tritUnknown & tritUnknown);
    ASSERT_EQ(UNKNOWN, tritUnknown & tritTrue);

    ASSERT_EQ(FALSE, tritTrue & tritFalse);
    ASSERT_EQ(UNKNOWN, tritTrue & tritUnknown);
    ASSERT_EQ(TRUE, tritTrue & tritTrue);
}

TEST(Trit_tests, logicOperatorAND_TritValue_test) {
    TritSet set(10);
    set[1] = FALSE;
    set[2] = TRUE;
    TritSet::Trit tritUnknown(set, 0);
    TritSet::Trit tritFalse(set, 1);
    TritSet::Trit tritTrue(set, 2);

    tritFalse = FALSE;
    tritTrue = TRUE;

    ASSERT_EQ(FALSE, tritFalse & FALSE);
    ASSERT_EQ(FALSE, tritFalse & UNKNOWN);
    ASSERT_EQ(FALSE, tritFalse & TRUE);

    ASSERT_EQ(FALSE, tritUnknown & FALSE);
    ASSERT_EQ(UNKNOWN, tritUnknown & UNKNOWN);
    ASSERT_EQ(UNKNOWN, tritUnknown & TRUE);

    ASSERT_EQ(FALSE, tritTrue & FALSE);
    ASSERT_EQ(UNKNOWN, tritTrue & UNKNOWN);
    ASSERT_EQ(TRUE, tritTrue & TRUE);
}

TEST(Trit_tests, logicOperatorOR_Trit_test) {
    TritSet set(10);
    set[1] = FALSE;
    set[2] = TRUE;
    TritSet::Trit tritUnknown(set, 0);
    TritSet::Trit tritFalse(set, 1);
    TritSet::Trit tritTrue(set, 2);

    tritFalse = FALSE;
    tritTrue = TRUE;

    ASSERT_EQ(FALSE, tritFalse | tritFalse);
    ASSERT_EQ(UNKNOWN, tritFalse | tritUnknown);
    ASSERT_EQ(TRUE, tritFalse | tritTrue);

    ASSERT_EQ(UNKNOWN, tritUnknown | tritFalse);
    ASSERT_EQ(UNKNOWN, tritUnknown | tritUnknown);
    ASSERT_EQ(TRUE, tritUnknown | tritTrue);

    ASSERT_EQ(TRUE, tritTrue | tritFalse);
    ASSERT_EQ(TRUE, tritTrue | tritUnknown);
    ASSERT_EQ(TRUE, tritTrue | tritTrue);
}

TEST(Trit_tests, logicOperatorOR_TritValue_test) {
    TritSet set(10);
    set[1] = FALSE;
    set[2] = TRUE;
    TritSet::Trit tritUnknown(set, 0);
    TritSet::Trit tritFalse(set, 1);
    TritSet::Trit tritTrue(set, 2);

    tritFalse = FALSE;
    tritTrue = TRUE;

    ASSERT_EQ(FALSE, tritFalse | FALSE);
    ASSERT_EQ(UNKNOWN, tritFalse | UNKNOWN);
    ASSERT_EQ(TRUE, tritFalse | TRUE);

    ASSERT_EQ(UNKNOWN, tritUnknown | FALSE);
    ASSERT_EQ(UNKNOWN, tritUnknown | UNKNOWN);
    ASSERT_EQ(TRUE, tritUnknown | TRUE);

    ASSERT_EQ(TRUE, tritTrue | FALSE);
    ASSERT_EQ(TRUE, tritTrue | UNKNOWN);
    ASSERT_EQ(TRUE, tritTrue | TRUE);
}

TEST(Trit_tests, logicOperatorNOT_test) {
    TritSet set(10);
    TritSet::Trit trit(set, 0);

    trit = !trit;
    ASSERT_EQ(set[0], UNKNOWN);

    trit = FALSE;
    trit = !trit;
    ASSERT_EQ(set[0], TRUE);

    trit = TRUE;
    trit = !trit;
    ASSERT_EQ(set[0], FALSE);
}

TEST(Cascading_operador_tests, TritValue_cascading_operador_test) {
    std::ostringstream os;
    os << TRUE << " " << FALSE << " " << UNKNOWN;
    std::string output = os.str();
    std::string expectedOutput  = "TRUE FALSE UNKNOWN";

    if (output != expectedOutput)
        FAIL();
    SUCCEED();
}

TEST(Cascading_operador_tests, Trit_cascading_operador_test) {
    TritSet set(10);
    TritSet::Trit tritUnknown(set, 0);
    TritSet::Trit tritFalse(set, 1);
    TritSet::Trit tritTrue(set, 2);

    tritFalse = FALSE;
    tritTrue = TRUE;

    std::ostringstream os;
    os << tritFalse << " " << tritUnknown << " " << tritTrue;
    std::string output = os.str();
    std::string expectedOutput  = "FALSE UNKNOWN TRUE";

    if (output != expectedOutput)
        FAIL();
    SUCCEED();
}

TEST(Cascading_operador_tests, TritSet_cascading_operador_test) {
    TritSet set(10);
    for (size_t i = 5; i < 8; ++i)
        set[i] = TRUE;
    set[9] = FALSE;

    std::ostringstream os;
    os << set;
    std::string output = os.str();
    std::string expectedOutput  = "UNKNOWN UNKNOWN UNKNOWN UNKNOWN UNKNOWN TRUE TRUE TRUE UNKNOWN FALSE";

    if (output != expectedOutput)
// Release
        FAIL();
    SUCCEED();
}

TEST(TritSet_tests, testFromTask) {
    TritSet set(1000);
// length of internal array
    size_t allocLength = set.capacity();
    ASSERT_GE(allocLength, 1000*2 / 8 / sizeof(uint));

//не выделяет никакой памяти
    set[1000] = UNKNOWN;
    ASSERT_EQ(allocLength, set.capacity());

// false, but no exception or memory allocation
    if (set[2000] == TRUE) {}
    ASSERT_EQ(allocLength, set.capacity());

//выделение памяти
    set[2000] = TRUE;
    ASSERT_LT(allocLength, set.capacity());

//no memory operations
    allocLength = set.capacity();
    set[2000] = FALSE;
    set[2000] = UNKNOWN;
    ASSERT_EQ(allocLength, set.capacity());

//освобождение памяти до начального значения или
//до значения необходимого для хранения последнего установленного трита
//в данном случае для трита 1000’000
    set.shrink();
    ASSERT_GT(allocLength, set.capacity());
}

TEST(TritSet_tests, auto_construction_test) {
    TritSet set(10);
    std::ostringstream os;
    for (auto i : set) {
        i = TRUE;
        os << i << " ";
    }
    for (int i = 0; i < set.getSize(); ++i) {
        ASSERT_EQ(set[i], TRUE);
    }

    std::string output = os.str();
    std::string expectedOutput  = "TRUE TRUE TRUE TRUE TRUE TRUE TRUE TRUE TRUE TRUE ";

    if (output != expectedOutput)
        FAIL();
    SUCCEED();
}
