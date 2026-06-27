#ifndef DRYOX_CONFIG_H
#define DRYOX_CONFIG_H

#include "tomlc17/tomlc17.h"
#include <stdio.h>

typedef struct dryoconfig_t dryoconfig_t;

typedef struct dryoconfig_t
{
  FILE *file;
  toml_result_t result;
  int err;
} dryoconfig_t;

#endif