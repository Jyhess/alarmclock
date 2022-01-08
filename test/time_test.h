
#include "js_time.h"
#include <unity.h>

namespace time_hm
{

    void test_get_set(void)
    {
        Time t;
        TEST_ASSERT_EQUAL(0, t.get_hour());
        TEST_ASSERT_EQUAL(0, t.get_minute());
        t.set_hour(2);
        t.set_minute(42);
        TEST_ASSERT_EQUAL(2, t.get_hour());
        TEST_ASSERT_EQUAL(42, t.get_minute());
    }

    void test_add_positif(void)
    {
        Time t;
        t.add_hour(18);
        t.add_minute(42);
        TEST_ASSERT_EQUAL(18, t.get_hour());
        TEST_ASSERT_EQUAL(42, t.get_minute());
        t.add_hour(19);
        t.add_minute(43);
        TEST_ASSERT_EQUAL(13, t.get_hour());
        TEST_ASSERT_EQUAL(25, t.get_minute());
        t.add_minute(43, true);
        TEST_ASSERT_EQUAL(8, t.get_minute());
        TEST_ASSERT_EQUAL(14, t.get_hour());
    }

    void test_add_negatif(void)
    {
        Time t(18, 42);
        t.add_hour(-2);
        t.add_minute(-3);
        TEST_ASSERT_EQUAL(16, t.get_hour());
        TEST_ASSERT_EQUAL(39, t.get_minute());
        t.add_hour(-18);
        t.add_minute(-43);
        TEST_ASSERT_EQUAL(22, t.get_hour());
        TEST_ASSERT_EQUAL(56, t.get_minute());
        t.add_minute(-59, true);
        TEST_ASSERT_EQUAL(21, t.get_hour());
        TEST_ASSERT_EQUAL(57, t.get_minute());
    }

    void test_to_minutes()
    {
        Time t(18, 42);
        TEST_ASSERT_EQUAL(18*60 + 42, t.to_minutes());
    }

    void test_from_minutes()
    {
        {
            Time t = Time::from_minutes(18*60 + 42);
            TEST_ASSERT_EQUAL(18, t.get_hour());
            TEST_ASSERT_EQUAL(42, t.get_minute());
        }
        {
            Time t = Time::from_minutes(25*60 + 42);
            TEST_ASSERT_EQUAL(1, t.get_hour());
            TEST_ASSERT_EQUAL(42, t.get_minute());
        }
    }

    void test_from_seconds()
    {
        {
            Time t = Time::from_seconds(120);
            TEST_ASSERT_EQUAL(0, t.get_hour());
            TEST_ASSERT_EQUAL(2, t.get_minute());
        }

        {
            Time t = Time::from_seconds(3600);
            TEST_ASSERT_EQUAL(1, t.get_hour());
            TEST_ASSERT_EQUAL(0, t.get_minute());
        }

        {
            uint32_t seconds = (18*60 + 42)*60 + 21;
            TEST_ASSERT_EQUAL_UINT32(67341, seconds);
            Time t = Time::from_seconds(seconds);
            TEST_ASSERT_EQUAL(18, t.get_hour());
            TEST_ASSERT_EQUAL(42, t.get_minute());
        }
    }
}
