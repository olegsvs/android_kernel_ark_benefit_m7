cmd_drivers/input/built-in.o :=  aarch64-ld -EL    -r -o drivers/input/built-in.o drivers/input/input-core.o drivers/input/evdev.o drivers/input/touchscreen/built-in.o drivers/input/misc/built-in.o 
