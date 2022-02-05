
build/os15.bin: build/boot.bin build/kernel.bin
	cat $^ > $@
	dd if=/dev/zero of=$@ bs=1 count=0 seek=16384 status=none
	cp build/os15.bin build/os15bochs.bin
	dd if=/dev/zero of=build/os15bochs.bin bs=1 count=0 seek=10321920 status=none

build/kernel.bin: link.ld build/entry.o build_kernel build_programs
	ld -m elf_i386 -o $@ -T link.ld build/entry.o $(wildcard build/kernel/*.o $(wildcard build/programs/*.o)) --oformat binary

build/entry.o: entry.asm |build
	nasm $< -f elf -o $@

build_kernel: $(wildcard kernel/*.c) $(wildcard kernel/*.h) |build
	@for f in $(basename $(notdir $(wildcard kernel/*.c))); do gcc -Wall -Wextra -m32 -ffreestanding -fno-pie -c -o build/kernel/$${f}.o kernel/$${f}.c; done;

build_programs: $(wildcard programs/*.c) $(wildcard programs/*.h) |build
	@for f in $(basename $(notdir $(wildcard programs/*.c))); do gcc -Wall -Wextra -m32 -ffreestanding -fno-pie -c -o build/programs/$${f}.o programs/$${f}.c; done;

build/boot.bin: boot.asm |build
	nasm $< -f bin -o $@

run: build/os15.bin
	qemu-system-x86_64 -drive file=$<,format=raw

build:
	mkdir -p build/kernel
	mkdir -p build/programs

clean:
	rm -r build
