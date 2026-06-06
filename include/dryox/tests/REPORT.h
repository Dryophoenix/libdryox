#ifndef DRYOTU_REPORT_H
#define DRYOTU_REPORT_H
#include <stdio.h>

#define REPORT(result, msg)                                                                                            \
  ((result) == 0 ? printf("%s: %s succeeded.\n", __FILE__, msg) : printf("%s: %s failed.\n", __FILE__, msg))

#endif