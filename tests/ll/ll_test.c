#include <check.h>

#include <stdlib.h>
#include "ll.h"

int
cmp_int(
    const void* a,
    const void* b
) {
    return *((const int*) a) == *((const int*) b);
}

struct r_set_cfg cfg = {
    NULL,
    cmp_int,
    NULL,
    NULL
};


START_TEST (test_ll_insert_data) {
    struct ll* ll = malloc(sizeof(*ll));
    int data = 5;
    ll_insert(ll, &data, &cfg);

    ck_assert(ll->head->data == &data);

    free(ll->head);
    free(ll);
}
END_TEST

START_TEST (test_ll_delete_data) {
    struct ll* ll = malloc(sizeof(*ll));
    int data = 7;

    ck_assert(ll_insert(ll, &data, &cfg));
    ck_assert(ll->head->data == &data);

    ck_assert(ll_delete(ll, &data, &cfg));
    ck_assert(ll->head == NULL);

    ck_assert(!ll_delete(ll, ll->head, &cfg));

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
        ck_assert(ll_insert(ll, &(data[i]), &cfg));
    }

    for (i = 0; i < 10; i++) {
        ck_assert(ll_delete(ll, &(data[i]), &cfg));
    }

    ll_destroy(ll, &cfg);
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
    tcase_add_test(case_delete, test_ll_delete_data);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);
    suite_add_tcase(s, case_delete);

    return s;
}
