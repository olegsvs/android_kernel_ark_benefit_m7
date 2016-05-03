	.arch armv8-a+fp+simd
	.file	"asm-offsets.c"
// GNU C (GCC) version 4.9 20150123 (prerelease) (aarch64-linux-android)
//	compiled by GNU C version 4.8, GMP version 5.0.5, MPFR version 3.1.1, MPC version 1.0.1
// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed:  -nostdinc
// -I /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include
// -I arch/arm64/include/generated -I include
// -I /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi
// -I arch/arm64/include/generated/uapi
// -I /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi
// -I include/generated/uapi
// -iprefix /home/olegsvs/Android/aarch64-linux-android-4.9/bin/../lib/gcc/aarch64-linux-android/4.9/
// -D __KERNEL__ -D KBUILD_STR(s)=#s
// -D KBUILD_BASENAME=KBUILD_STR(asm_offsets)
// -D KBUILD_MODNAME=KBUILD_STR(asm_offsets)
// -isystem /home/olegsvs/Android/aarch64-linux-android-4.9/bin/../lib/gcc/aarch64-linux-android/4.9/include
// -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h
// -MD arch/arm64/kernel/.asm-offsets.s.d arch/arm64/kernel/asm-offsets.c
// -mbionic -mlittle-endian -mgeneral-regs-only -mtune=cortex-a53
// -march=armv8-a -mabi=lp64 -auxbase-strip arch/arm64/kernel/asm-offsets.s
// -O2 -O3 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
// -Werror=implicit-function-declaration -Wno-format-security
// -Werror=format -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast
// -Wframe-larger-than=1400 -Wno-unused-but-set-variable
// -Wdeclaration-after-statement -Wno-pointer-sign -std=gnu90
// -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
// -fno-pic -ffast-math -fsingle-precision-constant -fgraphite-identity
// -ftree-loop-distribution -floop-interchange -floop-strip-mine
// -floop-block -fmodulo-sched -fmodulo-sched-allow-regmoves
// -fno-inline-functions -fno-stack-protector -fno-omit-frame-pointer
// -fno-optimize-sibling-calls -fno-var-tracking-assignments
// -fno-strict-overflow -fconserve-stack -fverbose-asm
// options enabled:  -faggressive-loop-optimizations -fassociative-math
// -fauto-inc-dec -fbranch-count-reg -fcaller-saves
// -fcombine-stack-adjustments -fcompare-elim -fcprop-registers
// -fcrossjumping -fcse-follow-jumps -fcx-limited-range -fdefer-pop
// -fdevirtualize-speculatively -fdwarf2-cfi-asm -fearly-inlining
// -feliminate-unused-debug-types -fexpensive-optimizations
// -ffinite-math-only -fforward-propagate -ffunction-cse -fgcse
// -fgcse-after-reload -fgcse-lm -fgnu-runtime -fgnu-unique
// -fgraphite-identity -fguess-branch-probability -fhoist-adjacent-loads
// -fident -fif-conversion -fif-conversion2 -findirect-inlining -finline
// -finline-atomics -finline-functions-called-once -finline-small-functions
// -fipa-cp -fipa-cp-clone -fipa-profile -fipa-pure-const -fipa-reference
// -fipa-sra -fira-hoist-pressure -fira-share-save-slots
// -fira-share-spill-slots -fisolate-erroneous-paths-dereference -fivopts
// -fkeep-static-consts -fleading-underscore -flifetime-dse -floop-block
// -floop-interchange -floop-strip-mine -fmerge-constants
// -fmerge-debug-strings -fmodulo-sched -fmodulo-sched-allow-regmoves
// -fmove-loop-invariants -fomit-frame-pointer -foptimize-strlen
// -fpartial-inlining -fpeel-codesize-limit -fpeephole -fpeephole2 -fplt
// -fpredictive-commoning -fprefetch-loop-arrays -freciprocal-math -free
// -freg-struct-return -freorder-blocks -freorder-functions
// -frerun-cse-after-loop -fsched-critical-path-heuristic
// -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
// -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
// -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-insns
// -fschedule-insns2 -fsection-anchors -fshow-column -fshrink-wrap
// -fsingle-precision-constant -fsplit-ivs-in-unroller -fsplit-wide-types
// -fstrict-enum-precision -fstrict-volatile-bitfields -fsync-libcalls
// -fthread-jumps -ftoplevel-reorder -ftree-bit-ccp -ftree-builtin-call-dce
// -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop
// -ftree-copyrename -ftree-cselim -ftree-dce -ftree-dominator-opts
// -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-distribute-patterns
// -ftree-loop-distribution -ftree-loop-if-convert -ftree-loop-im
// -ftree-loop-ivcanon -ftree-loop-optimize -ftree-loop-vectorize
// -ftree-parallelize-loops= -ftree-partial-pre -ftree-phiprop -ftree-pre
// -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink
// -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion
// -ftree-tail-merge -ftree-ter -ftree-vrp -funit-at-a-time
// -funroll-codesize-limit -funsafe-math-optimizations -funswitch-loops
// -fverbose-asm -fzero-initialized-in-bss -mandroid -mbionic
// -mfix-cortex-a53-835769 -mfix-cortex-a53-843419 -mgeneral-regs-only
// -mlittle-endian -mlra -momit-leaf-frame-pointer

	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.type	main, %function
