#include "dryox/dryoinit.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include "utils/dryo_mkdirp.h"

// Init version 0.2.0

/*
init.c is a libdryox module.
It serves the following extern functions:
    dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir *mode, ...);

It defines the following types:
    typedef enum Dryox_XDG_Dir;
*/

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
        fprintf(stderr, "%s line %d %s fatal: mode may not be unset and does not default", __FILE__, __LINE__, __func__);
        return -1;
    }

    va_list args;
    // -- end arguments handling --

    // -- begin variables initializing --

    const char *HOME = getenv("HOME");

    char DPATH[PATH_MAX];
    char DFILE[PATH_MAX];

    // -- end variables initializing --

    // -- begin environment handling --

    switch (mode)
    {

    case DRYOX_XDG_CONFIG:
    {
        const char *DENV_OVERRIDE = getenv("DENV_OVERRIDE_CONFIG");
        const char *DXDG_CONFIG = getenv("XDG_CONFIG_HOME");
        if (DENV_OVERRIDE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DENV_OVERRIDE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DENV_OVERRIDE, projectname, filename);
        }
        else if (DXDG_CONFIG != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DXDG_CONFIG);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DXDG_CONFIG, projectname, filename);
        }
        else if (HOME != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s/.config", HOME);
            snprintf(DFILE, PATH_MAX, "%s/.config/%s/%s", HOME, projectname, filename);
        }
        else
        {
            fprintf(stderr, "%s line %d %s fatal: DENV_OVERRIDE_CONFIG, XDG_CONFIG_HOME, and HOME are all unset.", __FILE__, __LINE__, __func__);
            return -1;
        }
        break;
    }

    case DRYOX_XDG_DATA:
    {
        const char *DENV_OVERRIDE = getenv("DENV_OVERRIDE_DATA");
        const char *DXDG_DATA = getenv("XDG_DATA_HOME");
        if (DENV_OVERRIDE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DENV_OVERRIDE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DENV_OVERRIDE, projectname, filename);
        }
        else if (DXDG_DATA != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DXDG_DATA);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DXDG_DATA, projectname, filename);
        }
        else if (HOME != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s/.local/share", HOME);
            snprintf(DFILE, PATH_MAX, "%s/.local/share/%s/%s", HOME, projectname, filename);
        }
        else
        {
            fprintf(stderr, "%s line %d %s fatal: DENV_OVERRIDE_DATA, XDG_DATA_HOME, and HOME are all unset.", __FILE__, __LINE__, __func__);
            return -1;
        }
        break;
    }

    case DRYOX_XDG_STATE:
    {
        const char *DENV_OVERRIDE = getenv("DENV_OVERRIDE_STATE");
        const char *DXDG_STATE = getenv("XDG_STATE_HOME");
        if (DENV_OVERRIDE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DENV_OVERRIDE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DENV_OVERRIDE, projectname, filename);
        }
        else if (DXDG_STATE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DXDG_STATE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DXDG_STATE, projectname, filename);
        }
        else if (HOME != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s/.local/state", HOME);
            snprintf(DFILE, PATH_MAX, "%s/.local/state/%s/%s", HOME, projectname, filename);
        }
        else
        {
            fprintf(stderr, "%s line %d %s fatal: DENV_OVERRIDE_STATE, XDG_STATE_HOME, and HOME are all unset.", __FILE__, __LINE__, __func__);
            return -1;
        }
        break;
    }

    case DRYOX_XDG_CACHE:
    {
        const char *DENV_OVERRIDE = getenv("DENV_OVERRIDE_CACHE");
        const char *DXDG_CACHE = getenv("XDG_CACHE_HOME");
        if (DENV_OVERRIDE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DENV_OVERRIDE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DENV_OVERRIDE, projectname, filename);
        }
        else if (DXDG_CACHE != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s", DXDG_CACHE);
            snprintf(DFILE, PATH_MAX, "%s/%s/%s", DXDG_CACHE, projectname, filename);
        }
        else if (HOME != NULL)
        {
            snprintf(DPATH, PATH_MAX, "%s/.cache", HOME);
            snprintf(DFILE, PATH_MAX, "%s/.cache/%s/%s", HOME, projectname, filename);
        }
        else
        {
            fprintf(stderr, "%s line %d %s fatal: DENV_OVERRIDE_CACHE, XDG_CACHE_HOME, and HOME are all unset.", __FILE__, __LINE__, __func__);
            return -1;
        }
        break;
    }

    case DRYOX_LITERAL:
        va_start(args, mode);
        char *arg = va_arg(args, char *);
        char arg_copy[PATH_MAX];
        snprintf(arg_copy, PATH_MAX, "%s", arg);
        if (arg == NULL)
        {
            va_end(args);
            fprintf(stderr, "%s line %d %s fatal: DRYOX_LITERAL must pass a path as a variadic ..., as char *", __FILE__, __LINE__, __func__);
            return -1;
        }
        snprintf(DFILE, PATH_MAX, "%s", arg);
        snprintf(DPATH, PATH_MAX, "%s", dirname(arg_copy));
        va_end(args);
        break;

    default:
        fprintf(stderr, "%s line %d %s fatal: mode switch somehow defaulted.", __FILE__, __LINE__, __func__);
        return -1;
    }
    // -- end environment handling --

    // -- begin finalizing --

    dryou_mkdirp(DPATH);
    snprintf(file_out, PATH_MAX, "%s", DFILE);
    FILE *f = fopen(DFILE, "a");
    if (f == NULL)
        return -1;
    fclose(f);
    return 0;

    // -- end finalizing --

    // -- end.
}