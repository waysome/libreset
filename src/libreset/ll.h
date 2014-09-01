#ifndef __LL_H__
#define __LL_H__

/**
 *
 */
struct ll_element {
    struct ll_element*  next;
    void*               data;
};

struct ll {
    struct ll_element* head;
};

struct ll*
ll_alloc(void);

void
ll_destroy(struct ll* ll);

struct ll*
ll_insert(struct ll* ll, struct ll_element* e);

struct ll*
ll_insert_data(struct ll* ll, void* data);

struct ll*
ll_delete(struct ll* ll, struct ll_element* del);

#define ll_foreach(it,ll) \
    for (struct ll_element* it = ll->head; it; it = it->next)

struct ll_element*
ll_element_alloc(void);

struct ll_element*
ll_element_alloc_new(void* data);

void
ll_element_destroy(struct ll_element*);

#endif
