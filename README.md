# HYBRID OS

**Your machine. Your operating systems. Your choice.**

Hybrid is an independent operating-system experiment: a freestanding x86 kernel with a dark, EONIS-inspired control-center GUI. It is not a Debian/Ubuntu live system.

> **Status: early kernel prototype.** The kernel requests an 800x600x32 Multiboot framebuffer and draws a graphical dashboard using its own tiny bitmap font and rectangle renderer. It includes keyboard navigation and a basic console. If a compatible framebuffer is unavailable, a text-mode fallback is shown.

## GUI controls

- `1` — Home
- `2` — Systems (safe placeholder)
- `3` — Tools
- `4` — About
- `C` — Open console
- `H` — Return to Home
- `R` — Reboot
- In the console: `help`, `status`, `about`, `version`, `theme`, `os list`, `clear`, `reboot`, `halt`
- `Esc` — Close console

The UI is keyboard-driven; mouse input and a window manager are not implemented.

## Safety and limitations

This is not yet a general-purpose OS. The kernel has no filesystem, storage driver, networking, USB stack, full memory management, mouse support, or Windows application compatibility. The Systems page is a mock-up of planned functionality and does not discover installed operating systems.

The prototype does not scan, mount, partition, format, or write to disks. OS registration and booting are separate capabilities; each supported OS needs a tested boot method. Arbitrary ISO boot and Windows application compatibility cannot be assumed.

## Build and VirtualBox

GitHub Actions builds `isobuilds/hybrid-os-cli.iso` and uploads an artifact named `hybrid-os-cli-bios`. Use a 32-bit x86 / Other or Linux VM, allocate 2 GB RAM and 2 CPU cores, attach the ISO, and **disable EFI** for this Legacy BIOS/Multiboot milestone.
