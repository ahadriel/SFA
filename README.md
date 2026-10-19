# HYBRID OS

**Your machine. Your operating systems. Your choice.**

Hybrid is being developed as its own operating system, starting with a freestanding kernel and a dark, EONIS-inspired text-mode control-center interface. It is not a Debian/Ubuntu live system with a custom theme.

> **Status: early kernel prototype.** The current milestone boots a 32-bit freestanding kernel, draws a dark dashboard in VGA text mode, accepts keyboard input, and runs a small set of commands. This is a styled text-mode interface, not yet a graphical desktop.

## Current interface

The dashboard uses a near-black canvas, cyan/seafoam accents, restrained panel borders, system/component status panels, quick-command hints, and a console input line.

Current commands:

- `help` — show available commands
- `clear` — clear the console message
- `about` — identify the kernel prototype
- `status` — show subsystem status
- `os list` — placeholder for the future OS registry
- `reboot` — request a VM reboot
- `halt` — halt the CPU

Some status values are placeholders. The registry does not discover installed operating systems yet.

## Build

The GitHub Actions workflow builds `isobuilds/hybrid-os-cli.iso` using a freestanding 32-bit kernel and GRUB Multiboot v1. It does not run Debian Live Build or package a Linux userspace.

The workflow also uploads an artifact named `hybrid-os-cli-bios`.

## Run in VirtualBox

- Create a new VM with a 32-bit x86 / Other or Linux profile.
- Allocate 2 GB RAM and 2 CPU cores for a simple test VM.
- Attach `hybrid-os-cli.iso` as the optical disk.
- **Disable EFI** for this initial Legacy BIOS/Multiboot milestone.
- Start the VM and type `help` at the `hybrid>` prompt.

## Roadmap

1. Stabilize and test the dark control-center CLI.
2. Add exception/interrupt handling, timer support, and improved keyboard input.
3. Establish a deliberate graphics/framebuffer boot path for a real graphical interface.
4. Add memory management and a device abstraction layer.
5. Add read-only storage and partition discovery.
6. Add a persistent OS registry and separately tested boot methods.
7. Build an actual graphical desktop, followed by carefully scoped installer/recovery capabilities.

## Safety and limitations

This is not yet a general-purpose OS. Memory management, a filesystem, USB, networking, OS discovery, a graphical desktop, and Windows application compatibility are not implemented. The current prototype does not scan, mount, partition, format, or write to disks.

Registering an OS and booting it are separate capabilities. Every supported operating system will need a tested boot method. Arbitrary ISO boot and Windows application compatibility cannot be assumed.
