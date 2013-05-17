cmd_arch/arm64/kernel/kuser32.o := aarch64-gcc -Wp,-MD,arch/arm64/kernel/.kuser32.o.d  -nostdinc -isystem /home/olegsvs/5/aarch64-linux-gnu-5.3/bin/../lib/gcc/aarch64-linux-gnu/5.3.x-sabermod/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian  -D__ASSEMBLY__         -c -o arch/arm64/kernel/kuser32.o arch/arm64/kernel/kuser32.S

source_arch/arm64/kernel/kuser32.o := arch/arm64/kernel/kuser32.S

deps_arch/arm64/kernel/kuser32.o := \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/unistd.h \
    $(wildcard include/config/compat.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/unistd.h \
  include/asm-generic/unistd.h \
  include/uapi/asm-generic/unistd.h \
    $(wildcard include/config/mmu.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  include/uapi/asm-generic/bitsperlong.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \

arch/arm64/kernel/kuser32.o: $(deps_arch/arm64/kernel/kuser32.o)

$(deps_arch/arm64/kernel/kuser32.o):
