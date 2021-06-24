#include "time_test.h"
#include "time_s_test.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(time::test_get_set);
    RUN_TEST(time::test_add_positif);
    RUN_TEST(time::test_add_negatif);
    RUN_TEST(time_s::test_get_set);
    RUN_TEST(time_s::test_add_positif);
    RUN_TEST(time_s::test_add_negatif);
    UNITY_END();

    return 0;
}
