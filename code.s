.global enable_tracing
.global enable_tracing_end

enable_tracing:
    pushf
    orl     $1 << 8, 0(%rsp)
    popf
    int3    # raise signal
enable_tracing_end:
    .skip   8
