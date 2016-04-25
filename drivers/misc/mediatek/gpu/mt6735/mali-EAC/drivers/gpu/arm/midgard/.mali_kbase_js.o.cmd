cmd_drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o := /home/olegsvs/Android/aarch64-linux-android-4.9/bin/aarch64-linux-android-gcc -Wp,-MD,drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/.mali_kbase_js.o.d  -nostdinc -isystem /home/olegsvs/Android/aarch64-linux-android-4.9/bin/../lib/gcc/aarch64-linux-android/4.9/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include -Iarch/arm64/include/generated  -Iinclude -I/home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi -Iinclude/generated/uapi -include /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Werror=format -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast -O2 -mgeneral-regs-only -fno-pic -Wframe-larger-than=1400 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/misc/mediatek/mach/mt6735/include -DMALI_CUSTOMER_RELEASE=1 -DMALI_KERNEL_TEST_API=0 -DMALI_UNIT_TEST=0 -DMALI_ERROR_INJECT_ON=0 -DMALI_MOCK_TEST=0 -DMALI_COVERAGE=0 -DMALI_INSTRUMENTATION_LEVEL=0 -DMALI_RELEASE_NAME=\""r5p0-06rel0"\" -DMALI_GCC_WORKAROUND_MIDCOM_4598=0 -DMALI_KBASE_THIRDPARTY_PATH=../../drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform/vexpress -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform_dummy -I -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/../../../base -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/linux -DCONFIG_MALI_PLATFORM_FAKE -DCONFIG_MALI_MIDGARD_DVFS -I-I/home/olegsvs/5/Q37/alps/kernel-3.10/include -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/misc/mediatek/mach/mt6735/include -DCONFIG_DEBUG_FS -DCONFIG_PROC_FS -DCONFIG_MALI_DEBUG -DMALI_CUSTOMER_RELEASE=1 -DMALI_KERNEL_TEST_API=0 -DMALI_UNIT_TEST=0 -DMALI_ERROR_INJECT_ON=0 -DMALI_MOCK_TEST=0 -DMALI_COVERAGE=0 -DMALI_INSTRUMENTATION_LEVEL=0 -DMALI_RELEASE_NAME=\""r5p0-06rel0"\" -DMALI_GCC_WORKAROUND_MIDCOM_4598=0 -DMALI_KBASE_THIRDPARTY_PATH=../../drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform/vexpress -I/home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform_dummy -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/../../../base -I/home/olegsvs/5/Q37/alps/kernel-3.10/include/linux -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard -Idrivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform/vexpress    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mali_kbase_js)"  -D"KBUILD_MODNAME=KBUILD_STR(mali_kbase)" -c -o drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.c

source_drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o := drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.c

