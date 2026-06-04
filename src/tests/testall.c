#include "utils/dryo_mkdirp.h"
#include "tests/dryo_mkdirp.h"
#include "dryox/dryoinit.h"
#include "tests/dryoinit.h"
#include "dryox/dryologging.h"
#include "tests/dryologging.h"
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
    /*

    test dryo_mkdirp(const char *dirpath);

    IS PART OF:
    utils

    REQUIRES:
    -

    ACCEPTS ENVIRONMENT:
    -

    */

    TEST_dryo_mkdirp();

    /*
    test dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir mode);

    IS PART OF:
    dryoinit

    REQUIRES:
    dryo_mkdirp FROM utils.c

    ACCEPTS ENVIRONMENT:
    DENV_OVERRIDE_(x);
    XDG_(x)_HOME;
    where (x) = DATA, STATE, CONFIG, CACHE.
    HOME;

    */

    TEST_dryoinit();

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
    TEST_dryologging();

    return 0;
}