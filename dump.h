#ifndef DUMP_H
#define DUMP_H

#include <sys/types.h>  // for pid_t

void disassemble_code_ptrace(unsigned long remote_address, pid_t pid);

#endif
