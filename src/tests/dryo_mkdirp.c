#include "tests/dryo_mkdirp.h"
#include "utils/dryo_mkdirp.h"

void TEST_dryo_mkdirp(void)
{
    result = dryo_mkdirp("tests/dryo_mkdirp/a/b/c");
    if (result == 0)
        printf("dryo_mkdirp passed,\n");
    else
        printf("dryo_mkdirp failed,\n");
}

