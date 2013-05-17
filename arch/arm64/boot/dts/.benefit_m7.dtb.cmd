cmd_arch/arm64/boot/dts/benefit_m7.dtb := aarch64-gcc -E -Wp,-MD,arch/arm64/boot/dts/.benefit_m7.dtb.d.pre.tmp -nostdinc -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/boot/dts -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/boot/dts/include -undef -D__DTS__ -x assembler-with-cpp -o arch/arm64/boot/dts/.benefit_m7.dtb.dts.tmp arch/arm64/boot/dts/benefit_m7.dts ; /home/olegsvs/5/Q37/alps/kernel-3.10/scripts/dtc/dtc -O dtb -o arch/arm64/boot/dts/benefit_m7.dtb -b 0 -i arch/arm64/boot/dts/ -i /home/olegsvs/5/Q37/alps/kernel-3.10/drivers/misc/mediatek/mach/mt6735/benefit_m7/dct/dct/ -d arch/arm64/boot/dts/.benefit_m7.dtb.d.dtc.tmp arch/arm64/boot/dts/.benefit_m7.dtb.dts.tmp ; cat arch/arm64/boot/dts/.benefit_m7.dtb.d.pre.tmp arch/arm64/boot/dts/.benefit_m7.dtb.d.dtc.tmp > arch/arm64/boot/dts/.benefit_m7.dtb.d

source_arch/arm64/boot/dts/benefit_m7.dtb := arch/arm64/boot/dts/benefit_m7.dts

deps_arch/arm64/boot/dts/benefit_m7.dtb := \

arch/arm64/boot/dts/benefit_m7.dtb: $(deps_arch/arm64/boot/dts/benefit_m7.dtb)

$(deps_arch/arm64/boot/dts/benefit_m7.dtb):
