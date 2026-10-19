# HYBRID OS

**Your machine. Your operating systems. Your choice.**

Hybrid OS is an operating-system project with two deliberately separate tracks: a freestanding x86 kernel experiment and a practical desktop live image.

## Downloads

- **Hybrid Desktop Edition** — a usable amd64 XFCE live desktop with Firefox ESR, networking tools, development tools, and Wine compatibility tools. Run the **Build Hybrid Desktop ISO** GitHub Actions workflow and download the `hybrid-desktop-amd64` artifact. See [Desktop documentation](docs/DESKTOP.md).
- **Freestanding Kernel Prototype** — a standalone 32-bit x86 kernel with a custom graphical control-center interface and console, built as `isobuilds/hybrid-os-cli.iso`. See [the kernel roadmap](docs/ROADMAP.md).

## Desktop edition

The desktop edition uses Debian's mature kernel, hardware drivers, package ecosystem, and XFCE desktop so it can provide useful desktop features now. It includes Firefox ESR, NetworkManager, developer tools, Python, Git, filesystem utilities, Wine, and Winetricks.

Wine supports some Windows applications, not all EXE/MSI software. Apps that need Windows kernel drivers, certain anti-cheat systems, DRM, or unsupported dependencies may not run. Hybrid OS is not Windows and does not claim universal compatibility.

## Freestanding kernel prototype

The independent kernel requests an 800x600x32 Multiboot framebuffer, draws its own dark dashboard, and supports keyboard navigation and a small console. It is an early prototype, not yet a general-purpose OS: it lacks a full filesystem, storage and network drivers, user-mode processes, a window manager, and broad hardware support.

## Safety

The desktop live image does not automatically partition, format, or install to disks. Starting an installer is an explicit action; selecting the wrong target can erase data. Back up important files and verify the target disk before confirming installation. The freestanding prototype does not scan or write disks.

## Build

- Kernel ISO: **Build Hybrid OS CLI ISO** workflow; Legacy BIOS/Multiboot.
- Desktop ISO: **Build Hybrid Desktop ISO** workflow; amd64 with BIOS and EFI boot support.

See [docs/DESKTOP.md](docs/DESKTOP.md), [docs/ROADMAP.md](docs/ROADMAP.md), and [docs/SAFETY.md](docs/SAFETY.md).
