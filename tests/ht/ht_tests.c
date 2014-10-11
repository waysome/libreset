#include <check.h>

#include <stdlib.h>

#include "ht/ht.h"
#include "set_cfg.h"

#define LEN(x) (sizeof((x)) / sizeof((x)[0]))

START_TEST (test_ht_init) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    ck_assert(ht.buckets != NULL);
    ck_assert(ht.sizeexp == 1);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_init_big) {
    size_t exp;

    for (exp = 2; exp < 10; exp++) {
        struct ht ht;
        ck_assert(&ht == ht_init(&ht, exp)); /* allocate 2^exp */

        ck_assert(ht.buckets != NULL);
        ck_assert(ht_nbuckets(&ht) == (1 << exp)); /* (1 << exp) == 2^exp */

        ck_assert(0 == ht_destroy(&ht, &cfg_int));
    }
}
END_TEST

static const struct {
    size_t exp;
    unsigned int nvals;
} map_exp_nvals[] = {
    { .exp =  2,     .nvals = 10 },
    { .exp =  3,     .nvals = 10 },
    { .exp =  4,     .nvals = 10 },
    { .exp =  5,     .nvals = 10 },
    { .exp =  6,     .nvals = 10 },
    { .exp =  7,     .nvals = 10 },
    { .exp =  8,     .nvals = 10 },
    { .exp =  9,     .nvals = 10 },
    { .exp = 10,     .nvals = 10 },

    { .exp =  2,     .nvals = 100 },
    { .exp =  3,     .nvals = 100 },
    { .exp =  4,     .nvals = 100 },
    { .exp =  5,     .nvals = 100 },
    { .exp =  6,     .nvals = 100 },
    { .exp =  7,     .nvals = 100 },
    { .exp =  8,     .nvals = 100 },
    { .exp =  9,     .nvals = 100 },
    { .exp = 10,     .nvals = 100 },

    { .exp =  2,     .nvals = 1000 },
    { .exp =  3,     .nvals = 1000 },
    { .exp =  4,     .nvals = 1000 },
    { .exp =  5,     .nvals = 1000 },
    { .exp =  6,     .nvals = 1000 },
    { .exp =  7,     .nvals = 1000 },
    { .exp =  8,     .nvals = 1000 },
    { .exp =  9,     .nvals = 1000 },
    { .exp = 10,     .nvals = 1000 },
};

