#include <check.h>

#include <errno.h>
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
    ck_assert(0 == ll_insert(ll, &data, &cfg_int));

    ck_assert(0 == ll_delete(ll, &data, &cfg_int));
    ck_assert(0 != ll_delete(ll, &data, &cfg_int));

    free(ll);
}
END_TEST

START_TEST (test_ll_delete_data) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int data = 7;

    ck_assert(0 == ll_insert(ll, &data, &cfg_int));
    ck_assert(&data == ll_find(ll, &data, &cfg_int));

    ck_assert(0 == ll_delete(ll, &data, &cfg_int));
    ck_assert(&data != ll_find(ll, &data, &cfg_int));

    ck_assert(0 != ll_delete(ll, ll->head, &cfg_int));

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
        ck_assert(0 == ll_insert(ll, &(data[i]), &cfg_int));
        ck_assert(&(data[i]) == ll_find(ll, &(data[i]), &cfg_int));
    }

    for (i = 0; i < 10; i++) {
        ck_assert(0 == ll_delete(ll, &(data[i]), &cfg_int));

        /*
         * check that none of the elements in the LL holds the data we just
         * removed
         */
        ck_assert(&(data[i]) != ll_find(ll, &(data[i]), &cfg_int));
        ck_assert(NULL == ll_find(ll, &(data[i]), &cfg_int));
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

    ck_assert(0 == ll_insert(ll, &(data[0]), &cfg_int));
    ck_assert(0 != ll_insert(ll, &(data[1]), &cfg_int));

    ck_assert(0 == ll_delete(ll, &(data[0]), &cfg_int));

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
        ck_assert(0 == ll_insert(ll, &(data[i]), &cfg_int));
    }

    ndel = ll_ndel(ll, predicate_lower_five, NULL, &cfg_int);

    /* We delete 0-4, which are 5 numbers */
    ck_assert(ndel == 5);

    for (i = 0; i < 5; i++) {
        ck_assert(NULL == ll_find(ll, &i, &cfg_int));
    }
    for (i = 5; i < 10; i++) {
        ck_assert(NULL != ll_find(ll, &i, &cfg_int));
        ck_assert(&i != ll_find(ll, &i, &cfg_int));
    }

    ll_destroy(ll, &cfg_int);
    free(ll);
}
END_TEST

START_TEST (test_ll_is_empty_after_insertion_and_deletion) {
    struct ll* ll = malloc(sizeof(*ll));
    int data[] = { 0, 1 };

    ck_assert(0 == ll_insert(ll, &(data[0]), &cfg_int));
    ck_assert(0 == ll_insert(ll, &(data[1]), &cfg_int));

    struct ll_element* old_head = ll->head;
    ck_assert(0 == ll_delete(ll, &(data[0]), &cfg_int));
    ck_assert(old_head != ll->head);

    ck_assert(0 == ll_delete(ll, &(data[1]), &cfg_int));

    ck_assert(ll_is_empty(ll));
    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_count) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int i;
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (i = 0; i < 10; i++) {
        ck_assert(0 == ll_insert(ll, &(data[i]), &cfg_int));
    }

    ck_assert(ll_count(ll) == 10);
    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_subset) {
    struct ll* ll = calloc(1, sizeof(*ll));
    int i;
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (i = 0; i < 10; i++) {
        ck_assert(ll_insert(ll, &(data[i]), &cfg_int) == 0);
    }

    ck_assert(ll_is_subset(ll, ll, &cfg_int) == 1);

    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_subset_distinct_wrong) {
    struct ll* ll = calloc(1, sizeof(*ll));
    struct ll* ll2 = calloc(1, sizeof(*ll2));
    int i;
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3, 4, 5, 6,    8, 9, 10 };

    for (i = 0; i < 10; i++) {
        ck_assert(ll_insert(ll, &(data[i]), &cfg_int) == 0);
        ck_assert(ll_insert(ll2, &(data2[i]), &cfg_int) == 0);
    }

    ck_assert(ll_is_subset(ll2, ll, &cfg_int) != 1);
    ck_assert(ll_is_subset(ll, ll2, &cfg_int) != 1);

    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_subset_distinct) {
    struct ll* ll = calloc(1, sizeof(*ll));
    struct ll* ll2 = calloc(1, sizeof(*ll2));
    int i;
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3 };

    for (i = 0; i < 10; i++) {
        ck_assert(ll_insert(ll, &(data[i]), &cfg_int) == 0);
    }
    for (i = 0; i < 4; i++) {
        ck_assert(ll_insert(ll2, &(data2[i]), &cfg_int) == 0);
    }

    ck_assert(ll_is_subset(ll2, ll, &cfg_int) == 1);
    ck_assert(ll_is_subset(ll, ll2, &cfg_int) != 1);

    ll_destroy(ll, &cfg_int);
}
END_TEST

