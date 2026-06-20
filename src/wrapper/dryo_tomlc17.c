#include "wrapper/dryo_tomlc17.h"
#include "dryox/dryologging.h"
#include "tomlc17/tomlc17.h"

/*
 * This is essentially just a wrapper of tomlc17 for a stable API.
 *
 */

toml_result_t dryo_toml_parse(FILE *f)
{
  toml_result_t result = toml_parse_file(f);
  if (result.ok != true)
  {
    dryolog(LOG_ERROR, "");
  }
  return result;
}

void dryo_toml_free(toml_result_t result) { toml_free(result); }