main:
#APP
// 34 "arch/arm64/kernel/asm-offsets.c" 1
	
->TSK_ACTIVE_MM 720 offsetof(struct task_struct, active_mm)	//
// 0 "" 2
// 35 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 36 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_FLAGS 0 offsetof(struct thread_info, flags)	//
// 0 "" 2
// 37 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_PREEMPT 80 offsetof(struct thread_info, preempt_count)	//
// 0 "" 2
// 38 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_ADDR_LIMIT 8 offsetof(struct thread_info, addr_limit)	//
// 0 "" 2
// 39 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_TASK 16 offsetof(struct thread_info, task)	//
// 0 "" 2
// 40 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_EXEC_DOMAIN 24 offsetof(struct thread_info, exec_domain)	//
// 0 "" 2
// 41 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_CPU 84 offsetof(struct thread_info, cpu)	//
// 0 "" 2
// 42 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_CPU_EXCP 96 offsetof(struct thread_info, cpu_excp)	//
// 0 "" 2
// 43 "arch/arm64/kernel/asm-offsets.c" 1
	
->TI_REGS_ON_EXCP 88 offsetof(struct thread_info, regs_on_excp)	//
// 0 "" 2
// 44 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 45 "arch/arm64/kernel/asm-offsets.c" 1
	
->THREAD_CPU_CONTEXT 1264 offsetof(struct task_struct, thread.cpu_context)	//
// 0 "" 2
// 46 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 47 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X0 0 offsetof(struct pt_regs, regs[0])	//
// 0 "" 2
// 48 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X1 8 offsetof(struct pt_regs, regs[1])	//
// 0 "" 2
// 49 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X2 16 offsetof(struct pt_regs, regs[2])	//
// 0 "" 2
// 50 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X3 24 offsetof(struct pt_regs, regs[3])	//
// 0 "" 2
// 51 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X4 32 offsetof(struct pt_regs, regs[4])	//
// 0 "" 2
// 52 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X5 40 offsetof(struct pt_regs, regs[5])	//
// 0 "" 2
// 53 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X6 48 offsetof(struct pt_regs, regs[6])	//
// 0 "" 2
// 54 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X7 56 offsetof(struct pt_regs, regs[7])	//
// 0 "" 2
// 55 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X15 120 offsetof(struct pt_regs, regs[15])	//
// 0 "" 2
// 56 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X16 128 offsetof(struct pt_regs, regs[16])	//
// 0 "" 2
// 57 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X29 232 offsetof(struct pt_regs, regs[29])	//
// 0 "" 2
// 58 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_LR 240 offsetof(struct pt_regs, regs[30])	//
// 0 "" 2
// 59 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_SP 248 offsetof(struct pt_regs, sp)	//
// 0 "" 2
// 61 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_COMPAT_SP 104 offsetof(struct pt_regs, compat_sp)	//
// 0 "" 2
// 63 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_PSTATE 264 offsetof(struct pt_regs, pstate)	//
// 0 "" 2
// 64 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_PC 256 offsetof(struct pt_regs, pc)	//
// 0 "" 2
// 65 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_ORIG_X0 272 offsetof(struct pt_regs, orig_x0)	//
// 0 "" 2
// 66 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_SYSCALLNO 280 offsetof(struct pt_regs, syscallno)	//
// 0 "" 2
// 67 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_FRAME_SIZE 288 sizeof(struct pt_regs)	//
// 0 "" 2
// 68 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 69 "arch/arm64/kernel/asm-offsets.c" 1
	