START_TEST (test_ll_union) {
    struct ll* a = calloc(1, sizeof(*a));
    struct ll* b = calloc(1, sizeof(*b));
    struct ll* c = calloc(1, sizeof(*c));

    int i;
    int data_a[] = { 0, 1, 2, 3, 4 };
    int data_b[] = { 5, 6, 7, 8, 9 };

    for (i = 0; i < 5; i++) {
        ck_assert(0 == ll_insert(a, &(data_a[i]), &cfg_int));
        ck_assert(0 == ll_insert(b, &(data_b[i]), &cfg_int));
    }

    ck_assert(0 == ll_union(c, a, &cfg_int));
    ck_assert(0 == ll_union(c, b, &cfg_int));

    for (i = 0; i < 5; i++) {
        ck_assert(&data_a[i] == ll_find(c, &(data_a[i]), &cfg_int));
        ck_assert(&data_b[i] == ll_find(c, &(data_b[i]), &cfg_int));
    }
}
END_TEST

START_TEST (test_ll_union_overlap) {
    struct ll* a = calloc(1, sizeof(*a));
    struct ll* b = calloc(1, sizeof(*b));
    struct ll* c = calloc(1, sizeof(*c));

    int i;
    int data_a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data_b[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (i = 0; i < 5; i++) {
        ck_assert(0 == ll_insert(a, &(data_a[i]), &cfg_int));
        ck_assert(0 == ll_insert(b, &(data_b[i]), &cfg_int));
    }

    ck_assert(0 == ll_union(c, a, &cfg_int));
    ck_assert(-EEXIST == ll_union(c, b, &cfg_int));

    for (i = 0; i < 5; i++) {
        ck_assert(&data_a[i] == ll_find(c, &(data_a[i]), &cfg_int));
    }

}
END_TEST

Suite*
suite_ll_create(void) {
    Suite* s;
    TCase* case_insert;
    TCase* case_delete;
    TCase* case_empty;
    TCase* case_subset;
    TCase* case_union;

    s = suite_create("Linkedlist");

    /* Test case creation */
    case_insert         = tcase_create("Inserting");
    case_delete         = tcase_create("Deleting");
    case_empty          = tcase_create("Emptyness");
    case_subset          = tcase_create("Subset");
    case_union          = tcase_create("Union");

    /* test adding */
    tcase_add_test(case_insert, test_ll_insert_data);

    tcase_add_test(case_insert, test_ll_insert_multiple);
    tcase_add_test(case_insert, test_ll_insert_multiple_same);
    tcase_add_test(case_delete, test_ll_delete_data);
    tcase_add_test(case_delete, test_ll_delete_by_predicate);

    tcase_add_test(case_empty, test_ll_is_empty_after_insertion_and_deletion);
    tcase_add_test(case_delete, test_ll_count);

    tcase_add_test(case_subset, test_ll_subset);
    tcase_add_test(case_subset, test_ll_subset_distinct);
    tcase_add_test(case_subset, test_ll_subset_distinct_wrong);

    tcase_add_test(case_union, test_ll_union);
    tcase_add_test(case_union, test_ll_union_overlap);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_insert);
    suite_add_tcase(s, case_delete);
    suite_add_tcase(s, case_empty);
    suite_add_tcase(s, case_subset);
    suite_add_tcase(s, case_union);

    return s;
}
