#include "dryox/dryoconfig.h"
#include "dryox/dryoinit.h"
#include "dryox/dryologging.h"
#include "tomlc17/tomlc17.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*
dryoconfig.c is a libdryox module.

It serves the following extern functions:
  dryoconfig_t dryoconfig_START_DECLARATION(char *filename, char *projectname);
  int dryoconfig_declare_section(char *name, dryoconfig_t *dryo_config);
  int dryoconfig_declare_comment(char *comment, bool is_inline);

It defines the following types:
  (none -- dryoconfig_t is defined in dryoconfig.h)
*/

// -- Start declaration functions --

// Each of the declaration functions are for declaring expected configs, and
// do throw an error if (iff the config exists) the config does not match
// expected.

// Below is similar to VA_START.

dryoconfig_t dryoconfig_START_DECLARATION(char *filename, char *projectname)
{
  assert(filename != NULL && projectname != NULL);

  char fname[PATH_MAX];

  if (dryoinit(fname, filename, projectname, DRYOX_XDG_CONFIG) == -1)
  {
    int errbuf = errno;
    dryolog_parrot_errno(errbuf, "dryoinit could not instantiate the config file.");
    return (dryoconfig_t){.err = errbuf};
  }

  dryoconfig_t dryo_config = {0};

  dryo_config.file = fopen(fname, "a+");

  if (dryo_config.file == NULL)
  {
    int errbuf = errno;
    dryolog_parrot_errno(errbuf, "fopen did not return a file.");
    return (dryoconfig_t){.err = errbuf};
  }

  return dryo_config;
}

// "Declare a section", aka. verify that the section exists; if it doesn't,
// make a new section. If the program can't write to file, raise an error.

int dryoconfig_declare_section(char *name, dryoconfig_t *dryo_config)
{
  if (dryo_config->file == NULL)
  {
    return -1;
  }
  toml_result_t result = toml_parse_file(dryo_config->file);
  toml_datum_t datum = toml_seek(result.toptab, name);

  if (datum.type == TOML_UNKNOWN)
  {
    // print to file. then save to file. then rewind file pointer.
    fprintf(dryo_config->file, "%s%s%s\n", "[", name, "]");
    fflush(dryo_config->file);
    rewind(dryo_config->file);

    result = toml_parse_file(dryo_config->file);
    datum = toml_seek(result.toptab, name);

    if (datum.type == TOML_UNKNOWN)
    {
      dryolog(LOG_ERROR, "%s %s %s\n", "dryoconfig could not write section", name, "to file.");
      toml_free(result);
      return 1;
    }
  }
  toml_free(result);
  return 0;
}

int dryoconfig_declare_comment(char *comment, bool is_inline) { return 0; }