->MM_CONTEXT_ID 712 offsetof(struct mm_struct, context.id)	//
// 0 "" 2
// 70 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 71 "arch/arm64/kernel/asm-offsets.c" 1
	
->VMA_VM_MM 64 offsetof(struct vm_area_struct, vm_mm)	//
// 0 "" 2
// 72 "arch/arm64/kernel/asm-offsets.c" 1
	
->VMA_VM_FLAGS 80 offsetof(struct vm_area_struct, vm_flags)	//
// 0 "" 2
// 73 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 74 "arch/arm64/kernel/asm-offsets.c" 1
	
->VM_EXEC 4 VM_EXEC	//
// 0 "" 2
// 75 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 76 "arch/arm64/kernel/asm-offsets.c" 1
	
->PAGE_SZ 4096 PAGE_SIZE	//
// 0 "" 2
// 77 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 78 "arch/arm64/kernel/asm-offsets.c" 1
	
->CPU_INFO_SZ 24 sizeof(struct cpu_info)	//
// 0 "" 2
// 79 "arch/arm64/kernel/asm-offsets.c" 1
	
->CPU_INFO_SETUP 16 offsetof(struct cpu_info, cpu_setup)	//
// 0 "" 2
// 80 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 81 "arch/arm64/kernel/asm-offsets.c" 1
	
->DMA_BIDIRECTIONAL 0 DMA_BIDIRECTIONAL	//
// 0 "" 2
// 82 "arch/arm64/kernel/asm-offsets.c" 1
	
->DMA_TO_DEVICE 1 DMA_TO_DEVICE	//
// 0 "" 2
// 83 "arch/arm64/kernel/asm-offsets.c" 1
	
->DMA_FROM_DEVICE 2 DMA_FROM_DEVICE	//
// 0 "" 2
// 84 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 85 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_REALTIME 0 CLOCK_REALTIME	//
// 0 "" 2
// 86 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_MONOTONIC 1 CLOCK_MONOTONIC	//
// 0 "" 2
// 87 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_REALTIME_RES 1 MONOTONIC_RES_NSEC	//
// 0 "" 2
// 88 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_REALTIME_COARSE 5 CLOCK_REALTIME_COARSE	//
// 0 "" 2
// 89 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_MONOTONIC_COARSE 6 CLOCK_MONOTONIC_COARSE	//
// 0 "" 2
// 90 "arch/arm64/kernel/asm-offsets.c" 1
	
->CLOCK_COARSE_RES 10000000 LOW_RES_NSEC	//
// 0 "" 2
// 91 "arch/arm64/kernel/asm-offsets.c" 1
	
