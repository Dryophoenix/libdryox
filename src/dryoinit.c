#include "dryox/dryoinit.h"
#include "utils/dryo_mkdirp.h"
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENV_NAME_MAX 32

/*
init.c is a libdryox module.
It serves the following functions:
    dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir
mode, ...);

It serves the following types:
    typedef enum Dryox_XDG_Dir;
*/

/* This is defined in the header.
typedef enum
{
    DRYOX_XDG_UNSET,
    DRYOX_XDG_CONFIG,
    DRYOX_XDG_DATA,
    DRYOX_XDG_STATE,
    DRYOX_XDG_CACHE,
    DRYOX_LITERAL,
} Dryox_XDG_Dir;
*/
typedef struct
{
  Dryox_XDG_Dir mode;
  char env_override[ENV_NAME_MAX];
  char env_xdg[ENV_NAME_MAX];
  char format_override[PATH_MAX];
  char format_xdg[PATH_MAX];
  char format_home[PATH_MAX];
} mode_associated_data;

mode_associated_data mode_associations[6] = {
    {DRYOX_XDG_UNSET, "", "", "", "", ""},
    {DRYOX_XDG_CONFIG, "DENV_OVERRIDE_CONFIG", "XDG_CONFIG_HOME", "%s/%s/%s", "%s/%s/%s", "%s/.config/%s/%s"},
    {DRYOX_XDG_DATA, "DENV_OVERRIDE_DATA", "XDG_DATA_HOME", "%s/%s/%s", "%s/%s/%s", "%s/.local/share/%s/%s"},
    {DRYOX_XDG_STATE, "DENV_OVERRIDE_STATE", "XDG_STATE_HOME", "%s/%s/%s", "%s/%s/%s", "%s/.local/state/%s/%s"},
    {DRYOX_XDG_CACHE, "DENV_OVERRIDE_CACHE", "XDG_CACHE_HOME", "%s/%s/%s", "%s/%s/%s", "%s/.cache/%s/%s"},
    {DRYOX_LITERAL, "", "", "", "", ""},
};

int dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir mode, ...)
{
  // start --

  // -- this section handles arguments. --
  if (filename == NULL)
    filename = "default.log";

  if (projectname == NULL)
    projectname = "DryoX";

  if (mode == DRYOX_XDG_UNSET)
  {
    fprintf(stderr, "%s line %d %s fatal: mode may not be unset and explicitly does not default.", __FILE__, __LINE__,
            __func__);
    return -1;
  }

  va_list args;
  // --

  // -- begin variables initializing --
  const char *HOME = getenv("HOME");

  char DFILE[PATH_MAX]; // DFILE is "dryofile",

  // -- end variables initializing --

  // -- begin environment handling --

  if (mode == DRYOX_LITERAL)
  {
    va_start(args, mode);
    char *arg = va_arg(args, char *);
    if (arg == NULL)
    {
      va_end(args);
      fprintf(stderr,
              "%s line %d %s fatal: DRYOX_LITERAL must pass a path as a "
              "variadic ..., as char *",
              __FILE__, __LINE__, __func__);
      return -1;
    }

    snprintf(DFILE, PATH_MAX, "%s", arg);
    va_end(args);
  }

  else
  {
    mode_associated_data *mode_associated_datum = &mode_associations[mode];
    const char *env_ov = getenv(mode_associated_datum->env_override);
    const char *env_xdg = getenv(mode_associated_datum->env_xdg);
    if (env_ov != NULL)
      snprintf(DFILE, PATH_MAX, mode_associated_datum->format_override, env_ov, projectname, filename);
    else if (env_xdg != NULL)
      snprintf(DFILE, PATH_MAX, mode_associated_datum->format_xdg, env_xdg, projectname, filename);
    else if (HOME != NULL)
      snprintf(DFILE, PATH_MAX, mode_associated_datum->format_home, HOME, projectname, filename);
    else
    {
      fprintf(stderr, "%s line %d %s fatal: all environment variables unset.", __FILE__, __LINE__, __func__);
      return -1;
    }
  }
  // -- end environment handling --

  // -- begin finalizing --
  char DFILE_copy[PATH_MAX];
  snprintf(DFILE_copy, PATH_MAX, "%s", DFILE);
  dryo_mkdirp(dirname(DFILE_copy));
  snprintf(file_out, PATH_MAX, "%s", DFILE);
  FILE *f = fopen(DFILE, "a");
  if (f == NULL)
    return -1;
  fclose(f);
  return 0;
  // --

  // -- end.
}

char *dryoinit_external(char *filename, char *projectname, Dryox_XDG_Dir mode)
{
  static char file_out[PATH_MAX];
  if (dryoinit(file_out, filename, projectname, mode) == 0)
    return file_out;
  else
    return NULL;
}