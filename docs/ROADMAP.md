# Hybrid OS roadmap

Hybrid OS is an independent freestanding operating-system project. The current target is a 32-bit x86 kernel booted by GRUB Multiboot v1 in Legacy BIOS mode. The current dashboard is VGA text mode; it is not a graphical desktop yet.

## Phase 0 — repository and build foundation
- Keep source code and build scripts in Git.
- Build the ISO reproducibly with GitHub Actions.
- Verify the generated ISO and publish both a repository copy and a workflow artifact.
- Keep development disks isolated and use disposable virtual machines for experiments.

## Phase 1 — dark control-center CLI (current)
- Boot the freestanding kernel in VirtualBox.
- Present a dark, EONIS-inspired control-center layout using the available VGA text mode.
- Use a near-black background, cyan/seafoam accents, concise status panels, and a command input line.
- Keep all disk operations disabled.
- Test all available commands and keyboard input in the VM.

## Phase 2 — kernel foundations
- Add CPU exception handling and a carefully tested interrupt descriptor table.
- Add timer support and improve keyboard input.
- Establish a memory-map and physical memory management design.
- Add a device abstraction layer and improve diagnostics.
- Keep the build freestanding and avoid assuming hosted operating-system services exist.

## Phase 3 — graphics path and desktop
- Choose and document a supported framebuffer/graphics boot path.
- Validate resolution, pixel format, framebuffer bounds, and failure handling before drawing.
- Introduce a graphics renderer, font support, and a small UI component system.
- Port the dark Hybrid visual language into a real graphical shell before adding desktop applications.
- Keep the text-mode interface as a fallback while graphics support matures.

## Phase 4 — storage and read-only discovery
- Add storage drivers and filesystem support only after the required kernel foundations exist.
- Start with read-only disk and partition inventory.
- Clearly identify devices and partitions; never assume device numbering is stable.
- Do not mount, format, repartition, or write to disks automatically.

## Phase 5 — persistent OS registry
- Store OS registration metadata on a clearly identified Hybrid data partition, using a stable identifier such as a filesystem UUID.
- Provide list, add, and remove operations for registry records.
- Removing a registry entry must only remove the record, never format or delete a partition.
- Validate names, paths, and boot metadata.

## Phase 6 — boot manager
- Implement and test a separate boot method for each supported OS family/configuration.
- Consider Linux kernel plus initramfs booting or chainloading an existing bootloader where appropriate.
- Treat Windows, removable media, and direct ISO boot as distinct features requiring distinct tests.
- Explain unsupported configurations rather than guessing.

## Phase 7 — installer and recovery
- Design an explicit, opt-in installer for a clearly selected target disk.
- Show the disk model, size, and identity before any write.
- Require multiple deliberate confirmations before destructive operations.
- Provide recovery media and documented rollback.
- Never use the development host's disk as an implicit target.

## Phase 8 — broader hardware and application compatibility
- Expand drivers and hardware support incrementally.
- Research a technically viable strategy for Windows application compatibility, without promising arbitrary EXE/MSI support.
- Keep third-party components and their license obligations documented.

## Definition of done for the current milestone
- GitHub Actions builds the freestanding kernel ISO reproducibly.
- The ISO boots in a fresh VirtualBox VM with EFI disabled.
- The dark text-mode control center appears.
- Keyboard input and the documented commands work.
- The status screen clearly labels unimplemented components.
- No disk is scanned, mounted, or modified by the prototype.
