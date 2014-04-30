#ifndef SPAWN_H
#define SPAWN_H

#include <sys/types.h>  // for pid_t

pid_t spawn_program(char **argv);

#endif
