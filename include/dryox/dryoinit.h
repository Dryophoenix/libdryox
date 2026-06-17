#ifndef DRYOX_INIT_H
#define DRYOX_INIT_H

typedef enum
{
  DRYOX_XDG_UNSET,
  DRYOX_XDG_CONFIG,
  DRYOX_XDG_DATA,
  DRYOX_XDG_STATE,
  DRYOX_XDG_CACHE,
  DRYOX_LITERAL,
} Dryox_XDG_Dir;

int dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir mode, ...);
char *dryoinit_external(char *filename, char *projectname, Dryox_XDG_Dir mode);

#endif
