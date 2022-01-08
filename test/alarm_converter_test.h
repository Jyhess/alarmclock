#include "js_alarm_converter.h"
#include "time.h"
#include "js_asserts.h"
#include <unity.h>
#include <iostream>

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
        uint8_t src_flag = 5;
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
        TEST_ASSERT_EQUAL(1, dst_flag);
    }

    void test_save_alarm()
    {
        // Test save & read in 7 8 bits registers
        Time src0(18, 54);
        Time src1(19, 55);
        Time src2(20, 56);
        Time src3(21, 57);
        Time src4(22, 58);
        Time src5(23, 59);
        uint8_t src_flag = 3;
        uint64_t value = serialize_alarms(src0, src1, src2, src3, src4, src5, src_flag);

        uint8_t reg_alarms[7];
        // Alarm 1
        reg_alarms[0] = static_cast<uint8_t>(value);
        reg_alarms[1] = static_cast<uint8_t>(value >> 8);
        reg_alarms[2] = static_cast<uint8_t>(value >> 16);
        reg_alarms[3] = static_cast<uint8_t>(value >> 24);

        // Alarm 2
        reg_alarms[4] = static_cast<uint8_t>(value >> 32);
        reg_alarms[5] = static_cast<uint8_t>(value >> 40);
        reg_alarms[6] = static_cast<uint8_t>(value >> 48);

        uint64_t read = static_cast<uint64_t>(reg_alarms[0])
          + (static_cast<uint64_t>(reg_alarms[1]) << 8) 
          + (static_cast<uint64_t>(reg_alarms[2]) << 16)
          + (static_cast<uint64_t>(reg_alarms[3]) << 24)
          ;

        read += (static_cast<uint64_t>(reg_alarms[4]) << 32) +
            (static_cast<uint64_t>(reg_alarms[5]) << 40) +
            (static_cast<uint64_t>(reg_alarms[6]) << 48)
            ;


        Time dst0;
        Time dst1;
        Time dst2;
        Time dst3;
        Time dst4;
        Time dst5;
        uint8_t dst_flag;
        deserialize_alarm(read, dst0, dst1, dst2, dst3, dst4, dst5, dst_flag);
/*
        std::cout << "value=" << std::hex << value << std::endl;
        for (int i = 0; i < 8; ++i) {
          std::cout << "reg_alarm[" << i << "]=" << std::hex << (int)reg_alarms[i] << std::endl;
        }
        std::cout << "read=" << std::hex << read << std::endl;
*/
        TEST_ASSERT_TIME_EQUAL(src0, dst0);
        TEST_ASSERT_TIME_EQUAL(src1, dst1);
        TEST_ASSERT_TIME_EQUAL(src2, dst2);
        TEST_ASSERT_TIME_EQUAL(src3, dst3);
        TEST_ASSERT_TIME_EQUAL(src4, dst4);
        //TEST_ASSERT_TIME_EQUAL(src5, dst5); // Can save only 5 alarms with 7 bytes
        TEST_ASSERT_EQUAL(1, dst_flag);
    }
}
