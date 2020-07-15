#include "util.hpp"

double get_dt(const struct timeval &before, const struct timeval &after)
{
  int dt_s = after.tv_sec - before.tv_sec;
  int dt_ns = after.tv_usec - before.tv_usec;

  double rv = double(dt_s) + 1e-6*double(dt_ns);
  return rv;
}
