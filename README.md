# HYBRID OS

**Your machine. Your operating systems. Your choice.**

Hybrid is a dynamically managed multi-OS boot environment. The machine boots into Hybrid first; from there, users can select a registered operating system or add/remove supported systems later without reinstalling the whole environment.

> **Status: early prototype.** This repository is the start of the project, not yet a production-ready operating system or a bootable ISO.

## Vision

```text
UEFI firmware
    ↓
Hybrid bootloader (GRUB-based prototype)
    ↓
Hybrid OS selector / recovery shell
    ├── Boot a registered operating system
    ├── Discover supported installations
    ├── Register a new OS
    └── Remove an OS registration safely
```

## Design goals

- **Dynamic registry:** operating systems can be registered after initial setup.
- **Persistent control partition:** Hybrid configuration lives separately from guest OS installations.
- **Safe-by-default management:** removing a registry entry must not erase an OS or partition.
- **UEFI-first:** start with a reproducible VirtualBox UEFI target, then add other firmware modes deliberately.
- **Honest boot support:** chainloading and direct ISO boot are different features. Not every ISO can be booted directly from disk; some require extraction, loopback support, or installation to a dedicated partition.
- **Small steps:** build, test, and document each stage before attempting destructive disk operations.

## Initial development target

- VirtualBox VM with UEFI enabled
- A dedicated 25 GiB virtual disk for Hybrid
- Existing 500 GiB development disk kept separate
- GRUB as the initial bootloader
- A minimal Linux-based userspace for the management shell

**Important:** do not run installer or partitioning commands against a real disk until its device identity has been verified. Device names such as `/dev/sda` and `/dev/sdb` can change between boots.

## Planned milestones

1. [x] Establish project vision and safety rules.
2. [ ] Create a reproducible build environment.
3. [ ] Build a bootable UEFI prototype ISO.
4. [ ] Display a Hybrid shell/menu after boot.
5. [ ] Add a persistent OS registry on the dedicated Hybrid disk.
6. [ ] Implement safe `list`, `add`, `remove`, and `boot` operations.
7. [ ] Test booting a supported installed Linux system.
8. [ ] Add OS detection, recovery tools, and more boot methods.
9. [ ] Automate ISO builds with GitHub Actions.

## Current limitation

The first prototype will manage **boot entries**; it will not magically make every arbitrary `.iso`, `.exe`, or operating-system image bootable. Each operating system needs a supported boot method. Windows installation and direct booting of arbitrary ISOs will be treated as separate compatibility milestones.

## Contributing / testing

See [docs/ROADMAP.md](docs/ROADMAP.md) and [docs/SAFETY.md](docs/SAFETY.md) before testing disk-related features.
