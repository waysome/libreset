#include <check.h>

#include <stdlib.h>
#include "ll/ll.h"
#include "set_cfg.h"

static int
predicate_lower_five(
    void const* dataptr,
    void* etcptr __attribute__((unused))
) {
    return (*(int*) dataptr < 5);
}


START_TEST (test_ll_insert_data) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int data = 5;
    ll_insert(ll, &data, &cfg_int);

    ck_assert(ll->head->data == &data);

    free(ll->head);
    free(ll);
}
END_TEST

START_TEST (test_ll_delete_data) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int data = 7;

    ck_assert(ll_insert(ll, &data, &cfg_int));
    ck_assert(ll->head->data == &data);

    ck_assert(ll_delete(ll, &data, &cfg_int));
    ck_assert(ll->head == NULL);

    ck_assert(!ll_delete(ll, ll->head, &cfg_int));

    free(ll);
}
END_TEST

START_TEST (test_ll_insert_multiple) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int i;
    int data[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    };

    for (i = 0; i < 10; i++) {
        ck_assert(ll_insert(ll, &(data[i]), &cfg_int));
    }

    for (i = 0; i < 10; i++) {
        ck_assert(ll_delete(ll, &(data[i]), &cfg_int));
    }

    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_insert_multiple_same) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int i;
    int data[] = {
        0, 0
    };

    ck_assert(ll_insert(ll, &(data[0]), &cfg_int));
    ck_assert(!ll_insert(ll, &(data[1]), &cfg_int));

    ck_assert(ll_delete(ll, &(data[0]), &cfg_int));

    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_delete_by_predicate) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int i;
    unsigned int ndel;
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (i = 0; i < 10; i++) {
        ck_assert(ll_insert(ll, &(data[i]), &cfg_int));
    }

    ndel = ll_ndel(ll, predicate_lower_five, NULL, &cfg_int);

    /* We delete 0-4, which are 5 numbers */
    ck_assert_int_eq(ndel, 5);

    struct ll_element* iter = ll->head;
    while (iter) {
        int cmp = *(int*) iter->data;
        ck_assert(cmp >= 5);
        iter = iter->next;
    }

    ll_destroy(ll, &cfg_int);
}
END_TEST

Suite*
suite_ll_create(void) {
    Suite* s;
    TCase* case_insert;
    TCase* case_delete;
    TCase* case_element_alloc;

    s = suite_create("Linkedlist");

    /* Test case creation */
    case_insert         = tcase_create("Inserting");
    case_delete         = tcase_create("Deleting");

    /* test adding */
    tcase_add_test(case_insert, test_ll_insert_data);

    tcase_add_test(case_insert, test_ll_insert_multiple);
    tcase_add_test(case_insert, test_ll_insert_multiple_same);
    tcase_add_test(case_delete, test_ll_delete_data);
    tcase_add_test(case_delete, test_ll_delete_by_predicate);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);
    suite_add_tcase(s, case_delete);

    return s;
}
