#include <check.h>

#include <stdlib.h>

#include "libreset/set.h"
#include "set_cfg.h"

START_TEST (test_r_set_cardinality) {
    struct r_set* set = r_set_new(&cfg_int);
    int data = 1;
    r_set_insert(set, &data);
    ck_assert(1 == r_set_cardinality(set));
}
END_TEST

START_TEST (test_r_set_equal) {
    struct r_set* set = r_set_new(&cfg_int);
    struct r_set* set2 = r_set_new(&cfg_int);
    int data = 1;
    r_set_insert(set, &data);
    ck_assert(1 != r_set_equal(set, set2));

    r_set_insert(set2, &data);
    ck_assert(1 == r_set_equal(set, set2));

    int data2 = 2;
    r_set_insert(set2, &data2);
    ck_assert(1 != r_set_equal(set, set2));
}
END_TEST

START_TEST (test_r_set_union) {
    struct r_set* set1 = r_set_new(&cfg_int);
    struct r_set* set2 = r_set_new(&cfg_int);
    struct r_set* set3 = r_set_new(&cfg_int);

    int data = 1;
    r_set_insert(set2, &data);
    r_set_insert(set3, &data);
    ck_assert(0 == r_set_union(set1, set2, set3));

    ck_assert(1 == r_set_cardinality(set1));
}
END_TEST

Suite*
suite_set_create(void) {
    Suite* s;
    TCase* case_cardinality;
    TCase* case_equality;
    TCase* case_union;

    s = suite_create("Set");

    /* Test case creation */
    case_cardinality  = tcase_create("Cardinality");
    case_equality     = tcase_create("Equality");
    case_union        = tcase_create("Union");

    /* test adding to test cases */
    tcase_add_test(case_cardinality, test_r_set_cardinality);

    tcase_add_test(case_equality, test_r_set_equal);

    tcase_add_test(case_union, test_r_set_union);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_cardinality);
    suite_add_tcase(s, case_equality);
    suite_add_tcase(s, case_union);

    return s;
}