->NSEC_PER_SEC 1000000000 NSEC_PER_SEC	//
// 0 "" 2
// 92 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 93 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_CS_CYCLE_LAST 0 offsetof(struct vdso_data, cs_cycle_last)	//
// 0 "" 2
// 94 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_XTIME_CLK_SEC 8 offsetof(struct vdso_data, xtime_clock_sec)	//
// 0 "" 2
// 95 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_XTIME_CLK_NSEC 16 offsetof(struct vdso_data, xtime_clock_nsec)	//
// 0 "" 2
// 96 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_XTIME_CRS_SEC 24 offsetof(struct vdso_data, xtime_coarse_sec)	//
// 0 "" 2
// 97 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_XTIME_CRS_NSEC 32 offsetof(struct vdso_data, xtime_coarse_nsec)	//
// 0 "" 2
// 98 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_WTM_CLK_SEC 40 offsetof(struct vdso_data, wtm_clock_sec)	//
// 0 "" 2
// 99 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_WTM_CLK_NSEC 48 offsetof(struct vdso_data, wtm_clock_nsec)	//
// 0 "" 2
// 100 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_TB_SEQ_COUNT 56 offsetof(struct vdso_data, tb_seq_count)	//
// 0 "" 2
// 101 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_CS_MULT 60 offsetof(struct vdso_data, cs_mult)	//
// 0 "" 2
// 102 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_CS_SHIFT 64 offsetof(struct vdso_data, cs_shift)	//
// 0 "" 2
// 103 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_TZ_MINWEST 68 offsetof(struct vdso_data, tz_minuteswest)	//
// 0 "" 2
// 104 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_TZ_DSTTIME 72 offsetof(struct vdso_data, tz_dsttime)	//
// 0 "" 2
// 105 "arch/arm64/kernel/asm-offsets.c" 1
	
->VDSO_USE_SYSCALL 76 offsetof(struct vdso_data, use_syscall)	//
// 0 "" 2
// 106 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 107 "arch/arm64/kernel/asm-offsets.c" 1
	
->TVAL_TV_SEC 0 offsetof(struct timeval, tv_sec)	//
// 0 "" 2
// 108 "arch/arm64/kernel/asm-offsets.c" 1
	
->TVAL_TV_USEC 8 offsetof(struct timeval, tv_usec)	//
// 0 "" 2
// 109 "arch/arm64/kernel/asm-offsets.c" 1
	
->TSPEC_TV_SEC 0 offsetof(struct timespec, tv_sec)	//
// 0 "" 2
// 110 "arch/arm64/kernel/asm-offsets.c" 1
	
->TSPEC_TV_NSEC 8 offsetof(struct timespec, tv_nsec)	//
// 0 "" 2
// 111 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 112 "arch/arm64/kernel/asm-offsets.c" 1
	
->TZ_MINWEST 0 offsetof(struct timezone, tz_minuteswest)	//
// 0 "" 2
// 113 "arch/arm64/kernel/asm-offsets.c" 1
	
->TZ_DSTTIME 4 offsetof(struct timezone, tz_dsttime)	//
// 0 "" 2
// 114 "arch/arm64/kernel/asm-offsets.c" 1
	
->
// 0 "" 2
// 148 "arch/arm64/kernel/asm-offsets.c" 1
	
->CPU_SUSPEND_SZ 128 sizeof(struct cpu_suspend_ctx)	//
// 0 "" 2
// 149 "arch/arm64/kernel/asm-offsets.c" 1
	
->CPU_CTX_SP 120 offsetof(struct cpu_suspend_ctx, sp)	//
// 0 "" 2
// 150 "arch/arm64/kernel/asm-offsets.c" 1
	
->MPIDR_HASH_MASK 0 offsetof(struct mpidr_hash, mask)	//
// 0 "" 2
// 151 "arch/arm64/kernel/asm-offsets.c" 1
	
->MPIDR_HASH_SHIFTS 8 offsetof(struct mpidr_hash, shift_aff)	//
// 0 "" 2
// 152 "arch/arm64/kernel/asm-offsets.c" 1
	
->SLEEP_SAVE_SP_SZ 16 sizeof(struct sleep_save_sp)	//
// 0 "" 2
// 153 "arch/arm64/kernel/asm-offsets.c" 1
	
->SLEEP_SAVE_SP_PHYS 8 offsetof(struct sleep_save_sp, save_ptr_stash_phys)	//
// 0 "" 2
// 154 "arch/arm64/kernel/asm-offsets.c" 1
	
->SLEEP_SAVE_SP_VIRT 0 offsetof(struct sleep_save_sp, save_ptr_stash)	//
// 0 "" 2
#NO_APP
	mov	w0, 0	//,
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.9 20150123 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
