#include "set_cfg.h"

#include "libreset/hash.h"


static rs_hash hashf(void const*);
static int cmpf(void const*, void const*);
void const* copyf(void const*);


struct r_set_cfg cfg_int = {
    hashf,
    cmpf,
    copyf,
    NULL
};


static rs_hash hashf(void const* d) {
    return *((int*) d);
}

static int cmpf(void const* a, void const* b) {
    return *((int*) a) == *((int*) b);
}

void const* copyf(void const* d) {
    return d;
}

