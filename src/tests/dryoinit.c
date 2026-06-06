#include "tests/dryoinit.h"
#include "dryox/dryoinit.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Eisenhower 2 Refactor

/*
 * What does this test do?
 *
 * TODO: Describe what this test does
 *
 */

void TEST_dryoinit(void)
{
  int result;
  char teststr[PATH_MAX];

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

  char LOG_FILE[PATH_MAX];
  result = dryoinit(LOG_FILE, NULL, NULL, DRYOX_LITERAL, "tests/logging/test.log");
  if (result == 0)
    printf("dryoinit (DRYOX_LITERAL) passed,\n");
  else
  {
    snprintf(LOG_FILE, PATH_MAX, "tests/logging/test.log");
    printf("dryoinit (DRYOX_LITERAL) failed,\n");
  }
}