#include <stdio.h>
#include "dryox/utils.h"
#include "dryox/init.h"

int main(void)
{
    const char *path = "test_out/a/b/c";
    int result = mkdirp((char *)path);
    if (result == 0)
        printf("mkdirp(\"%s\") succeeded\n", path);
    else
        printf("mkdirp(\"%s\") failed\n", path);

    result = dryoinit("test.log", "TestProject");
    if (result == 0)
        printf("dryoinit succeeded\n");
    else
        printf("dryoinit failed\n");

    return result;
}
