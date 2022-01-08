#pragma once

#define TEST_ASSERT_TIME_EQUAL(a, b) \
UNITY_TEST_ASSERT_EQUAL_UINT8(a.get_hour(), b.get_hour(), __LINE__, "Hour comparison"); \
UNITY_TEST_ASSERT_EQUAL_UINT8(a.get_minute(), b.get_minute(), __LINE__, "Minute comparison")
