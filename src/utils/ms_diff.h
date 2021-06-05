#pragma once

// Return difference in ms between 2 times, taking in account negative values
// Max diff is about 60s
unsigned long ms_diff(long start_time, long now);
