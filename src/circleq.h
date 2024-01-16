#pragma once

/* CIRCLEQ is a single header intrusive circular linked list.
 *
 * It is inspired by `<sys/queue.h>` from `glibc` and `FreeBSD`.
 */

#define CIRCLEQ_HEAD(name, type) \
    struct name { \
        struct type *cqh_last; \
    }

#define CIRCLEQ_HEAD_INITIALIZER(head) \
    { NULL }

#define CIRCLEQ_ENTRY(type) \
    struct { \
        struct type *cqe_next; \
    }

#define CIRCLEQ_INIT(head) do { \
        (head)->cqh_last = NULL; \
    } while (0)

#define CIRCLEQ_EMPTY(head) \
    ((head)->cqh_last == NULL)

#define CIRCLEQ_LAST(head) \
    ((head)->cqh_last)

#define CIRCLEQ_FIRST(head, field) \
    (CIRCLEQ_EMPTY(head) ? NULL : \
     CIRCLEQ_NEXT(CIRCLEQ_LAST(head), field))

#define CIRCLEQ_NEXT(elm, field) \
    ((elm)->field.cqe_next)

#define CIRCLEQ_INSERT_AFTER(cqelm, elm, field) do { \
        CIRCLEQ_NEXT(elm, field) = CIRCLEQ_NEXT(cqelm, field); \
        CIRCLEQ_NEXT(cqelm, field) = (elm); \
    } while (0)

#define CIRCLEQ_INSERT_HEAD(head, elm, field) do { \
        if (CIRCLEQ_EMPTY(head)) { \
            CIRCLEQ_LAST(head) = CIRCLEQ_NEXT(elm, field) = (elm); \
        } else { \
            CIRCLEQ_INSERT_AFTER(CIRCLEQ_LAST(head), elm, field); \
        } \
    } while (0)

#define CIRCLEQ_INSERT_TAIL(head, elm, field) do { \
        if (CIRCLEQ_EMPTY(head)) { \
            CIRCLEQ_LAST(head) = CIRCLEQ_NEXT(elm, field) = (elm); \
        } else { \
            CIRCLEQ_INSERT_AFTER(CIRCLEQ_LAST(head), elm, field); \
            CIRCLEQ_LAST(head) = (elm); \
        } \
    } while (0)

#define CIRCLEQ_REMOVE_AFTER(head, elm, field) do { \
        if (CIRCLEQ_NEXT(elm, field) == (elm)) { \
            CIRCLEQ_INIT(head); \
        } else if (CIRCLEQ_NEXT(elm, field) == CIRCLEQ_LAST(head)) { \
            CIRCLEQ_NEXT(elm, field) = CIRCLEQ_NEXT(CIRCLEQ_NEXT(elm, field), field); \
            CIRCLEQ_LAST(head) = (elm); \
        } else { \
            CIRCLEQ_NEXT(elm, field) = CIRCLEQ_NEXT(CIRCLEQ_NEXT(elm, field), field); \
        } \
    } while (0)

#define CIRCLEQ_REMOVE_HEAD(head, field) do { \
        if (CIRCLEQ_NEXT(CIRCLEQ_LAST(head), field) == CIRCLEQ_LAST(head)) { \
            CIRCLEQ_INIT(head); \
        } else { \
            CIRCLEQ_NEXT(CIRCLEQ_LAST(head), field) = CIRCLEQ_NEXT(CIRCLEQ_NEXT(CIRCLEQ_LAST(head), field), field); \
        } \
    } while (0)

#define CIRCLEQ_REMOVE(head, elm, type, field) do { \
        if (CIRCLEQ_FIRST(head, field) == (elm)) { \
            CIRCLEQ_REMOVE_HEAD(head, field); \
        } else { \
            struct type *cqelm = CIRCLEQ_FIRST(head, field); \
            while (CIRCLEQ_NEXT(cqelm, field) != (elm)) \
                cqelm = CIRCLEQ_NEXT(cqelm, field);  \
            CIRCLEQ_REMOVE_AFTER(head, cqelm, field); \
        } \
    } while (0)

#define CIRCLEQ_REMOVE_TAIL(head, type, field) do { \
        struct type *cqlast = CIRCLEQ_LAST(head); \
        CIRCLEQ_REMOVE(head, cqlast, type, field); \
    } while (0)

#define CIRCLEQ_CONCAT(head1, head2, type, field) do { \
        if (!CIRCLEQ_EMPTY(head2)) { \
            if (!CIRCLEQ_EMPTY(head1)) { \
                struct type *cqelm = CIRCLEQ_NEXT(CIRCLEQ_LAST(head1), field); \
                CIRCLEQ_NEXT(CIRCLEQ_LAST(head1), field) = CIRCLEQ_NEXT(CIRCLEQ_LAST(head2), field); \
                CIRCLEQ_NEXT(CIRCLEQ_LAST(head2), field) = cqelm; \
            } \
            CIRCLEQ_LAST(head1) = CIRCLEQ_LAST(head2); \
            CIRCLEQ_INIT(head2); \
        } \
    } while (0)

