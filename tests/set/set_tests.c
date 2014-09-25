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

Suite*
suite_set_create(void) {
    Suite* s;
    TCase* case_cardinality;

    s = suite_create("Set");

    /* Test case creation */
    case_cardinality  = tcase_create("Cardinality");

    /* test adding to test cases */
    tcase_add_test(case_cardinality, test_r_set_cardinality);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_cardinality);

    return s;
}

