#include "tests/dryologging.h"
#include "dryox/dryologging.h"
#include "tests/REPORT.h"
#include <stdlib.h>

/*
 * What does this test do?
 *
 * TODO: Describe what this test does
 *
 */

void TEST_dryologging(void)
{
  REPORT(dryolog(LOG_DEBUG, "TEST_DEBUG"), "LOG_DEBUG");
  REPORT(dryolog(LOG_INFO, "TEST_INFO"), "LOG_INFO");
  REPORT(dryolog(LOG_WARN, "TEST_WARN"), "LOG_WARN");
  REPORT(dryolog(LOG_ERROR, "TEST_ERROR"), "LOG_ERROR");
  REPORT(dryolog(LOG_FATAL, "TEST_FATAL"), "LOG_FATAL");
  setenv("DRYOX_LITERAL_PATH", "tests/logging/test.log", 1);
  REPORT(dryolog(LOG_DEBUG, "TEST_DEBUG"), "LOG_DEBUG");
  REPORT(dryolog(LOG_INFO, "TEST_INFO"), "LOG_INFO");
  REPORT(dryolog(LOG_WARN, "TEST_WARN"), "LOG_WARN");
  REPORT(dryolog(LOG_ERROR, "TEST_ERROR"), "LOG_ERROR");
  REPORT(dryolog(LOG_FATAL, "TEST_FATAL"), "LOG_FATAL");
  unsetenv("DRYOX_LITERAL_PATH");
}