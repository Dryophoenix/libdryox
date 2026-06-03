#include "tests/dryologging.h"
#include "dryox/dryologging.h"

// TODO: Eisenhower 3 Rewrite (comprehensiveness)

/*
 * What does this test do?
 *
 * TODO: Describe what this test does
 *
 */

void TEST_dryologging(void)
{
    if (
        dryolog(LOG_DEBUG, "test Debug") == 0 &&
        dryolog(LOG_INFO, "test Info") == 0 &&
        dryolog(LOG_WARN, "test Warn") == 0 &&
        dryolog(LOG_ERROR, "test Error") == 0 &&
        dryolog(LOG_FATAL, "test Fatal") == 0)
        printf("dryolog probably passed,\n");
    else
        printf("dryolog probably failed, \n");
}