cmd_arch/arm64/boot/dts/lentk6735m_65u_l1.dtb := /home/olegsvs/Android/aarch64-linux-android-4.9/bin/aarch64-linux-android-gcc -E -Wp,-MD,arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.d.pre.tmp -nostdinc -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/boot/dts -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/boot/dts/include -undef -D__DTS__ -x assembler-with-cpp -o arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.dts.tmp arch/arm64/boot/dts/lentk6735m_65u_l1.dts ; /home/olegsvs/5/Q37/alps/kernel-3.10/scripts/dtc/dtc -O dtb -o arch/arm64/boot/dts/lentk6735m_65u_l1.dtb -b 0 -i arch/arm64/boot/dts/ -i /home/olegsvs/5/Q37/alps/kernel-3.10/drivers/misc/mediatek/mach/mt6735/lentk6735m_65u_l1/dct/dct/ -d arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.d.dtc.tmp arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.dts.tmp ; cat arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.d.pre.tmp arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.d.dtc.tmp > arch/arm64/boot/dts/.lentk6735m_65u_l1.dtb.d

source_arch/arm64/boot/dts/lentk6735m_65u_l1.dtb := arch/arm64/boot/dts/lentk6735m_65u_l1.dts

deps_arch/arm64/boot/dts/lentk6735m_65u_l1.dtb := \
  arch/arm64/boot/dts/mt6735m.dtsi \
    $(wildcard include/config/base.h) \
    $(wildcard include/config/addr.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/drivers/misc/mediatek/mach/mt6735/lentk6735m_65u_l1/dct/dct/cust_eint.dtsi \

arch/arm64/boot/dts/lentk6735m_65u_l1.dtb: $(deps_arch/arm64/boot/dts/lentk6735m_65u_l1.dtb)

$(deps_arch/arm64/boot/dts/lentk6735m_65u_l1.dtb):
