cmd_lib/reciprocal_div.o := aarch64-gcc -Wp,-MD,lib/.reciprocal_div.o.d  -nostdinc -isystem /home/olegsvs/5/aarch64-linux-gnu-5.3/bin/../lib/gcc/aarch64-linux-gnu/5.3.x-sabermod/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Werror=format -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast -O2 -mgeneral-regs-only -fno-pic -mtune=cortex-a53 -march=armv8-a -O3 -ffast-math -fsingle-precision-constant -fgraphite-identity -ftree-loop-distribution -ftree-loop-linear -floop-interchange -floop-strip-mine -floop-block -fmodulo-sched -fmodulo-sched-allow-regmoves -fno-inline-functions -Wframe-larger-than=1400 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(reciprocal_div)"  -D"KBUILD_MODNAME=KBUILD_STR(reciprocal_div)" -c -o lib/reciprocal_div.o lib/reciprocal_div.c

source_lib/reciprocal_div.o := lib/reciprocal_div.c

deps_lib/reciprocal_div.o := \
  arch/arm64/include/generated/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/arm64/include/generated/asm/types.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc5.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  arch/arm64/include/generated/asm/posix_types.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/posix_types.h \
  include/linux/reciprocal_div.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \

lib/reciprocal_div.o: $(deps_lib/reciprocal_div.o)

$(deps_lib/reciprocal_div.o):
