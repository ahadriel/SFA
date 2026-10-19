# HYBRID OS

**Your machine. Your operating systems. Your choice.**

Hybrid is being developed as its own operating system, starting with a freestanding kernel and a command-line interface. We are not using a Debian/Ubuntu live system as the OS foundation.

> **Status: early kernel prototype.** The first target is a bootable ISO that starts our own kernel and accepts keyboard commands. A graphical desktop comes after the CLI, memory management, and basic hardware foundations.

## Development direction

1. Boot a standalone kernel in VirtualBox.
2. Build an interactive CLI with commands and a predictable shell.
3. Add interrupts, memory management, timers, and a basic device layer.
4. Add storage and a persistent OS-entry registry, with safe read-only discovery first.
5. Implement supported boot handoff methods for registered operating systems.
6. Build a graphical interface on top of the kernel after the core CLI is stable.
7. Expand from the initial BIOS/Legacy prototype to UEFI deliberately.

## Build

The GitHub Actions workflow builds hybrid-os-cli.iso using a freestanding 32-bit kernel build and GRUB. It does not run Debian Live Build or package a Linux userspace.

After the workflow succeeds, open its run in the Actions tab and download the hybrid-os-cli-bios artifact.

### Run in VirtualBox

- Create a new VM with type Linux/Other 32-bit (the kernel is currently a 32-bit Multiboot prototype).
- Attach hybrid-os-cli.iso as the optical disk.
- **Disable EFI** for this initial BIOS/Legacy milestone.
- Start the VM. Type help at the hybrid> prompt.

## Current CLI commands

- help — show available commands
- clear — clear the screen
- about — identify the kernel prototype
- status — show current subsystem status
- os list — placeholder for the future OS registry
- reboot — request a VM reboot
- halt — halt the CPU

## Scope and safety

This is a genuine freestanding kernel prototype, not yet a full general-purpose OS. Memory management, a filesystem, USB, networking, OS discovery, and GUI support are not implemented yet. Registry and boot management will be added in stages. No installer, partitioner, or destructive disk operations are included in this first milestone.

Booting an OS entry and registering its name are separate capabilities. Each OS will need a tested boot method; arbitrary ISO and Windows application compatibility cannot be assumed.
