#pragma once

#include <sys/time.h>

double get_dt(const struct timeval &before, const struct timeval &after);
