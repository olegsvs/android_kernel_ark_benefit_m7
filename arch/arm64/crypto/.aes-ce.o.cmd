cmd_arch/arm64/crypto/aes-ce.o := aarch64-gcc -Wp,-MD,arch/arm64/crypto/.aes-ce.o.d  -nostdinc -isystem /home/olegsvs/5/aarch64-linux-gnu-5.3/bin/../lib/gcc/aarch64-linux-gnu/5.3.x-sabermod/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian  -D__ASSEMBLY__      -DINTERLEAVE=2 -DINTERLEAVE_INLINE   -c -o arch/arm64/crypto/aes-ce.o arch/arm64/crypto/aes-ce.S

source_arch/arm64/crypto/aes-ce.o := arch/arm64/crypto/aes-ce.S

deps_arch/arm64/crypto/aes-ce.o := \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/linkage.h \
  arch/arm64/crypto/aes-modes.S \

arch/arm64/crypto/aes-ce.o: $(deps_arch/arm64/crypto/aes-ce.o)

$(deps_arch/arm64/crypto/aes-ce.o):
