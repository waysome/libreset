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

START_TEST (test_avl_alloc_destroy) {
    struct avl* avl = avl_alloc();

    ck_assert(avl != NULL);
    ck_assert(avl->root == NULL);

    ck_assert_int_eq(1, avl_destroy(avl, NULL));
}
END_TEST

START_TEST (test_avl_add) {
    struct avl* avl = avl_alloc();

    int data = 1;
    rs_hash hash = 1;

    struct avl_el* element = avl_add(avl, hash, &data, NULL);

    ck_assert(element != NULL);
    ck_assert(element->l != NULL);
    ck_assert(element->r != NULL);

    ck_assert_int_eq(element->node_cnt, 0);
    ck_assert_int_eq(element->hash, hash);

    ck_assert(element->ll.head != NULL);
    ck_assert(element->ll.head->next == NULL);
    ck_assert(element->ll.head->data != NULL);
    ck_assert(*(int*) element->ll.head->data == data);

    free(element);
    free(avl);
}
END_TEST

START_TEST (test_avl_add_destroy) {
    struct avl* avl = avl_alloc();

    int data = 1;
    rs_hash hash = 1;

    avl_add(avl, hash, &data, NULL);

    ck_assert_int_eq(1, avl_destroy(avl, NULL));
}
END_TEST

Suite*
suite_avl_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;

    s = suite_create("AVL");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_avl_alloc);
    tcase_add_test(case_allocfree, test_avl_alloc_destroy);

    tcase_add_test(case_adding, test_avl_add);
    tcase_add_test(case_adding, test_avl_add_destroy);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);
    suite_add_tcase(s, case_adding);

    return s;
}

