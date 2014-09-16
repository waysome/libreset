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

START_TEST (test_avl_add_multiple) {
    struct avl* avl = avl_alloc();

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    rs_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    struct avl_el* elements[10];

    int i;
    for (i = 0; i < 10; i++) {
        struct avl_el* element = avl_add(avl, hash[i], &data[i], NULL);

        ck_assert(element != NULL);

        ck_assert_int_eq(element->hash, hash[i]);

        ck_assert(element->ll.head != NULL);
        ck_assert(element->ll.head->next == NULL); /* No similar hashes */
        ck_assert(element->ll.head->data != NULL);
        ck_assert(*(int*) element->ll.head->data == data[i]);

        elements[i] = element;
    }

    for (i = 0; i < 10; i++) {
        free(elements[i]);
    }

    free(avl);
}
END_TEST

START_TEST (test_avl_add_multiple_destroy) {
    struct avl* avl = avl_alloc();

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    rs_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_add(avl, hash[i], &data[i], NULL);
    }

    ck_assert_int_eq(1, avl_destroy(avl, NULL));
}
END_TEST

START_TEST (test_avl_add_collisions) {
    struct avl* avl = avl_alloc();

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    rs_hash hash = 0;
    struct avl_el* element = NULL;
    struct avl_el* cmp = NULL;

    int i;
    for (i = 0; i < 10; i++) {
        if (element == NULL) {
            element = avl_add(avl, hash, &data[i], NULL);
        } else {
            cmp = avl_add(avl, hash, &data[i], NULL);
            ck_assert(cmp == element); /* collisions */
        }
    }

    ck_assert(avl->root != NULL);
    ck_assert(avl->root->l == NULL);
    ck_assert(avl->root->r == NULL);

    ck_assert_int_eq(1, avl_destroy(avl, NULL));
}
END_TEST

START_TEST (test_avl_find_single) {
    struct avl* avl = avl_alloc();
    int data        = 1;
    rs_hash hash    = 1;
    struct avl_el* found;

    avl_add(avl, hash, &data, NULL);
    found = avl_find(avl, hash, NULL, NULL);

    ck_assert(found == avl->root);
    ck_assert_int_eq(1, avl_destroy(avl, NULL));
}
END_TEST


START_TEST (test_avl_find_multiple) {
    struct avl* avl = avl_alloc();
    int data[]      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    rs_hash hash[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    struct avl_el* found[10] = { NULL };

    int i;
    int j;

    for (i = 0; i < 10; i++) {
        avl_add(avl, hash[i], &data[i], NULL);
    }

    for (i = 0; i < 10; i++) {
        found[i] = avl_find(avl, hash[i], NULL, NULL);
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (i != j) {
                ck_assert(found[i] != found[j]);
            }
        }
    }

    avl_destroy(avl, NULL);
}
END_TEST

Suite*
suite_avl_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;
    TCase* case_finding;

    s = suite_create("AVL");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");
    case_finding    = tcase_create("Finding");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_avl_alloc);
    tcase_add_test(case_allocfree, test_avl_alloc_destroy);

    tcase_add_test(case_adding, test_avl_add);
    tcase_add_test(case_adding, test_avl_add_destroy);
    tcase_add_test(case_adding, test_avl_add_multiple);
    tcase_add_test(case_adding, test_avl_add_multiple_destroy);
    tcase_add_test(case_adding, test_avl_add_collisions);

    tcase_add_test(case_finding, test_avl_find_single);
    tcase_add_test(case_finding, test_avl_find_multiple);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);
    suite_add_tcase(s, case_adding);
    suite_add_tcase(s, case_finding);

    return s;
}

