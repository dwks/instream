#include <stdio.h>  // for printf
#include <unistd.h>  // for fork
#include <sys/ptrace.h>  // for ptrace
#include <sys/wait.h>  // for waitpid

pid_t spawn_program(char **argv) {
    pid_t pid = fork();
    if(pid == 0) {  // child, run the target program
        ptrace(PTRACE_TRACEME, 0, 0, 0);

        printf("[%d] Executing target...\n", (int)getpid());

        execvp(argv[0], argv);
	}
	else {  // parent, set up tracing
        ptrace(PTRACE_ATTACH, pid, 0, 0);

        printf("[%d] Done attaching onto %d\n", (int)getpid(), pid);
	}

    return pid;
}
