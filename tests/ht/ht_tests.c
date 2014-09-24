#include <check.h>

#include <stdlib.h>

#include "ht/ht.h"
#include "set_cfg.h"

START_TEST (test_ht_init) {
    struct ht ht;
    ht_init(&ht, 1); /* allocate 2^1 */

    ck_assert(ht.buckets != NULL);
    ck_assert(ht.sizeexp == 1);

    ht_destroy(&ht, &cfg_int);
}
END_TEST

START_TEST (test_ht_init_big) {
    size_t exp;

    for (exp = 2; exp < 10; exp++) {
        struct ht ht;
        ht_init(&ht, exp); /* allocate 2^exp */

        ck_assert(ht.buckets != NULL);
        ck_assert(ht_nbuckets(&ht) == (1 << exp)); /* (1 << exp) == 2^exp */

        ht_destroy(&ht, &cfg_int);
    }
}
END_TEST

static void
test_table_insert_values(
    size_t exp,
    unsigned int nvals
) {
    struct ht ht;
    ht_init(&ht, exp);

    /* allocating on not-cleared stack, assuming to be random inside */
    int vals[nvals];

    unsigned int i;
    for (i = nvals; i; --i) {
        vals[i - 1] = i;
        ck_assert(1 == ht_insert(&ht, vals + i - 1, &cfg_int));
    }

    ht_destroy(&ht, &cfg_int);
}

START_TEST (test_ht_insert_small_table_few_vals) {
    test_table_insert_values(2, 10);
}
END_TEST

START_TEST (test_ht_insert_big_table_few_vals) {
    test_table_insert_values(10, 10);
}
END_TEST

START_TEST (test_ht_insert_small_table_much_vals) {
    test_table_insert_values(2, 1000);
}
END_TEST

Suite*
suite_ht_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;
    TCase* case_finding;

    s = suite_create("HT");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");
    case_finding    = tcase_create("Finding");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_ht_init);
    tcase_add_test(case_allocfree, test_ht_init_big);

    tcase_add_test(case_adding, test_ht_insert_small_table_few_vals);
    tcase_add_test(case_adding, test_ht_insert_big_table_few_vals);
    tcase_add_test(case_adding, test_ht_insert_small_table_much_vals);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);

    return s;
}

