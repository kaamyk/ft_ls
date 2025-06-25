#ifndef FTLS_H
#define FTLS_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/xattr.h>
#include <time.h>
#include <stdlib.h>
#include <error.h>
#include <stdlib.h>

/* === parser.c === */
int	parser( const int argc, const char **argv );

#endif