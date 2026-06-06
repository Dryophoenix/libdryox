#include "tests/dryoinit.h"
#include "dryox/dryoinit.h"
#include "tests/ENVUTILS.h"
#include "tests/REPORT.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * What does this test do?
 *
 * TODO: Describe what this test does
 *
 */

void TEST_dryoinit(void)
{
  char *overridebuf;

  // STATE
  {
    char teststr[PATH_MAX];
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE), "state");
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("DENV_OVERRIDE_STATE", overridebuf, "tests/init/OVERRIDE/state");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE), "state OVERRIDE");
    RESTOREENV("DENV_OVERRIDE_STATE", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("XDG_STATE_HOME", overridebuf, "tests/init/XDG/state");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE), "state XDG");
    RESTOREENV("XDG_STATE_HOME", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("HOME", overridebuf, "tests/init/HOME/state");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_STATE), "state HOME");
    RESTOREENV("HOME", overridebuf);
  }

  // CONFIG
  {
    char teststr[PATH_MAX];
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CONFIG), "config");
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("DENV_OVERRIDE_CONFIG", overridebuf, "tests/init/OVERRIDE/config");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CONFIG), "config OVERRIDE");
    RESTOREENV("DENV_OVERRIDE_CONFIG", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("XDG_CONFIG_HOME", overridebuf, "tests/init/XDG/config");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CONFIG), "config XDG");
    RESTOREENV("XDG_CONFIG_HOME", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("HOME", overridebuf, "tests/init/HOME/config");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CONFIG), "config HOME");
    RESTOREENV("HOME", overridebuf);
  }

  // DATA
  {
    char teststr[PATH_MAX];
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_DATA), "data");
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("DENV_OVERRIDE_DATA", overridebuf, "tests/init/OVERRIDE/data");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_DATA), "data OVERRIDE");
    RESTOREENV("DENV_OVERRIDE_DATA", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("XDG_DATA_HOME", overridebuf, "tests/init/XDG/data");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_DATA), "data XDG");
    RESTOREENV("XDG_DATA_HOME", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("HOME", overridebuf, "tests/init/HOME/data");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_DATA), "data HOME");
    RESTOREENV("HOME", overridebuf);
  }

  // CACHE
  {
    char teststr[PATH_MAX];
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CACHE), "cache");
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("DENV_OVERRIDE_CACHE", overridebuf, "tests/init/OVERRIDE/cache");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CACHE), "cache OVERRIDE");
    RESTOREENV("DENV_OVERRIDE_CACHE", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("XDG_CACHE_HOME", overridebuf, "tests/init/XDG/cache");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CACHE), "cache XDG");
    RESTOREENV("XDG_CACHE_HOME", overridebuf);
  }
  {
    char teststr[PATH_MAX];
    CACHESETENV("HOME", overridebuf, "tests/init/HOME/cache");
    REPORT(dryoinit(teststr, "test.log", "TestProject", DRYOX_XDG_CACHE), "cache HOME");
    RESTOREENV("HOME", overridebuf);
  }

  // LITERAL
  {
    char teststr[PATH_MAX];
    REPORT(dryoinit(teststr, NULL, NULL, DRYOX_LITERAL, "tests/logging/test.log"), "literal");
  }
}