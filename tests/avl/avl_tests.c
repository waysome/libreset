#include <check.h>

#include <stdlib.h>

#include "avl.h"

START_TEST (test_avl_alloc) {
    struct avl* avl = avl_alloc();

    ck_assert(avl != NULL);
    ck_assert(avl->root == NULL);

    free(avl);
}
END_TEST

Suite*
suite_avl_create(void) {
    Suite* s;
    TCase* case_allocfree;

    s = suite_create("AVL");

    /* Test case creation */
    case_allocfree = tcase_create("Allocating and deleting");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_avl_alloc);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);

    return s;
}