deps_drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o := \
    $(wildcard include/config/mali/gator/support.h) \
    $(wildcard include/config/mali/debug.h) \
    $(wildcard include/config/attr/js/scheduling/tick/ns.h) \
    $(wildcard include/config/attr/js/soft/stop/ticks.h) \
    $(wildcard include/config/attr/js/soft/stop/ticks/cl.h) \
    $(wildcard include/config/attr/js/hard/stop/ticks/ss.h) \
    $(wildcard include/config/attr/js/hard/stop/ticks/cl.h) \
    $(wildcard include/config/attr/js/hard/stop/ticks/nss.h) \
    $(wildcard include/config/attr/js/reset/ticks/ss.h) \
    $(wildcard include/config/attr/js/reset/ticks/cl.h) \
    $(wildcard include/config/attr/js/reset/ticks/nss.h) \
    $(wildcard include/config/attr/js/ctx/timeslice/ns.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase.h \
    $(wildcard include/config/gpu/tracepoints.h) \
    $(wildcard include/config/mali/system/trace.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/mali_malisw.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/linux/stddef.h \
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
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/mali_stdtypes.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/arm_cstd/arm_cstd.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/arm_cstd/arm_cstd_compilers.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/arm_cstd/arm_cstd_types.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/arm_cstd/arm_cstd_types_gcc.h \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/olegsvs/Android/aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
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
  arch/arm64/include/generated/asm/posix_types.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/posix_types.h \
  include/linux/bitops.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/bitops.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/barrier.h \
    $(wildcard include/config/smp.h) \
  include/asm-generic/bitops/builtin-__ffs.h \
  include/asm-generic/bitops/builtin-ffs.h \
  include/asm-generic/bitops/builtin-__fls.h \
  include/asm-generic/bitops/builtin-fls.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/le.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  arch/arm64/include/generated/asm/swab.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/swab.h \
  include/linux/byteorder/generic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/uapi/linux/kernel.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/sysinfo.h \
  include/generated/uapi/linux/version.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_debug.h \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  arch/arm64/include/generated/asm/bug.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/page.h \
    $(wildcard include/config/arm64/64k/pages.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/pgtable-3level-types.h \
  include/asm-generic/pgtable-nopud.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/memory.h \
    $(wildcard include/config/compat.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/const.h \
  arch/arm64/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  include/linux/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/getorder.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/atomic.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/cmpxchg.h \
  include/asm-generic/atomic-long.h \
  include/linux/highmem.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/debug/highmem.h) \
  include/linux/fs.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/wait.h \
    $(wildcard include/config/lockdep.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/thread_info.h \
    $(wildcard include/config/mt/rt/sched.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/preempt/monitor.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/irqflags.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/ptrace.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/ptrace.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/hwcap.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/hwcap.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/spinlock.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/processor.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/string.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/fpsimd.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/hw_breakpoint.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  arch/arm64/include/generated/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/cache.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/seqlock.h \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/path.h \
  include/linux/stat.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/stat.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/stat.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/stat.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/compat.h \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/hmp/pack/small/task.h) \
    $(wildcard include/config/mtk/sched/cmp/tgs.h) \
    $(wildcard include/config/mtk/sched/cmp/pack/small/task.h) \
    $(wildcard include/config/mt/load/balance/profiler.h) \
    $(wildcard include/config/sched/hmp/prio/filter.h) \
    $(wildcard include/config/sched/hmp.h) \
    $(wildcard include/config/sched/hmp/enhancement.h) \
    $(wildcard include/config/hmp/tracer.h) \
    $(wildcard include/config/mtprof/cputime.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/mt/sched/trace.h) \
    $(wildcard include/config/mt/sched/debug.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/zram.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mt/prio/tracer.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mtk/sched/rqavg/us.h) \
    $(wildcard include/config/mtk/sched/rqavg/ks.h) \
  include/uapi/linux/sched.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/math64.h \
  arch/arm64/include/generated/asm/div64.h \
  include/asm-generic/div64.h \
  include/uapi/linux/time.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/param.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/timex.h \
  include/asm-generic/timex.h \
  include/linux/jiffies.h \
  include/linux/rbtree.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/arm64/include/generated/asm/errno.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/errno.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/errno-base.h \
  include/linux/nodemask.h \
    $(wildcard include/config/movable/node.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/compaction.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/auxvec.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
    $(wildcard include/config/arch/supports/uprobes.h) \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/mtk/svp.h) \
  include/generated/bounds.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/sparsemem.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/mmu.h \
  arch/arm64/include/generated/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/smp.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/uapi/linux/ipc.h \
  arch/arm64/include/generated/asm/ipcbuf.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/ipcbuf.h \
  arch/arm64/include/generated/asm/sembuf.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/sembuf.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/signal.h \
  include/asm-generic/signal.h \
  include/uapi/asm-generic/signal.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/signal-defs.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/sigcontext.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/sigcontext.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/mt/load/balance/enhancement.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/mtkpasr.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/mt/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/srcu.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/topology.h \
    $(wildcard include/config/arm/cpu/topology.h) \
    $(wildcard include/config/arch/scale/invariant/cpu/capacity.h) \
    $(wildcard include/config/disable/cpu/sched/domain/balance.h) \
    $(wildcard include/config/mtk/sched/cmp.h) \
  include/asm-generic/topology.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/seccomp.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/unistd.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/unistd.h \
  include/asm-generic/unistd.h \
  include/uapi/asm-generic/unistd.h \
  include/asm-generic/seccomp.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  arch/arm64/include/generated/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/sysctl.h \
  include/uapi/linux/sysctl.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  include/linux/gfp.h \
    $(wildcard include/config/pm/sleep.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/rtpm_prio.h \
    $(wildcard include/config/mt/rt/monitor.h) \
  include/linux/ptrace.h \
  include/linux/err.h \
  include/linux/pid_namespace.h \
    $(wildcard include/config/pid/ns.h) \
  include/linux/mm.h \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/arch/uses/numa/prot/none.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  include/linux/shrinker.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/pgtable.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/proc-fns.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/pgtable-hwdef.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/pgtable-3level-hwdef.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
  include/linux/nsproxy.h \
  include/linux/kref.h \
  include/uapi/linux/ptrace.h \
  include/uapi/linux/stat.h \
  include/linux/radix-tree.h \
  include/linux/semaphore.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/fiemap.h \
  include/linux/migrate_mode.h \
  include/linux/percpu-rwsem.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/uapi/linux/fs.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/limits.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/ioctl.h \
  arch/arm64/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/uapi/asm/fcntl.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/asm-generic/fcntl.h \
  include/linux/uaccess.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/uaccess.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/compiler.h \
  include/linux/hardirq.h \
    $(wildcard include/config/generic/hardirqs.h) \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  include/linux/vtime.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/hardirq.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/irq.h \
  include/asm-generic/irq.h \
  include/linux/irq_cpustat.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/cacheflush.h \
  arch/arm64/include/generated/asm/kmap_types.h \
  include/asm-generic/kmap_types.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/vmalloc.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_base_kernel.h \
    $(wildcard include/config/cpu/page/size/log2.h) \
    $(wildcard include/config/fault.h) \
    $(wildcard include/config/gpu/core/type.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_base_mem_priv.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_profiling_gator_api.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_uku.h \
    $(wildcard include/config/mali/error/inject.h) \
    $(wildcard include/config/mali/no/mali.h) \
    $(wildcard include/config/cpu/l1/dcache/line/size/log2.h) \
    $(wildcard include/config/cpu/l1/dcache/size.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_uk.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/malisw/mali_stdtypes.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_gpuprops_types.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_linux.h \
  include/linux/platform_device.h \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/hibernate/callbacks.h) \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
  include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/ratelimit.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/device.h \
  include/linux/pm_wakeup.h \
  include/linux/mod_devicetable.h \
  include/linux/uuid.h \
  include/uapi/linux/uuid.h \
  include/linux/miscdevice.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/major.h \
  include/linux/module.h \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/kmod.h \
  include/linux/elf.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/elf.h \
  arch/arm64/include/generated/asm/user.h \
  include/asm-generic/user.h \
  include/uapi/linux/elf.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/include/uapi/linux/elf-em.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/static_key.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/module.h \
  include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm.h \
    $(wildcard include/config/mali/midgard/dvfs.h) \
    $(wildcard include/config/pm/devfreq.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_midg_regmap.h \
    $(wildcard include/config/next.h) \
    $(wildcard include/config/start/flush/no/action.h) \
    $(wildcard include/config/start/flush/clean.h) \
    $(wildcard include/config/start/flush/clean/invalidate.h) \
    $(wildcard include/config/start/mmu.h) \
    $(wildcard include/config/job/chain/flag.h) \
    $(wildcard include/config/end/flush/no/action.h) \
    $(wildcard include/config/end/flush/clean.h) \
    $(wildcard include/config/end/flush/clean/invalidate.h) \
    $(wildcard include/config/thread/pri.h) \
    $(wildcard include/config/as/shift.h) \
    $(wildcard include/config/mode/off.h) \
    $(wildcard include/config/mode/manual.h) \
    $(wildcard include/config/mode/tile.h) \
    $(wildcard include/config/limit/external/reads/shift.h) \
    $(wildcard include/config/limit/external/reads.h) \
    $(wildcard include/config/limit/external/reads/octant.h) \
    $(wildcard include/config/limit/external/reads/quarter.h) \
    $(wildcard include/config/limit/external/reads/half.h) \
    $(wildcard include/config/limit/external/writes/shift.h) \
    $(wildcard include/config/limit/external/writes.h) \
    $(wildcard include/config/limit/external/writes/octant.h) \
    $(wildcard include/config/limit/external/writes/quarter.h) \
    $(wildcard include/config/limit/external/writes/half.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_ca.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_policy.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_ca_fixed.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_always_on.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_coarse_demand.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_pm_demand.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mem_lowlevel.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/dma-direction.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
    $(wildcard include/config/need/sg/dma/length.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/scatterlist.h \
    $(wildcard include/config/arm/has/sg/chain.h) \
  include/asm-generic/scatterlist.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/io.h \
  include/asm-generic/iomap.h \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/generic/iomap.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/olegsvs/5/Q37/alps/kernel-3.10/arch/arm64/include/asm/dma-mapping.h \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/asm-generic/dma-mapping-common.h \
  include/linux/kmemcheck.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_defs.h \
    $(wildcard include/config/kds.h) \
    $(wildcard include/config/sync.h) \
    $(wildcard include/config/debug/fs.h) \
    $(wildcard include/config/devfreq.h) \
    $(wildcard include/config/mali/midgard/enable/trace.h) \
    $(wildcard include/config/mali/trace/timeline.h) \
    $(wildcard include/config/have/clk.h) \
    $(wildcard include/config/attr/js/stop/stop/ticks/ss.h) \
    $(wildcard include/config/attr/js/stop/stop/ticks/cl.h) \
    $(wildcard include/config/mali/midgard/rt/pm.h) \
    $(wildcard include/config/devfreq/thermal.h) \
    $(wildcard include/config/mali/power/actor.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/attr/invalid.h) \
    $(wildcard include/config/attr/js/reset/timeout/ms.h) \
    $(wildcard include/config/attr/power/management/callbacks.h) \
    $(wildcard include/config/attr/cpu/speed/func.h) \
    $(wildcard include/config/attr/gpu/speed/func.h) \
    $(wildcard include/config/attr/platform/funcs.h) \
    $(wildcard include/config/attr/power/management/dvfs/freq.h) \
    $(wildcard include/config/attr/pm/gpu/poweroff/tick/ns.h) \
    $(wildcard include/config/attr/pm/poweroff/tick/shader.h) \
    $(wildcard include/config/attr/pm/poweroff/tick/gpu.h) \
    $(wildcard include/config/attr/power/model/callbacks.h) \
    $(wildcard include/config/attr/end.h) \
    $(wildcard include/config/of.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_base_hwconfig.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_base_hwconfig_issues.h \
    $(wildcard include/config/issues/h/.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_base_hwconfig_features.h \
    $(wildcard include/config/features/h/.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mem_lowlevel.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mem_alloc.h \
  include/linux/mempool.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mmu_hw.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android/sync.h \
  /home/olegsvs/5/Q37/alps/kernel-3.10/drivers/staging/android/uapi/sync.h \
  include/linux/debugfs.h \
  include/linux/seq_file.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js_defs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js_policy_cfs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_trace_defs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_trace_timeline.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js_policy.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_debug.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js_ctx_attr.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mem.h \
    $(wildcard include/config/ump.h) \
    $(wildcard include/config/dma/shared/buffer.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_hw.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_security.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_utility.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_gpu_memory_debugfs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_mem_profile_debugfs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_jd_debugfs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_cpuprops.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_gpuprops.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js_affinity.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_gator.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_jm.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_defs.h \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_config_defaults.h \
    $(wildcard include/config/defaults/h/.h) \
  drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/platform/vexpress/mali_kbase_config_platform.h \

drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o: $(deps_drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o)

$(deps_drivers/misc/mediatek/gpu/mt6735/mali-EAC/drivers/gpu/arm/midgard/mali_kbase_js.o):
