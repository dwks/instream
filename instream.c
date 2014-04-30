#include <stdio.h>
#include <sys/ptrace.h>  // for ptrace
#include <sys/wait.h>  // for waitpid
#include <sys/user.h>  // for user_regs_struct
#include "spawn.h"

static void loop(pid_t pid);

/*void handle_interrupt(int sig) {
    if(sig == SIGINT) {
        ptrace(PTRACE_DETACH, pid, 0, 0);
    }
}*/

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("Usage: %s program args...\n", argv[0]);
        return 1;
    }

    pid_t pid = spawn_program(argv + 1);
    loop(pid);

    return 0;
}

static unsigned long current_rip(pid_t pid) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, 0, &regs);

    return (unsigned long)regs.rip;
}

static void loop(pid_t pid) {
    for(;;) {
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status)) {
            printf("Target exited with status %d\n", WEXITSTATUS(status));
            break;
        }
        else if(WIFSIGNALED(status)) {
            printf("Target killed by signal %d\n", WTERMSIG(status));
            break;
        }
        else if(WIFSTOPPED(status)) {
            int sig = WSTOPSIG(status);

            if(sig == SIGTRAP) {
                printf("[%12lx]\n", current_rip(pid));
                ptrace(PTRACE_CONT, pid, 0, 0);
            }
            else {
                printf("Target received signal %d\n", sig);
                ptrace(PTRACE_CONT, pid, 0, sig);
            }
        }
        else if(WIFCONTINUED(status)) {
            printf("Target continued\n");
        }
        else {
            printf("Target stopped for unknown reason\n");
            break;  // for debugging
        }
    }
}
