#ifndef INJECT_H
#define INJECT_H

#include <sys/types.h>  // for pid_t

void inject_at_entry_point(pid_t pid);
void undo_code_injection(pid_t pid);

#endif
