#include <check.h>

#include <stdlib.h>
#include "ht.h"

START_TEST (test_ht_insert_data) {
}
END_TEST


Suite*
suite_ht_create(void) {
    Suite* s;
    TCase* case_insert;

    s = suite_create("Hashtable");

    /* Test case creation */
    case_insert         = tcase_create("Inserting");

    /* test adding */
    tcase_add_test(case_insert, test_ll_insert_data);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);

    return s;
}
