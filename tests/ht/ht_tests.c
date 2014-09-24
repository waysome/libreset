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

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);

    return s;
}