START_TEST (test_ht_insert_distinct_values) {
    size_t exp          = map_exp_nvals[_i].exp;
    unsigned int nvals  = map_exp_nvals[_i].nvals;

    struct ht ht;
    ck_assert(&ht == ht_init(&ht, exp));

    /* allocating on not-cleared stack, assuming to be random inside */
    int vals[nvals];

    unsigned int i;
    for (i = nvals; i; --i) {
        vals[i - 1] = i;
        ck_assert(0 == ht_insert(&ht, vals + i - 1, &cfg_int));
    }

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_find_multiple) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
    }

    for (i = 0; i < 10; ++i) {
        ck_assert(&data[i] == ht_find(&ht, &data[i], &cfg_int));
    }

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_del) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
    }

    ck_assert(0 == ht_del(&ht, &data[5], &cfg_int));

    ck_assert(NULL == ht_find(&ht, &data[5], &cfg_int));

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_cardinality) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ck_assert((i + 1) == ht_cardinality(&ht));
    }

    ck_assert(0 == ht_del(&ht, &data[5], &cfg_int));
    ck_assert(9 == ht_cardinality(&ht));

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_equal) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
    }

    ck_assert(ht_equal(&ht, &ht, &cfg_int) == 1);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_equal_wrong) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */
    struct ht ht2;
    ck_assert(&ht2 == ht_init(&ht2, 2)); /* allocate 2^2 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 1, 1, 2, 3, 4, 6, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ht_insert(&ht2, &data2[i], &cfg_int);
    }

    ck_assert(ht_equal(&ht, &ht2, &cfg_int) == 0);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_equal_almost) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */
    struct ht ht2;
    ck_assert(&ht2 == ht_init(&ht2, 2)); /* allocate 2^2 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3, 4, 6, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ht_insert(&ht2, &data2[i], &cfg_int);
    }

    ck_assert(ht_equal(&ht, &ht2, &cfg_int) == 0);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_equal_different_buckets) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */
    struct ht ht2;
    ck_assert(&ht2 == ht_init(&ht2, 3)); /* allocate 2^2 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ht_insert(&ht2, &data2[i], &cfg_int);
    }


    ck_assert(ht_equal(&ht, &ht2, &cfg_int) == 1);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_equal_different_buckets_wrong) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */
    struct ht ht2;
    ck_assert(&ht2 == ht_init(&ht2, 3)); /* allocate 2^3 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3, 4, 5, 6,    8, 9, 10 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ht_insert(&ht2, &data2[i], &cfg_int);
    }

    ck_assert(10 == ht_cardinality(&ht));
    ck_assert(10 == ht_cardinality(&ht2));

    ck_assert(ht_equal(&ht, &ht2, &cfg_int) != 1);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
}
END_TEST

START_TEST (test_ht_union) {
    struct ht ht;
    ck_assert(&ht == ht_init(&ht, 1)); /* allocate 2^1 */

    struct ht ht2;
    ck_assert(&ht2 == ht_init(&ht2, 2)); /* allocate 2^2 */

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 1, 1, 2, 3, 4, 6, 6, 7, 8, 9 };
    int i;
    for (i = 0; i < 10; ++i) {
        ht_insert(&ht, &data[i], &cfg_int);
        ht_insert(&ht2, &data2[i], &cfg_int);
    }

    struct ht u;
    ck_assert(&u == ht_init(&u , 2));

    ck_assert(0 == ht_union(&u, &ht, &cfg_int));
    ck_assert(0 == ht_union(&u, &ht2, &cfg_int));
    ck_assert(ht_equal(&u, &ht, &cfg_int) == 0);

    ck_assert(0 == ht_destroy(&ht, &cfg_int));
    ck_assert(0 == ht_destroy(&ht2, &cfg_int));
    ck_assert(0 == ht_destroy(&u, &cfg_int));
}
END_TEST

Suite*
suite_ht_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;
    TCase* case_finding;
    TCase* case_deleting;
    TCase* case_cardinality;
    TCase* case_equality;
    TCase* case_union;

    s = suite_create("HT");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");
    case_finding    = tcase_create("Finding");
    case_deleting    = tcase_create("Deleting");
    case_cardinality = tcase_create("Cardinality");
    case_equality = tcase_create("Equality");
    case_union      = tcase_create("Union");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_ht_init);
    tcase_add_test(case_allocfree, test_ht_init_big);

    tcase_add_loop_test(case_adding,
                        test_ht_insert_distinct_values,
                        0,
                        LEN(map_exp_nvals));
    tcase_add_test(case_finding, test_ht_find_multiple);

    tcase_add_test(case_deleting, test_ht_del);

    tcase_add_test(case_cardinality, test_ht_cardinality);

    tcase_add_test(case_equality, test_ht_equal);
    tcase_add_test(case_equality, test_ht_equal_wrong);
    tcase_add_test(case_equality, test_ht_equal_almost);
    tcase_add_test(case_equality, test_ht_equal_different_buckets);
    tcase_add_test(case_equality, test_ht_equal_different_buckets_wrong);

    tcase_add_test(case_union, test_ht_union);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);
    suite_add_tcase(s, case_adding);
    suite_add_tcase(s, case_finding);
    suite_add_tcase(s, case_deleting);
    suite_add_tcase(s, case_cardinality);
    suite_add_tcase(s, case_equality);
    suite_add_tcase(s, case_union);

    return s;
}

