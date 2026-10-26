/* check-only stand-in for devkitARM's sys/dirent.h */
#pragma once
#include <sys/types.h>
typedef struct { int dummy; } DIR;
struct dirent { ino_t d_ino; unsigned char d_type; char d_name[256]; };
