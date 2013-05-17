cmd_arch/arm64/kernel/hyp-stub.o := aarch64-gcc -Wp,-MD,arch/arm64/kernel/.hyp-stub.o.d  -nostdinc -isystem /home/olegsvs/5/aarch64-linux-gnu-5.3/bin/../lib/gcc/aarch64-linux-gnu/5.3.x-sabermod/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian  -D__ASSEMBLY__         -c -o arch/arm64/kernel/hyp-stub.o arch/arm64/kernel/hyp-stub.S

source_arch/arm64/kernel/hyp-stub.o := arch/arm64/kernel/hyp-stub.S

deps_arch/arm64/kernel/hyp-stub.o := \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/modules.h) \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
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
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/linkage.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/assembler.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/big/endian.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/ptrace.h \
    $(wildcard include/config/compat.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/ptrace.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/hwcap.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/hwcap.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/virt.h \

arch/arm64/kernel/hyp-stub.o: $(deps_arch/arm64/kernel/hyp-stub.o)

$(deps_arch/arm64/kernel/hyp-stub.o):
