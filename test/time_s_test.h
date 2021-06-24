
#include "utils/time_s.h"
#include <unity.h>

namespace time_s
{
    void test_get_set(void)
    {
        TimeS t;
        TEST_ASSERT_EQUAL(0, t.get_hour());
        TEST_ASSERT_EQUAL(0, t.get_minute());
        TEST_ASSERT_EQUAL(0, t.get_second());
        t.set_hour(2);
        t.set_minute(42);
        t.set_second(12);
        TEST_ASSERT_EQUAL(2, t.get_hour());
        TEST_ASSERT_EQUAL(42, t.get_minute());
        TEST_ASSERT_EQUAL(12, t.get_second());
    }

    void test_add_positif(void)
    {
        TimeS t;
        t.add_hms(18, 42, 36);
        TEST_ASSERT_EQUAL(18, t.get_hour());
        TEST_ASSERT_EQUAL(42, t.get_minute());
        TEST_ASSERT_EQUAL(36, t.get_second());
        t.add_hms(19, 43, 51);
        TEST_ASSERT_EQUAL(14, t.get_hour());
        TEST_ASSERT_EQUAL(26, t.get_minute());
        TEST_ASSERT_EQUAL(27, t.get_second());
        t.add_hms(0, 0, 43);
        TEST_ASSERT_EQUAL(14, t.get_hour());
        TEST_ASSERT_EQUAL(27, t.get_minute());
        TEST_ASSERT_EQUAL(10, t.get_second());
    }

    void test_add_negatif(void)
    {
        TimeS t(18, 42, 36);
        t.add_hms(-2, -3, -4);
        TEST_ASSERT_EQUAL(16, t.get_hour());
        TEST_ASSERT_EQUAL(39, t.get_minute());
        TEST_ASSERT_EQUAL(32, t.get_second());
        t.add_hms(-18, -43, -42);
        TEST_ASSERT_EQUAL(21, t.get_hour());
        TEST_ASSERT_EQUAL(55, t.get_minute());
        TEST_ASSERT_EQUAL(50, t.get_second());
        t.add_hms(0, 0, -59);
        TEST_ASSERT_EQUAL(22, t.get_hour());
        TEST_ASSERT_EQUAL(54, t.get_minute());
        TEST_ASSERT_EQUAL(51, t.get_second());
    }

}
