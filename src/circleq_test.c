#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "circleq.h"

#define TEST(func) static void func(const char *__unit)

// GCOVR_EXCL_START
static void assert_expr(const char *unit, bool result,
                        const char *const expression,
                        const char *const file, const int line)
{
    if (result)
        return;

    fprintf(stderr, "%s:%d: unit test '%s' assertion failed: (%s)\n",
            file, line, unit, expression);
    fflush(stderr);
    abort();
}
// GCOVR_EXCL_STOP

#define assert_true(x) assert_expr(__unit, !!(x), #x, __FILE__, __LINE__)
#define assert_false(x) assert_true(!(x))

#define assert_null(x) assert_true((x) == NULL)
#define assert_not_null(x) assert_true((x) != NULL)

#define assert_equal(a, b) assert_true((a) == (b))
#define assert_not_equal(a, b) assert_true((a) != (b))

#define assert_str_equal(a, b) assert_true(strcmp(a, b) == 0)
#define assert_str_not_equal(a, b) assert_true(strcmp(a, b) != 0)

#define assert_strn_equal(a, b, n) assert_true(strncmp(a, b, n) == 0)
#define assert_strn_not_equal(a, b, n) assert_true(strncmp(a, b, n) != 0)

#define assert_mem_equal(a, b, n) assert_true(memcmp(a, b, n) == 0)
#define assert_mem_not_equal(a, b, n) assert_true(memcmp(a, b, n) != 0)

struct circleq_entry {
    CIRCLEQ_ENTRY(circleq_entry) entry;
    int value;
};

CIRCLEQ_HEAD(circleq_list, circleq_entry);

TEST(test_circleq_init)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    assert_true(CIRCLEQ_EMPTY(&list));
    assert_null(CIRCLEQ_LAST(&list));
    assert_null(CIRCLEQ_FIRST(&list, entry));

    CIRCLEQ_INIT(&list);
    assert_true(CIRCLEQ_EMPTY(&list));
    assert_null(CIRCLEQ_LAST(&list));
    assert_null(CIRCLEQ_FIRST(&list, entry));
}

TEST(test_circleq_insert_head)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry, e1, e2;

    entry = &e1;
    entry->value = 123;

    CIRCLEQ_INSERT_HEAD(&list, entry, entry);
    assert_false(CIRCLEQ_EMPTY(&list));
    assert_equal(entry->value, 123);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        assert_not_null(head);
        assert_equal(head, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        assert_not_null(tail);
        assert_equal(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        assert_not_null(next);
        assert_equal(next, entry);
    }

    entry = &e2;
    entry->value = 456;

    CIRCLEQ_INSERT_HEAD(&list, entry, entry);
    assert_false(CIRCLEQ_EMPTY(&list));
    assert_equal(entry->value, 456);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        assert_not_null(head);
        assert_equal(head, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        assert_not_null(next);
        assert_not_equal(next, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        assert_not_null(tail);
        assert_equal(tail, next);
    }

    CIRCLEQ_INIT(&list);
    assert_true(CIRCLEQ_EMPTY(&list));
    assert_null(CIRCLEQ_LAST(&list));
    assert_null(CIRCLEQ_FIRST(&list, entry));
}

TEST(test_circleq_insert_tail)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry, e1, e2;

    entry = &e1;
    entry->value = 123;

    CIRCLEQ_INSERT_TAIL(&list, entry, entry);
    assert_false(CIRCLEQ_EMPTY(&list));
    assert_equal(entry->value, 123);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        assert_not_null(head);
        assert_equal(head, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        assert_not_null(tail);
        assert_equal(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        assert_not_null(next);
        assert_equal(next, entry);
    }

    entry = &e2;
    entry->value = 456;

    CIRCLEQ_INSERT_TAIL(&list, entry, entry);
    assert_false(CIRCLEQ_EMPTY(&list));
    assert_equal(entry->value, 456);

    {
        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        assert_not_null(tail);
        assert_equal(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        assert_not_null(next);
        assert_not_equal(next, entry);

        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        assert_not_null(head);
        assert_equal(head, next);
    }

    CIRCLEQ_INIT(&list);
    assert_true(CIRCLEQ_EMPTY(&list));
    assert_null(CIRCLEQ_LAST(&list));
    assert_null(CIRCLEQ_FIRST(&list, entry));
}

TEST(test_circleq_remove_head)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry;
    struct circleq_entry entries[5];

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        entry = &entries[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_TAIL(&list, entry, entry);

        size_t j = 0;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((entry = CIRCLEQ_LAST(&list))) {
        CIRCLEQ_REMOVE_TAIL(&list, circleq_entry, entry);

        size_t j = 0;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, values[j++]);
        }

        ++i;
    }

    assert_equal(i, 5);
}

TEST(test_circleq_remove_tail)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry;
    struct circleq_entry entries[5];

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        entry = &entries[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_HEAD(&list, entry, entry);

        size_t j = 0;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, values[i - j++]);
        }
    }

    size_t i = 0;
    while ((entry = CIRCLEQ_FIRST(&list, entry))) {
        CIRCLEQ_REMOVE_HEAD(&list, entry);

        size_t j = ++i;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, values[5 - ++j]);
        }
    }

    assert_equal(i, 5);
}

