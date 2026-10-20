# Hybrid OS development roadmap

Hybrid OS is a freestanding 32-bit x86 kernel booted through GRUB Multiboot v1 in Legacy BIOS mode. A successful compile is not treated as proof that hardware functionality works.

## Milestone 1 — repeatable kernel build (complete)
- Compile a freestanding kernel with warnings treated as errors.
- Verify the Multiboot v1 header and ELF target.
- Generate a bootable ISO and publish a GitHub Actions artifact.
- Prevent overlapping build jobs from losing the generated ISO update.

## Milestone 2 — graphical control center and diagnostics (in progress)
- Framebuffer dashboard with keyboard navigation.
- Keyboard console and reboot/halt.
- Add useful CPU and boot-memory diagnostics.
- Run a read-only legacy ATA/IDE disk probe at startup and on demand (`S`).
- Count MBR primary partitions when a valid MBR signature is present; do not imply GPT or filesystem discovery.
- Keep unsupported systems and tools clearly marked as unavailable.

## Milestone 3 — kernel reliability
- Review the Global Descriptor Table; add an Interrupt Descriptor Table, exception handlers and panic diagnostics.
- Add PIC remapping, timer interrupts and a buffered keyboard driver.
- Add memory-map validation and a physical page-frame allocator.
- Add serial logging and emulator-based boot smoke tests.

## Milestone 4 — device and storage foundations
- Introduce explicit device abstractions.
- Enumerate PCI devices without modifying storage.
- Add block-device drivers only after interrupt, memory and error-handling foundations are tested.
- Start with read-only partition and filesystem inspection.

## Milestone 5 — filesystem and persistence
- Implement a small, tested filesystem path before writing user data.
- Add a persistent OS registry with versioning, integrity checks and recovery behavior.
- Separate registry entries from verified boot methods.

## Milestone 6 — multi-OS boot manager
- Add explicit, read-only inventory of disks and partitions.
- Implement individual boot methods only after each has repeatable tests.
- Removing a registry entry must never delete an OS or alter its partition.
- Never assume device names such as /dev/sda remain stable.

## Milestone 7 — desktop and applications
- Add a process model, user/kernel boundaries, system calls, scheduling and virtual memory.
- Add a user-space runtime, compositor/window manager, input stack and graphics drivers.
- Evaluate networking, audio, USB and package management independently.
- Treat Windows EXE/MSI compatibility as a separate research project, not a promise.

## Milestone 8 — installer and deployment hardening
- Identify target disks by model, capacity and stable identifiers.
- Require clear target selection, multiple confirmations and backups before destructive actions.
- Never use a development host disk as an implicit target.
- Run boot tests in QEMU/VirtualBox and publish checksums and release notes.

## Non-negotiable safety rules
- Read-only first for all storage discovery.
- No automatic partitioning, formatting, wiping or installation.
- A registry entry is not proof that an OS is bootable.
- No universal compatibility claims.
- Every milestone must distinguish implemented features from planned ones.
