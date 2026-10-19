# Disk safety rules

Hybrid is experimental. These rules are mandatory.

1. **Never assume device names.** `/dev/sda` and `/dev/sdb` can swap when VM disks or USB devices change.
2. **Do not partition automatically.** The prototype must not run `fdisk`, `parted`, `mkfs`, `wipefs`, or `dd` against a disk.
3. **Keep development disks separate.** The intended 25 GiB Hybrid disk and 500 GiB development disk must be identified by size, model, and UUID before any future installation step.
4. **Registry removal is not OS removal.** The `remove` command removes a saved boot entry only. It must not delete files, partitions, or operating systems.
5. **Read-only first.** Initial disk discovery uses `lsblk`, `blkid`, and filesystem metadata only.
6. **Back up important data.** A VM snapshot is recommended before testing bootloader changes.
7. **No claims of universal compatibility.** An ISO may require installation, extraction, a specific bootloader, or hardware drivers. Registry support does not guarantee boot support.
8. **Make risky actions explicit.** Future disk-writing features must show the exact target and require a separate confirmation before proceeding.
