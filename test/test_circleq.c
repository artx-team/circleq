#include <CUnit/Basic.h>

#include "circleq.h"

struct circleq_entry {
    CIRCLEQ_ENTRY(circleq_entry) entry;
    int value;
};

CIRCLEQ_HEAD(circleq_list, circleq_entry);

static void test_circleq_init(void)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    CU_ASSERT_TRUE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_LAST(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_FIRST(&list, entry));

    CIRCLEQ_INIT(&list);
    CU_ASSERT_TRUE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_LAST(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_FIRST(&list, entry));
}

static void test_circleq_insert_head(void)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry, e1, e2;

    entry = &e1;
    entry->value = 123;

    CIRCLEQ_INSERT_HEAD(&list, entry, entry);
    CU_ASSERT_FALSE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_EQUAL(entry->value, 123);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        CU_ASSERT_PTR_NOT_NULL(next);
        CU_ASSERT_PTR_EQUAL(next, entry);
    }

    entry = &e2;
    entry->value = 456;

    CIRCLEQ_INSERT_HEAD(&list, entry, entry);
    CU_ASSERT_FALSE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_EQUAL(entry->value, 456);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        CU_ASSERT_PTR_NOT_NULL(next);
        CU_ASSERT_PTR_NOT_EQUAL(next, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, next);
    }

    CIRCLEQ_INIT(&list);
    CU_ASSERT_TRUE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_LAST(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_FIRST(&list, entry));
}

static void test_circleq_insert_tail(void)
{
    struct circleq_list list = CIRCLEQ_HEAD_INITIALIZER(list);
    struct circleq_entry *entry, e1, e2;

    entry = &e1;
    entry->value = 123;

    CIRCLEQ_INSERT_TAIL(&list, entry, entry);
    CU_ASSERT_FALSE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_EQUAL(entry->value, 123);

    {
        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, entry);

        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        CU_ASSERT_PTR_NOT_NULL(next);
        CU_ASSERT_PTR_EQUAL(next, entry);
    }

    entry = &e2;
    entry->value = 456;

    CIRCLEQ_INSERT_TAIL(&list, entry, entry);
    CU_ASSERT_FALSE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_EQUAL(entry->value, 456);

    {
        struct circleq_entry *tail = CIRCLEQ_LAST(&list);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, entry);

        struct circleq_entry *next = CIRCLEQ_NEXT(entry, entry);
        CU_ASSERT_PTR_NOT_NULL(next);
        CU_ASSERT_PTR_NOT_EQUAL(next, entry);

        struct circleq_entry *head = CIRCLEQ_FIRST(&list, entry);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, next);
    }

    CIRCLEQ_INIT(&list);
    CU_ASSERT_TRUE(CIRCLEQ_EMPTY(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_LAST(&list));
    CU_ASSERT_PTR_NULL(CIRCLEQ_FIRST(&list, entry));
}

static void test_circleq_remove_head(void)
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
            CU_ASSERT_PTR_NOT_NULL(entry);
            CU_ASSERT_EQUAL(entry->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((entry = CIRCLEQ_LAST(&list))) {
        CIRCLEQ_REMOVE_TAIL(&list, circleq_entry, entry);

        size_t j = 0;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            CU_ASSERT_PTR_NOT_NULL(entry);
            CU_ASSERT_EQUAL(entry->value, values[j++]);
        }

        ++i;
    }

    CU_ASSERT_EQUAL(i, 5);
}

static void test_circleq_remove_tail(void)
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
            CU_ASSERT_PTR_NOT_NULL(entry);
            CU_ASSERT_EQUAL(entry->value, values[i - j++]);
        }
    }

    size_t i = 0;
    while ((entry = CIRCLEQ_FIRST(&list, entry))) {
        CIRCLEQ_REMOVE_HEAD(&list, entry);

        size_t j = ++i;
        CIRCLEQ_FOREACH(entry, &list, entry) {
            CU_ASSERT_PTR_NOT_NULL(entry);
            CU_ASSERT_EQUAL(entry->value, values[5 - ++j]);
        }
    }

    CU_ASSERT_EQUAL(i, 5);
}

static void test_circleq_concat(void)
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
    CU_ASSERT_TRUE(CIRCLEQ_EMPTY(&temp));
    CU_ASSERT_PTR_NULL(CIRCLEQ_LAST(&temp));
    CU_ASSERT_PTR_NULL(CIRCLEQ_FIRST(&temp, entry));

    size_t i = 0;
    CIRCLEQ_FOREACH(entry, &list, entry) {
        CU_ASSERT_PTR_NOT_NULL(entry);
        CU_ASSERT_EQUAL(entry->value, values[i++ % 5]);
    }

    CU_ASSERT_EQUAL(i, 10);
}

static void test_circleq_swap(void)
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
        CU_ASSERT_PTR_NOT_NULL(entry);
        CU_ASSERT_EQUAL(entry->value, values[i++ % 5]);
    }

    CU_ASSERT_EQUAL(i, 5);
}

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return (int)CU_get_error();

    CU_pSuite pSuite = CU_add_suite("circleq", NULL, NULL);
    if (!pSuite)
        goto error;

    if (!CU_ADD_TEST(pSuite, test_circleq_init) ||
        !CU_ADD_TEST(pSuite, test_circleq_insert_head) ||
        !CU_ADD_TEST(pSuite, test_circleq_insert_tail) ||
        !CU_ADD_TEST(pSuite, test_circleq_remove_head) ||
        !CU_ADD_TEST(pSuite, test_circleq_remove_tail) ||
        !CU_ADD_TEST(pSuite, test_circleq_concat) ||
        !CU_ADD_TEST(pSuite, test_circleq_swap)) {
        goto error;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int num = CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    if (CU_get_error() == CUE_SUCCESS) {
        return (int)num;
    } else {
        printf("CUnit Error: %s\n", CU_get_error_msg());
        return (int)CU_get_error();
    }

error:
    CU_cleanup_registry();
    return (int)CU_get_error();
}
