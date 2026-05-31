#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <libgen.h>
#include "dryox/utils.h"

int dryoinit(char *logfile_out, char *filename, char *projectname)
{
    // If there's an environmental variable, handle it...
    const char *LOG_FILE_ENV = getenv("LOG_FILE");

    if (LOG_FILE_ENV != NULL)
    {
        strncpy(logfile_out, LOG_FILE_ENV, PATH_MAX - 1);
        logfile_out[PATH_MAX - 1] = '\0';
        mkdirp(dirname(logfile_out));
        FILE *f = fopen(logfile_out, "a");
        fclose(f);
        return 0;
    }

    if (filename == NULL)
        filename = "catchall.log";
    if (projectname == NULL)
        projectname = "DryoX";

    const char *DRYOX_LOG_DIR = getenv("DRYOX_LOG_DIR");
    const char *HOME = getenv("HOME");
    const char *XDG_STATE_HOME = getenv("XDG_STATE_HOME");
    char LOG_PATH[PATH_MAX];

    if (
        DRYOX_LOG_DIR != NULL)
    {
        snprintf(LOG_PATH, PATH_MAX, "%s/%s", DRYOX_LOG_DIR, projectname);
    }

    else if (
        XDG_STATE_HOME != NULL)
    {
        snprintf(LOG_PATH, PATH_MAX, "%s/%s", XDG_STATE_HOME, projectname);
    }

    else if (
        HOME != NULL)
    {
        snprintf(LOG_PATH, PATH_MAX, "%s/.local/state/%s", HOME, projectname);
    }

    else
    {
        fprintf(stderr,
                "The libdryox module \"init\" could not find a valid path.\n"
                "char DRYOX_LOG_DIR appears undefined,\n"
                "char XDG_STATE_HOME appears undefined,\n"
                "char HOME appears undefined,\n"
                "or the called function has no access to any of the above.\n");
        return -1;
    }
    mkdirp(LOG_PATH);
    char LOG_FILE[PATH_MAX];
    snprintf(LOG_FILE, PATH_MAX, "%s/%s", LOG_PATH, filename);
    strncpy(logfile_out, LOG_FILE, PATH_MAX - 1);
    logfile_out[PATH_MAX - 1] = '\0';
    FILE *f = fopen(LOG_FILE, "a");
    if (f == NULL)
        return -1;
    fclose(f);
    return 0;
}

int dryoinit_absolute(char *logfile_out, char *rawpath)
{
    if (rawpath == NULL)
    {
        fprintf(stderr,
                "The libdryox module \"init\" (absolute variant) was not assigned a path,\n"
                "or the called function has no access to it.\n");
        return -1;
    }
    strncpy(logfile_out, rawpath, PATH_MAX - 1);
    logfile_out[PATH_MAX - 1] = '\0';
    mkdirp(dirname(rawpath));
    FILE *f = fopen(rawpath, "a");
    if (f == NULL)
        return -1;
    fclose(f);
    return 0;
}