cmd_drivers/staging/android/ion/ion_profile.o := aarch64-gcc -Wp,-MD,drivers/staging/android/ion/.ion_profile.o.d  -nostdinc -isystem /home/olegsvs/5/aarch64-linux-gnu-5.3/bin/../lib/gcc/aarch64-linux-gnu/5.3.x-sabermod/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Werror=format -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast -std=gnu89 -O2 -mgeneral-regs-only -fno-pic -mtune=cortex-a53 -march=armv8-a -O3 -ffast-math -fsingle-precision-constant -fgraphite-identity -ftree-loop-distribution -ftree-loop-linear -floop-interchange -floop-strip-mine -floop-block -fmodulo-sched -fmodulo-sched-allow-regmoves -fno-inline-functions -Wframe-larger-than=1400 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android/ion -DION_RUNTIME_DEBUGGER=0    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ion_profile)"  -D"KBUILD_MODNAME=KBUILD_STR(ion_profile)" -c -o drivers/staging/android/ion/ion_profile.o drivers/staging/android/ion/ion_profile.c

source_drivers/staging/android/ion/ion_profile.o := drivers/staging/android/ion/ion_profile.c

deps_drivers/staging/android/ion/ion_profile.o := \
  drivers/staging/android/ion/ion_profile.h \
  include/linux/mmprofile.h \
  include/linux/mmprofile_static_event.h \

drivers/staging/android/ion/ion_profile.o: $(deps_drivers/staging/android/ion/ion_profile.o)

$(deps_drivers/staging/android/ion/ion_profile.o):
