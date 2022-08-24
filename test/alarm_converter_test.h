#include "js_alarm_converter.h"
#include "time.h"
#include "js_asserts.h"
#include <unity.h>
#include <iostream>
#include <array>

namespace alarm_converter
{
    void test_small()
    {
        Time src0(1, 2);
        Time src1(3, 4);
        Time src2(5, 6);
        Time src3(7, 8);
        Time src4(9, 10);
        Time src5(11, 12);
        uint8_t src_flag = 0;
        Time dst0;
        Time dst1;
        Time dst2;
        Time dst3;
        Time dst4;
        Time dst5;
        uint8_t dst_flag;

        uint64_t value = serialize_alarms(src0, src1, src2, src3, src4, src5, src_flag);
        deserialize_alarm(value, dst0, dst1, dst2, dst3, dst4, dst5, dst_flag);

        TEST_ASSERT_TIME_EQUAL(src0, dst0);
        TEST_ASSERT_TIME_EQUAL(src1, dst1);
        TEST_ASSERT_TIME_EQUAL(src2, dst2);
        TEST_ASSERT_TIME_EQUAL(src3, dst3);
        TEST_ASSERT_TIME_EQUAL(src4, dst4);
        //TEST_ASSERT_TIME_EQUAL(src5, dst5);  // Can save only 5 alarms with 7 bytes
        TEST_ASSERT_EQUAL(0, dst_flag);
    }

    void test_big()
    {
        Time src0(18, 54);
        Time src1(19, 55);
        Time src2(20, 56);
        Time src3(21, 57);
        Time src4(22, 58);
        Time src5(23, 59);
        uint8_t src_flag = 7;
        Time dst0;
        Time dst1;
        Time dst2;
        Time dst3;
        Time dst4;
        Time dst5;
        uint8_t dst_flag;

        uint64_t value = serialize_alarms(src0, src1, src2, src3, src4, src5, src_flag);
        deserialize_alarm(value, dst0, dst1, dst2, dst3, dst4, dst5, dst_flag);

        TEST_ASSERT_TIME_EQUAL(src0, dst0);
        TEST_ASSERT_TIME_EQUAL(src1, dst1);
        TEST_ASSERT_TIME_EQUAL(src2, dst2);
        TEST_ASSERT_TIME_EQUAL(src3, dst3);
        TEST_ASSERT_TIME_EQUAL(src4, dst4);
        //TEST_ASSERT_TIME_EQUAL(src5, dst5); // Can save only 5 alarms with 7 bytes
        TEST_ASSERT_EQUAL(src_flag, dst_flag);
    }

    class MockWire
    {
    public:
        std::array<uint8_t, 12> data;
        int _cursor;
        MockWire(): _cursor(0) { data.fill(0xAA); }

        void beginTransmission(uint8_t) { _cursor = 0; }
        uint8_t endTransmission(void) { return 0;  }
        uint8_t requestFrom(uint8_t, uint8_t size)
        {
            return size;
        }
        size_t write(uint8_t value)
        {
            if(_cursor == 0)
            {
                _cursor = value;
            }
            else
            {
                data[_cursor++] = value;
            }
            return 0;
        }
        int read(void)
        {
            return data[_cursor++];
        }
    };

    typedef AlarmStore<MockWire, 0, 1, 4, 6, 3> MockAlarmStore;

    void test_save_alarm()
    {
        // Test save & read in 7 8 bits registers
        Time src0(18, 54);
        Time src1(19, 55);
        Time src2(20, 56);
        Time src3(21, 57);
        Time src4(22, 58);
        Time src5(23, 59);
        uint8_t src_flag = 7;
        MockWire wire;
        MockAlarmStore store(wire);
        store.save_alarm(src0, src1, src2, src3, src4, src5, src_flag);

        Time dst0;
        Time dst1;
        Time dst2;
        Time dst3;
        Time dst4;
        Time dst5;
        uint8_t dst_flag;
        store.read_alarm(dst0, dst1, dst2, dst3, dst4, dst5, dst_flag);

        TEST_ASSERT_TIME_EQUAL(src0, dst0);
        TEST_ASSERT_TIME_EQUAL(src1, dst1);
        TEST_ASSERT_TIME_EQUAL(src2, dst2);
        TEST_ASSERT_TIME_EQUAL(src3, dst3);
        TEST_ASSERT_TIME_EQUAL(src4, dst4);
        //TEST_ASSERT_TIME_EQUAL(src5, dst5); // Can save only 5 alarms with 7 bytes
        TEST_ASSERT_EQUAL(src_flag, dst_flag);
    }
}
