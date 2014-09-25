#include <check.h>
#include <stdlib.h>

/*
 * Include test files below
 */
#include "ll/ll_test.c"
#include "avl/avl_tests.c"
#include "ht/ht_tests.c"

typedef Suite* (*suite_creator_f)(void);

#define LENOF(x) (sizeof(x) / sizeof((x)[0]))

int
main(void) {
    int nfailed = 0;

    Suite* s;
    SRunner* sr;

    /*
     * Insert suite creator functions here
     */
    suite_creator_f suite_funcs[] = {
        suite_ll_create,
        suite_avl_create,
        suite_ht_create,
    };

    unsigned long i = 0;
    for (i = 0; i < LENOF(suite_funcs); i++) {
        s = suite_funcs[i]();
        sr = srunner_create(s);
        srunner_run_all(sr, CK_VERBOSE);
        nfailed += srunner_ntests_failed(sr);
        srunner_free(sr);
    }

    return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
