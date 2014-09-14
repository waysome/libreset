#include <check.h>

/*
 * Include test files below
 */

typedef Suite* (*suite_creator_f)(void);

int
main(void) {
    int nfailed = 0;

    Suite* s;
    SRunner* sr;

    suite_creator_f iter;

    /*
     * Insert suite creator functions here
     */
    suite_creator_f suite_funcs[] = {

        NULL;
    };

    for (iter = suite_funcs[0]; *iter, iter++) {
        s = iter();
        sr = srunner_create(s);
        srunner_run_all(sr, CK_NORMAL);
        nfailed += srunner_ntests_failed(sr);
        srunner_free(sr);
    }

    return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
