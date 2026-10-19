#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

echo "Hybrid OS standalone kernel build"
echo "Repository root: $ROOT"

for source in kernel/kernel.c kernel/boot.S kernel/linker.ld iso/boot/grub/grub.cfg; do
  if [[ ! -f "$source" ]]; then
    echo "ERROR: required source file is missing from the checked-out commit: $source"
    echo "Check that the workflow is building the latest commit on main."
    exit 1
  fi
done

for tool in gcc ld grub-file grub-mkrescue xorriso readelf; do
  command -v "$tool" >/dev/null 2>&1 || {
    echo "ERROR: missing build tool: $tool"
    exit 1
  }
done

mkdir -p build iso/boot
gcc -m32 -std=gnu11 -ffreestanding -fno-pie -fno-stack-protector \
  -fno-builtin -fno-asynchronous-unwind-tables -fno-unwind-tables -O2 -Wall -Wextra -Werror -c kernel/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding -fno-pie -c kernel/boot.S -o build/boot.o

# Keep the Multiboot header inside GRUB's first-8-KiB search window.
# GNU ld on hosted runners may otherwise use a very large default page size.
ld -m elf_i386 -z max-page-size=0x1000 -T kernel/linker.ld -nostdlib \
  -o build/hybrid-kernel build/boot.o build/kernel.o

echo "Kernel sections:"
readelf -S build/hybrid-kernel
if ! grub-file --is-x86-multiboot build/hybrid-kernel; then
  echo "ERROR: kernel does not contain a valid Multiboot header in the required location."
  echo "Header bytes near the start of the file:"
  od -An -tx4 -N 8192 build/hybrid-kernel > build/header-dump.txt || true
  head -n 8 build/header-dump.txt || true
  exit 1
fi

cp build/hybrid-kernel iso/boot/hybrid-kernel
grub-mkrescue -o hybrid-os-cli.iso iso

test -s hybrid-os-cli.iso
echo
echo "Build successful: $ROOT/hybrid-os-cli.iso"
echo "Milestone: standalone CLI kernel ISO (Legacy BIOS/Multiboot)."
echo "VirtualBox: disable EFI for this first milestone."
