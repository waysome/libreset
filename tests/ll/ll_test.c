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

Suite*
suite_ll_create(void) {
    Suite* s;
    TCase* case_insert;

    s = suite_create("Linkedlist");

    /* Test case creation */
    case_insert = tcase_create("Inserting");
    tcase_add_test(case_insert, test_ll_insert_data);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);

    return s;
}
