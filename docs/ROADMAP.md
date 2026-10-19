# Hybrid OS roadmap

## Phase 0 — repository and build foundation
- Define the boot flow, safety constraints, and supported target.
- Keep the development VM's 25 GiB Hybrid disk separate from the 500 GiB development disk.
- Build artifacts in CI so the user does not have to manually assemble the ISO.

## Phase 1 — bootable prototype
- Build a Debian-based live ISO with UEFI GRUB support.
- Boot into a small Hybrid command-line manager.
- Show a friendly status banner and available commands.
- Provide a read-only disk/partition inventory command.

## Phase 2 — persistent registry
- Store registrations on a clearly identified Hybrid data partition, mounted by filesystem UUID.
- Provide `list`, `add`, and `remove` for registry records.
- Removing an entry must only remove the record, never format or delete a partition.
- Validate names and paths; quote all shell variables.

## Phase 3 — boot methods
- Support Linux installations with a discovered kernel and initramfs, or chainload a distro-provided bootloader when appropriate.
- Detect unsupported cases and explain what is missing.
- Treat Windows, removable media, and direct ISO boot as separate boot methods with separate tests.

## Phase 4 — installation and recovery
- Add an explicit, opt-in installer for the dedicated Hybrid disk.
- Require multiple checks of the target disk identity and show disk size/model before any write.
- Provide recovery media and documented rollback.

## Definition of done for the first prototype
- ISO builds reproducibly in GitHub Actions.
- ISO boots in a fresh VirtualBox VM with EFI enabled.
- User reaches the Hybrid shell without needing an existing Lubuntu installation.
- Disk inventory is read-only.
- No disk is formatted or modified automatically.
