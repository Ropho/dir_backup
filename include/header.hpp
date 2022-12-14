#ifndef HEADER_HPP
#define HEADER_HPP


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define LOG(msg) do {                                                   \
                    fprintf (stderr, "\033[32m%s\n\033[0m", msg);       \
                } while (0)

#define NOTE(msg)  do {                                                   \
                    fprintf (stderr, "\033[33m%s\n\033[0m", msg);         \
                } while (0)

#define ERROR(msg) do {                                                                         \
                        int error = errno;                                                      \
                        fprintf (stderr, "\033[31m%s: %s\n\033[0m", msg, strerror (error));     \
                        exit (1);                                                               \
                    } while (0)


void backup (const char *src_parent_name, DIR *src,
             const char *dst_parent_name, DIR *dst);

#endif