#define CIRCLEQ_SWAP(head1, head2, type, field) do { \
        struct type *cqlast = CIRCLEQ_LAST(head1); \
        CIRCLEQ_LAST(head1) = CIRCLEQ_LAST(head2); \
        CIRCLEQ_LAST(head2) = cqlast; \
    } while (0)

#define CIRCLEQ_FOREACH(var, head, field) \
    for ((var) = CIRCLEQ_FIRST(head, field); \
         (var); \
         (var) = ((var) == CIRCLEQ_LAST(head) ? NULL : CIRCLEQ_NEXT(var, field)))

#define CIRCLEQ_FOREACH_FROM(var, head, field) \
    for ((var) = ((var) ? (var) : CIRCLEQ_FIRST(head, field)); \
         (var); \
         (var) = ((var) == CIRCLEQ_LAST(head) ? NULL : CIRCLEQ_NEXT(var, field)))

#define CIRCLEQ_FOREACH_SAFE(var, head, field, tvar) \
    for ((var) = CIRCLEQ_FIRST(head, field); \
         (var) && ((tvar) = ((var) == CIRCLEQ_LAST(head) ? NULL : CIRCLEQ_NEXT(var, field)), 1); \
         (var) = (tvar))

#define CIRCLEQ_FOREACH_FROM_SAFE(var, head, field, tvar) \
    for ((var) = ((var) ? (var) : CIRCLEQ_FIRST(head, field)); \
         (var) && ((tvar) = ((var) == CIRCLEQ_LAST(head) ? NULL : CIRCLEQ_NEXT(var, field)), 1); \
         (var) = (tvar))

#define CIRCLEQ_MERGE(head1, head2, cmp, arg, type, field) do { \
        struct type *cqprev = NULL; \
        struct type *cqelm1 = CIRCLEQ_FIRST(head1, field); \
        struct type *cqelm2; \
        while ((cqelm2 = CIRCLEQ_FIRST(head2, field))) { \
            CIRCLEQ_REMOVE_HEAD(head2, field); \
            CIRCLEQ_FOREACH_FROM(cqelm1, head1, field) { \
                if ((cmp)(cqelm2, cqelm1, arg) < 0) \
                    break; \
                cqprev = cqelm1; \
            } \
            if (cqprev) { \
                CIRCLEQ_INSERT_AFTER(cqprev, cqelm2, field); \
                if (CIRCLEQ_LAST(head1) == cqprev) \
                    CIRCLEQ_LAST(head1) = cqelm2; \
            } else { \
                CIRCLEQ_INSERT_HEAD(head1, cqelm2, field); \
            } \
            cqprev = cqelm2; \
        } \
    } while (0)

#define CIRCLEQ_MERGE_SUBL(var, i, head, len1, len2, elm, pos, cmp, arg, field) do { \
        while ((pos) < (len1)) { \
            (elm) = CIRCLEQ_NEXT(elm, field); \
            (pos)++; \
        } \
        for ((i) = 0; (i) < (len1); ++(i)) { \
            (var) = CIRCLEQ_FIRST(head, field); \
            while ((pos) < (len1) + (len2) && \
                   (cmp)(var, CIRCLEQ_NEXT(elm, field), arg) >= 0) { \
                (elm) = CIRCLEQ_NEXT(elm, field); \
                (pos)++; \
            } \
            if ((var) != (elm)) { \
                CIRCLEQ_REMOVE_HEAD(head, field); \
                CIRCLEQ_INSERT_AFTER(elm, var, field); \
                if (CIRCLEQ_LAST(head) == (elm)) \
                    CIRCLEQ_LAST(head) = (var); \
                (elm) = (var); \
            } \
        } \
    } while (0)

#define CIRCLEQ_MERGESORT(head, cmp, arg, type, field) do { \
        CIRCLEQ_HEAD(, type) cqhead = CIRCLEQ_HEAD_INITIALIZER(cqhead); \
        unsigned long cqslen = 0, cqmask, cqmpos, cqidx; \
        struct type *cqmelm, *cqtvar; \
        while ((cqmelm = CIRCLEQ_FIRST(head, field))) { \
            CIRCLEQ_REMOVE_HEAD(head, field); \
            CIRCLEQ_INSERT_HEAD(&cqhead, cqmelm, field); \
            for (cqmask = cqmpos = 1, cqslen++; \
                 cqmask & ~cqslen; cqmask <<= 1) { \
                CIRCLEQ_MERGE_SUBL(cqtvar, cqidx, &cqhead, \
                                   cqmask, cqmask, cqmelm, cqmpos, \
                                   cmp, arg, field); \
            } \
        } \
        for (cqmelm = CIRCLEQ_FIRST(&cqhead, field), \
             cqmpos = 1, cqmask = 2; cqmask < cqslen; cqmask <<= 1) { \
            if (cqslen & cqmask) { \
                CIRCLEQ_MERGE_SUBL(cqtvar, cqidx, &cqhead, \
                                   cqslen & (cqmask - 1), cqmask, \
                                   cqmelm, cqmpos, cmp, arg, field); \
            } \
        } \
        CIRCLEQ_CONCAT(head, &cqhead, type, field); \
    } while (0)
