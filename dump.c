#include <stdio.h>  // for printf
#include <stdlib.h>  // for malloc, free
#include <sys/ptrace.h>  // for ptrace

#include <distorm.h>
#include <mnemonics.h>

#include "dump.h"

#define MAX_INSTRUCTIONS 1
#define MAX_BUFFER_SIZE 256

static unsigned char dump_buffer[MAX_BUFFER_SIZE];

static void disassemble_helper(unsigned long remote_address,
    unsigned char *buffer, unsigned long size) {

    _DecodedInst instructions[MAX_INSTRUCTIONS];
    unsigned instructionCount = 0;
    unsigned long offset = 0;
    int done = 0;
    while(!done) {
        _DecodeResult result = distorm_decode(
            remote_address + offset,            // virtual address
            buffer + offset,                    // buffer to decode
            size - offset,                      // amount of data to decode
            Decode64Bits,
            instructions, MAX_INSTRUCTIONS, &instructionCount);

        switch(result) {
        case DECRES_INPUTERR:
            printf("Error calling distorm_decode\n");
            return;
        case DECRES_SUCCESS:
            // all instructions decoded
            //done = 1;
            break;
        default:
            break;
        }
        if(instructionCount == 0) break;

        for(unsigned i = 0; i < instructionCount; i ++) {
            printf("%16lx (%02d) %-24s %s%s%s\n",
                (unsigned long)instructions[i].offset,
                instructions[i].size,
                instructions[i].instructionHex.p,
                instructions[i].mnemonic.p,
                instructions[i].operands.length > 0 ? " " : "",
                instructions[i].operands.p);
        }

        offset = instructions[instructionCount - 1].offset
            - remote_address
            + instructions[instructionCount - 1].size;
    }
}

void disassemble_code_ptrace(unsigned long remote_address, pid_t pid) {
    for(unsigned long i = 0; i < MAX_BUFFER_SIZE; i += 8) {
        *(unsigned long *)(dump_buffer + i) = ptrace(PTRACE_PEEKTEXT, pid,
            remote_address + i, 0);
    }

    disassemble_helper(remote_address, dump_buffer, MAX_BUFFER_SIZE);
}