TEST(test_circleq_concat)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry;
    struct circleq_entry entries1[5], entries2[5];
    struct circleq_list temp = CIRCLEQ_HEAD_INITIALIZER(temp);

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        entry = &entries1[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_TAIL(&list, entry, entry);

        entry = &entries2[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_TAIL(&temp, entry, entry);
    }

    CIRCLEQ_CONCAT(&list, &temp, circleq_entry, entry);
    assert_true(CIRCLEQ_EMPTY(&temp));
    assert_null(CIRCLEQ_LAST(&temp));
    assert_null(CIRCLEQ_FIRST(&temp, entry));

    size_t i = 0;
    CIRCLEQ_FOREACH(entry, &list, entry) {
        assert_not_null(entry);
        assert_equal(entry->value, values[i++ % 5]);
    }

    assert_equal(i, 10);
}

TEST(test_circleq_swap)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry;
    struct circleq_entry entries1[5], entries2[5];
    struct circleq_list temp = CIRCLEQ_HEAD_INITIALIZER(temp);

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        entry = &entries1[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_TAIL(&list, entry, entry);

        entry = &entries2[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_HEAD(&temp, entry, entry);
    }

    CIRCLEQ_SWAP(&list, &temp, circleq_entry, entry);

    size_t i = 0;
    CIRCLEQ_FOREACH(entry, &temp, entry) {
        assert_not_null(entry);
        assert_equal(entry->value, values[i++ % 5]);
    }

    assert_equal(i, 5);
}

static inline int circleq_entry_cmp(struct circleq_entry *e1,
                                    struct circleq_entry *e2, void *arg)
{
    const char *__unit = (const char *)arg;
    assert_not_null(e1);
    assert_not_null(e2);
    return (e1->value > e2->value) - (e1->value < e2->value);
}

TEST(test_circleq_merge)
{
    struct circleq_list list1 = CIRCLEQ_HEAD_INITIALIZER(list1);
    struct circleq_list list2 = CIRCLEQ_HEAD_INITIALIZER(list2);
    struct circleq_list list3 = CIRCLEQ_HEAD_INITIALIZER(list3);

    CIRCLEQ_MERGE(&list1, &list2, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);
    CIRCLEQ_MERGE(&list1, &list3, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);
    assert_true(CIRCLEQ_EMPTY(&list1));
    assert_true(CIRCLEQ_EMPTY(&list2));
    assert_true(CIRCLEQ_EMPTY(&list3));

    struct circleq_entry *entry;
    struct circleq_entry entries1[5];
    struct circleq_entry entries2[5];
    struct circleq_entry entries3[5];

    int values1[] = { 5, 6, 7, 8, 9 };
    int values2[] = { 1, 2, 3, 4, 5 };
    int values3[] = { 3, 4, 5, 6, 7 };
    int results[] = { 1, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 0 };

    for (size_t i = 0; i < 5; ++i) {
        entry = &entries1[i];
        entry->value = values1[i];
        CIRCLEQ_INSERT_TAIL(&list1, entry, entry);

        entry = &entries2[i];
        entry->value = values2[i];
        CIRCLEQ_INSERT_TAIL(&list2, entry, entry);

        entry = &entries3[i];
        entry->value = values3[i];
        CIRCLEQ_INSERT_TAIL(&list3, entry, entry);
    }

    for (size_t j = 0; j < 5; ++j) {
        CIRCLEQ_MERGE(&list2, &list3, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);
        assert_true(CIRCLEQ_EMPTY(&list3));

        CIRCLEQ_MERGE(&list1, &list2, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);
        assert_true(CIRCLEQ_EMPTY(&list2));

        size_t i = 0;
        CIRCLEQ_FOREACH(entry, &list1, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, results[i++ % 16]);
        }

        assert_equal(i, 15);
    }
}

TEST(test_circleq_mergesort)
{
    int values[] = { 5, 3, 1, 7, 7, 3, 9, 2, 5, 6, 4, 8, 4, 6, 5, 0 };
    int result[] = { 1, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 0 };

    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry;
    struct circleq_entry entries[15];

    CIRCLEQ_MERGESORT(&list, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);
    assert_true(CIRCLEQ_EMPTY(&list));

    for (size_t i = 0; i < 15; ++i) {
        entry = &entries[i];
        entry->value = values[i];
        CIRCLEQ_INSERT_TAIL(&list, entry, entry);
    }

    for (size_t j = 0; j < 5; ++j) {
        CIRCLEQ_MERGESORT(&list, circleq_entry_cmp, (void *)__unit, circleq_entry, entry);

        size_t i = 0;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            assert_not_null(entry);
            assert_equal(entry->value, result[i++ % 16]);
        }

        assert_equal(i, 15);
    }
}

int main(void)
{
    struct {
        const char *name;
        void (*func)(const char *__unit);
    } tests[] = {
        { "circleq_init", test_circleq_init },
        { "circleq_insert_head", test_circleq_insert_head },
        { "circleq_insert_tail", test_circleq_insert_tail },
        { "circleq_remove_head", test_circleq_remove_head },
        { "circleq_remove_tail", test_circleq_remove_tail },
        { "circleq_concat", test_circleq_concat },
        { "circleq_swap", test_circleq_swap },
        { "circleq_merge", test_circleq_merge },
        { "circleq_mergesort", test_circleq_mergesort },
    };

    for (size_t i = 0, n = sizeof(tests) / sizeof(tests[0]); i < n; ++i) {
        printf(">>> Testing (%zu of %zu) %s...\n", i + 1, n, tests[i].name);
        tests[i].func(tests[i].name);
    }

    return EXIT_SUCCESS;
}
