# Hybrid OS roadmap

Hybrid OS is an independent freestanding x86 kernel booted by GRUB Multiboot v1 in Legacy BIOS mode.

## Phase 1 — graphical control center (current)
- Request an 800x600x32 Multiboot framebuffer.
- Draw the EONIS-inspired dark dashboard with a built-in bitmap font.
- Provide keyboard navigation and a basic command console.
- Keep disk operations disabled and retain a text-mode fallback.

## Phase 2 — kernel foundations
- Add CPU exception handling, an interrupt descriptor table and timer support.
- Improve keyboard input and add mouse support.
- Establish memory-map and physical-memory management.
- Add a device abstraction layer and diagnostics.

## Phase 3 — storage and read-only discovery
- Add storage drivers and filesystem support after kernel foundations.
- Begin with read-only disk and partition inventory.
- Never mount, format, repartition, or write automatically.

## Phase 4 — persistent OS registry and boot manager
- Store registry metadata on an explicitly identified data partition.
- Separate registration from actual booting; test each OS boot method.
- Removing an entry must never delete or format a partition.

## Phase 5 — installer, recovery, and broader compatibility
- Use explicit target selection, clear disk identity and multiple confirmations.
- Never use the development host's disk as an implicit target.
- Expand drivers incrementally and research Windows application compatibility without promising arbitrary EXE/MSI support.

## Current milestone
- Build a freestanding ISO in GitHub Actions.
- Boot with EFI disabled.
- Show the graphical control center when a compatible framebuffer is available.
- Support keyboard navigation and console commands.
- Never scan, mount, or modify disks in this prototype.
