#include "tests/dryo_mkdirp.h"
#include "utils/dryo_mkdirp.h"
#include <stdio.h>

void TEST_dryo_mkdirp(void)
{
    int result;
    result = dryou_mkdirp("tests/dryo_mkdirp/a/b/c");
    if (result == 0)
        printf("dryo_mkdirp passed,\n");
    else
        printf("dryo_mkdirp failed,\n");
}

