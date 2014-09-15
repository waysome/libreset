#include <check.h>

#include <stdlib.h>
#include "ll.h"

START_TEST (test_ll_insert_data) {
    struct ll* ll = malloc(sizeof(*ll));
    int data = 5;
    ll_insert_data(ll, &data);

    ck_assert(ll->head->data == &data);

    free(ll->head);
    free(ll);
}
END_TEST

START_TEST (test_ll_delete_data) {
    struct ll* ll = malloc(sizeof(*ll));
    int data = 7;

    ll_insert_data(ll, &data);
    ck_assert(ll->head->data == &data);

    ll_delete(ll, ll->head);
    ck_assert(ll->head == NULL);

    free(ll);
}
END_TEST

START_TEST (test_ll_insert_multiple) {
    struct ll* ll = malloc(sizeof(*ll));
    int i;
    int data[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    };

    for (i = 0; i < 10; i++) {
        ck_assert_ptr_eq(ll_insert_data(ll, &(data[i])), ll);
        ck_assert_ptr_eq(ll->head->data, &(data[i]));
    }

    while (ll->head) {
        ck_assert_ptr_eq(ll_delete(ll, ll->head), ll);
    }

    ll_destroy(ll);
}
END_TEST

START_TEST (test_ll_element_alloc) {
    struct ll_element* el = ll_element_alloc();

    ck_assert_ptr_ne(el, NULL);
    ck_assert_ptr_eq(el->data, NULL);
    ck_assert_ptr_eq(el->next, NULL);

    free(el);
}
END_TEST

Suite*
suite_ll_create(void) {
    Suite* s;
    TCase* case_insert;

    s = suite_create("Linkedlist");

    /* Test case creation */
    case_insert = tcase_create("Inserting");
    tcase_add_test(case_insert, test_ll_insert_data);
    tcase_add_test(case_insert, test_ll_delete_data);
    tcase_add_test(case_insert, test_ll_insert_multiple);

    tcase_add_test(case_insert, test_ll_element_alloc);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);

    return s;
}
