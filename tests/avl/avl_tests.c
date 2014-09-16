#include <check.h>

#include "avl.h"

Suite*
suite_avl_create(void) {
    Suite* s;
    TCase* case_insert;

    s = suite_create("AVL");

    /* Test case creation */
    case_insert = tcase_create("");

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);

    return s;
}

