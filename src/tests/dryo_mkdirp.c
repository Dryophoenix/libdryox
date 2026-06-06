#include "tests/dryo_mkdirp.h"
#include "tests/REPORT.h"
#include "utils/dryo_mkdirp.h"
#include <stdio.h>

void TEST_dryo_mkdirp(void) { REPORT(dryo_mkdirp("tests/dryo_mkdirp/a/b/c"), "mkdirp"); }
