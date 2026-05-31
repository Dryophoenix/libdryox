#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "dryox/utils.h"
#include "dryox/init.h"
#include "dryox/logging.h"

int main(void)
{
    /*
    test template()

    IS PART OF:
    -

    REQUIRES:
    -

    ACCEPTS ENVIRONMENT:
    -

    */

    int result;
    char teststr[PATH_MAX] = {0};
    // int testint = 0;
    // float testfloat = 0.0;

    /*
    test mkdirp(char *dirpath);

    IS PART OF:
    utils

    REQUIRES:
    -

    ACCEPTS ENVIRONMENT:
    -

    */

    result = mkdirp("tests/mkdirp/a/b/c");
    if (result == 0)
        printf("mkdirp succeeded,\n");
    else
        printf("mkdirp failed,\n");

    /*
    test dryoinit(char *logfile_out, char *filename, char *projectname);

    IS PART OF:
    init

    REQUIRES:
    mkdirp FROM utils.c

    ACCEPTS ENVIRONMENT:
    DRYOX_LOG_DIR;
    XDG_STATE_HOME;
    HOME;

    */

    result = dryoinit(teststr, "test.log", "TestProject");
    if (result == 0)
        printf("dryoinit succeeded,\n");
    else
        printf("dryoinit failed,\n");

    /*
    test dryoinit_absolute(char *logfile_out, char *rawpath);

    IS PART OF:
    init

    REQUIRES:
    mkdirp FROM utils.c

    ACCEPTS ENVIRONMENT:
    -

    */

    char LOG_FILE[PATH_MAX];
    result = dryoinit_absolute(LOG_FILE, "tests/logging/test.log");
    if (result == 0)
        printf("dryoinit_absolute succeeded,\n");
    else
    {
        snprintf(LOG_FILE, PATH_MAX, "tests/logging/test.log");
        printf("dryoinit_absolute failed,\n");
    }

    // This test provides LOG_FILE in any case, providing it for test dryolog.

    /*
    test dryolog(Log_Level level, char *format, ...);

    IS PART OF:
    logging

    REQUIRES:
    utils
    init

    ACCEPTS ENVIRONMENT:
    LOG_FILE

    */

    if (
        dryolog(LOG_DEBUG, "test Debug") == 0 &&
        dryolog(LOG_INFO, "test Info") == 0 &&
        dryolog(LOG_WARN, "test Warn") == 0 &&
        dryolog(LOG_ERROR, "test Error") == 0 &&
        dryolog(LOG_FATAL, "test Fatal") == 0)
        printf("dryolog probably succeeded,\n");
    else
        printf("dryolog probably failed, \n");

    return 0;
}
