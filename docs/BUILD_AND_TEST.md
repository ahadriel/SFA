# Build, boot and test Hybrid OS

## CI checks
Every relevant push compiles the freestanding 32-bit kernel with warnings treated as errors, verifies the Multiboot v1 header, creates the BIOS ISO, and starts it in QEMU. The smoke test expects the kernel to print HYBRID: kernel initialised over COM1 serial. QEMU is stopped after a timeout because this is an interactive OS.

## VirtualBox
1. Create a compatible 32-bit VM.
2. Allocate 2 CPU cores and 128 MB–2 GB RAM.
3. Attach isobuilds/hybrid-os-cli.iso as an optical disk.
4. Disable EFI for this Legacy BIOS build.
5. Use keys 1–4 for pages, C for console, H for Home, R to reboot, and Esc to close the console.

## Current scope
The kernel has an IDT, exception/IRQ entry stubs, legacy PIC remapping, a 100 Hz PIT tick, IRQ-driven keyboard scancode buffering, and serial boot diagnostics. It does not yet provide a filesystem, disk drivers, networking, mouse, user mode/processes, paging, a heap, a package manager, or Windows EXE/MSI compatibility. The Systems page remains a non-destructive placeholder. Do not install this experimental ISO to a physical disk.
