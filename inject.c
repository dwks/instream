#include <stdio.h>  // for sprintf, perror
#include <stdlib.h>  // for exit
#include <sys/mman.h>  // for mmap
#include <sys/types.h>  // for open
#include <sys/ptrace.h>  // for ptrace
#include <sys/user.h>  // for user_regs_struct
#include <fcntl.h>  // for open
#include <unistd.h>  // for close
#include <elf.h>

extern const unsigned long enable_tracing, enable_tracing_end;

#define MAX_CODE_SIZE 64
static char original_code[MAX_CODE_SIZE];
static unsigned long entry_point;

static void die(const char *message) {
    perror(message);
    exit(1);
}

void inject_at_entry_point(pid_t pid) {
    char filename[BUFSIZ];
    sprintf(filename, "/proc/%d/exe", (int)pid);

    const size_t length = 0x1000;

    int fd = open(filename, O_RDONLY);
    if(fd < 0) die("open");
    void *addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if(!addr) die("mmap");

    Elf64_Ehdr *header = addr;
    entry_point = (unsigned long)header->e_entry;

    if(header->e_type == ET_DYN) {
        sprintf(filename, "/proc/%d/maps", (int)pid);
        int mapsfd = open(filename, O_RDONLY);
        char line[BUFSIZ];
        read(mapsfd, line, sizeof(line));
        close(mapsfd);
        unsigned long base = strtol(line, NULL, 16);

        printf("PIE entry point: %lx+%lx = %lx\n", base, entry_point, base + entry_point);
        entry_point += base;
    }
    else {
        printf("non-pie entry point: %lx\n", entry_point);
    }
    for(int i = 0; i < MAX_CODE_SIZE; i += 8) {
        unsigned long data = ptrace(PTRACE_PEEKTEXT, pid,
            entry_point + i, 0);
        *(unsigned long *)(original_code + i) = data;
    }

#if 1
    int code_size = (int)((unsigned long)&enable_tracing_end
        - (unsigned long)&enable_tracing);
    code_size = (code_size + 7) & ~0x7;  // round up to nearest 8 bytes
    for(int i = 0; i < code_size; i += 8) {
        ptrace(PTRACE_POKETEXT, pid, entry_point + i,
            *(unsigned long *)((unsigned long)&enable_tracing + i));
        //printf("write %lx\n",
        //    *(unsigned long *)((unsigned long)&enable_tracing + i));
    }
#endif

    munmap(addr, length);
    close(fd);
}

static void set_rip(pid_t pid, unsigned long new_rip) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, 0, &regs);

    regs.rip = new_rip;

    ptrace(PTRACE_SETREGS, pid, 0, &regs);
}

void undo_code_injection(pid_t pid) {
    for(int i = 0; i < MAX_CODE_SIZE; i += 8) {
        ptrace(PTRACE_POKETEXT, pid, entry_point + i,
            *(unsigned long *)(original_code + i));
    }

    set_rip(pid, entry_point);
}
