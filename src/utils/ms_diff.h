#pragma once

// Return difference in ms between 2 times, taking in account long overflow
// Max diff is about 60s
unsigned long ms_diff(unsigned long start_time, unsigned long now);
