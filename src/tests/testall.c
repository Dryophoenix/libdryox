#include "utils/dryo_mkdirp.h"
#include "tests/dryo_mkdirp.h"
#include "dryox/init.h"
#include "dryox/logging.h"

/*
testall.c is a libdryox helper.

This source serves mostly as documentation, instead relying on each function's
    specific test being included.

This serves so that each test may be done independently.

TODO: Write a function into makefile so that test (some program) tests (some
    program).

It serves the following functions:
    -

It defines the following types:
    -

*/

/*
test template()

IS PART OF:
-

REQUIRES:
-

ACCEPTS ENVIRONMENT:
-

*/

int main(void)
{


    int result;
    char teststr[PATH_MAX] = {0};
    // int testint = 0;
    // float testfloat = 0.0;

    /*

    test dryo_mkdirp(char *dirpath);

    IS PART OF:
    utils

    REQUIRES:
    -

    ACCEPTS ENVIRONMENT:
    -

    */

    TEST_dryo_mkdirp(void);

    /*
    for dryoinit 0.2.0
    test dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir mode);

    IS PART OF:
    init

    REQUIRES:
    dryo_mkdirp FROM utils.c

    ACCEPTS ENVIRONMENT:
    DENV_OVERRIDE_(x);
    XDG_(x)_HOME;
    where (x) = DATA, STATE, CONFIG, CACHE.
    HOME;
    */

    // Make sure dryoinit works.
    // Once you're sure dryoinit works, make sure the env works.

    result = dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE);
    if (result == 0)
        printf("dryoinit passed,\n");
    else
        printf("dryoinit failed,\n");

    setenv("DENV_OVERRIDE_STATE", "tests/init/OVERRIDE/state", 1);
    result = dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE);
    if (result == 0)
        printf("dryoinit OVERRIDE passed,\n");
    else
        printf("dryoinit OVERRIDE failed,\n");
    unsetenv("DENV_OVERRIDE_STATE");

    setenv("XDG_STATE_HOME", "tests/init/XDG/state", 1);
    result = dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE);
    if (result == 0)
        printf("dryoinit XDG passed,\n");
    else
        printf("dryoinit XDG failed,\n");
    unsetenv("XDG_STATE_HOME");

    char HOME_BUF[PATH_MAX];
    const char *HOME_ORIG = getenv("HOME");
    if (HOME_ORIG != NULL)
        snprintf(HOME_BUF, PATH_MAX, "%s", HOME_ORIG);

    setenv("HOME", "tests/init/HOME/state", 1);
    result = dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE);
    if (result == 0)
        printf("dryoinit HOME passed,\n");
    else
        printf("dryoinit HOME failed,\n");

    if (HOME_ORIG != NULL)
        setenv("HOME", HOME_BUF, 1);
    else
        unsetenv("HOME");

    /*
    test dryoinit with DRYOX_LITERAL mode

    IS PART OF:
    init

    REQUIRES:
    dryo_mkdirp FROM utils.c

    ACCEPTS ENVIRONMENT:
    -

    */

    char LOG_FILE[PATH_MAX];
    result = dryoinit(LOG_FILE, NULL, NULL, DRYOX_LITERAL, "tests/logging/test.log");
    if (result == 0)
        printf("dryoinit (DRYOX_LITERAL) passed,\n");
    else
    {
        snprintf(LOG_FILE, PATH_MAX, "tests/logging/test.log");
        printf("dryoinit (DRYOX_LITERAL) failed,\n");
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
        printf("dryolog probably passed,\n");
    else
        printf("dryolog probably failed, \n");

    return 0;
}