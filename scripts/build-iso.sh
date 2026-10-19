#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "\${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

for tool in gcc ld grub-file grub-mkrescue xorriso; do
  command -v "$tool" >/dev/null 2>&1 || {
    echo "Missing build tool: $tool"
    exit 1
  }
done

mkdir -p build iso/boot
gcc -m32 -std=gnu11 -ffreestanding -fno-pie -fno-stack-protector \
  -fno-builtin -O2 -Wall -Wextra -Werror -c kernel/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding -fno-pie -c kernel/boot.S -o build/boot.o
ld -m elf_i386 -T kernel/linker.ld -nostdlib -o build/hybrid-kernel build/boot.o build/kernel.o

if ! grub-file --is-x86-multiboot build/hybrid-kernel; then
  echo "Kernel does not contain a valid Multiboot header."
  exit 1
fi

cp build/hybrid-kernel iso/boot/hybrid-kernel
grub-mkrescue -o hybrid-os-cli.iso iso

echo
echo "Build successful: $ROOT/hybrid-os-cli.iso"
echo "This first milestone is a standalone BIOS/Legacy bootable CLI kernel."
echo "In VirtualBox, create a VM with EFI disabled and boot this ISO."
