#include <stdio.h>  // for printf
#include <unistd.h>  // for fork
#include <sys/ptrace.h>  // for ptrace
#include <sys/wait.h>  // for waitpid
#include <sys/types.h>  // for pid_t

pid_t spawn_program(char **argv) {
    pid_t pid = fork();
    if(pid == 0) {  // child, run the target program
        ptrace(PTRACE_TRACEME, 0, 0, 0);

        printf("[%d] Executing target...\n", (int)getpid());

#if 0
        // turn on tracing right away... stops after the exec
        __asm__ __volatile__ (
            "pushf\n"
            "orl    $(1 << 8), 0(%rsp)\n"
            "popf\n"
        );
#endif

        execvp(argv[0], argv);
	}
	else {  // parent, set up tracing
        ptrace(PTRACE_ATTACH, pid, 0, 0);

        printf("[%d] Done attaching onto %d\n", (int)getpid(), pid);
	}

    return pid;
}
