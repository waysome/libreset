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

    ck_assert_int_eq(1, ll_delete(ll, &data, &cfg_int));
    ck_assert_int_eq(0, ll_delete(ll, &data, &cfg_int));

    free(ll);
}
END_TEST

START_TEST (test_ll_delete_data) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int data = 7;

    ck_assert(ll_insert(ll, &data, &cfg_int));
    ck_assert(&data == ll_find(ll, &data, &cfg_int));

    ck_assert(ll_delete(ll, &data, &cfg_int));
    ck_assert(&data != ll_find(ll, &data, &cfg_int));

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

        /* Check if the last element in the LL has the data we just inserted */
        struct ll_element* inserted = ll->head;
        while (inserted->next) {
            inserted = inserted->next;
        }
        ck_assert_int_eq(*(int*)inserted->data, data[i]);
    }

    for (i = 0; i < 10; i++) {
        ck_assert(ll_delete(ll, &(data[i]), &cfg_int));

        /*
         * check that none of the elements in the LL holds the data we just
         * removed
         */
        struct ll_element* iter;
        for (iter = ll->head; iter; iter = iter->next) {
            ck_assert_int_ne(*(int*)iter->data, data[i]);
        }
    }

    ck_assert(ll->head == NULL);

    ll_destroy(ll, &cfg_int);
    free(ll);
}
END_TEST

START_TEST (test_ll_insert_multiple_same) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int data[] = {
        0, 0
    };

    ck_assert(ll_insert(ll, &(data[0]), &cfg_int));
    ck_assert(!ll_insert(ll, &(data[1]), &cfg_int));

    ck_assert(ll_delete(ll, &(data[0]), &cfg_int));

    ll_destroy(ll, &cfg_int);
    free(ll);
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
    free(ll);
}
END_TEST

START_TEST (test_ll_is_empty_after_insertion_and_deletion) {
    struct ll* ll = malloc(sizeof(*ll));
    int data[] = { 0, 1 };

    ll_insert(ll, &(data[0]), &cfg_int);
    ll_insert(ll, &(data[1]), &cfg_int);

    struct ll_element* old_head = ll->head;
    ll_delete(ll, &(data[0]), &cfg_int);
    ck_assert(old_head != ll->head);

    ll_delete(ll, &(data[1]), &cfg_int);

    ck_assert(ll_is_empty(ll));

    ll_destroy(ll, &cfg_int);
}
END_TEST

Suite*
suite_ll_create(void) {
    Suite* s;
    TCase* case_insert;
    TCase* case_delete;
    TCase* case_empty;

    s = suite_create("Linkedlist");

    /* Test case creation */
    case_insert         = tcase_create("Inserting");
    case_delete         = tcase_create("Deleting");
    case_empty          = tcase_create("Emptyness");

    /* test adding */
    tcase_add_test(case_insert, test_ll_insert_data);

    tcase_add_test(case_insert, test_ll_insert_multiple);
    tcase_add_test(case_insert, test_ll_insert_multiple_same);
    tcase_add_test(case_delete, test_ll_delete_data);
    tcase_add_test(case_delete, test_ll_delete_by_predicate);

    tcase_add_test(case_empty, test_ll_is_empty_after_insertion_and_deletion);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);
    suite_add_tcase(s, case_delete);
    suite_add_tcase(s, case_empty);

    return s;
}
