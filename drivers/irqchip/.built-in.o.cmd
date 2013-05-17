cmd_drivers/irqchip/built-in.o :=  aarch64-ld -EL    -r -o drivers/irqchip/built-in.o drivers/irqchip/irqchip.o drivers/irqchip/irq-gic.o drivers/irqchip/irq-mt-gic.o drivers/irqchip/irq-mt-eic.o 
