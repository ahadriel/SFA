# Hybrid Desktop Edition

Hybrid has two deliberately separate deliverables:

1. **Freestanding kernel ISO** (`hybrid-os-cli.iso`): the independent x86 kernel and its early control-center prototype.
2. **Hybrid Desktop ISO** (`hybrid-desktop-amd64.iso`): a practical amd64 live desktop using Debian's tested kernel, drivers, XFCE, Firefox ESR, development tools, and Wine compatibility tools.

The desktop edition exists to deliver usable desktop functionality while the independent kernel matures. These are intentionally not represented as the same kernel or implementation.

## Build and download

GitHub Actions workflow **Build Hybrid Desktop ISO** builds the desktop image. Run it from Actions → Build Hybrid Desktop ISO → Run workflow. The artifact is named `hybrid-desktop-amd64` and is retained for 14 days. Desktop images are large, so they are uploaded as workflow artifacts rather than committed to Git history.

## Run in VirtualBox

- Create a Linux/Debian (64-bit) VM.
- Allocate at least 4 GB RAM (8 GB recommended), 2 CPU cores (4 recommended), and 30 GB virtual disk if you plan to install.
- The workflow verifies the generated ISO's El Torito boot metadata; check the run output before assuming a particular VM firmware mode is supported.
- Attach the downloaded ISO as a virtual optical disk and boot.
- Choose the live desktop. The installer is an explicit user-initiated action.

## Included capabilities

XFCE desktop, file manager, terminal, settings, Firefox ESR, NetworkManager, development tools, Python, Git, filesystem utilities, Wine and Winetricks. The **Hybrid OS Control Center** provides a graphical, read-only disk/partition inventory, local OS registry, and ISO-path inventory. It does not install systems or modify boot configuration.

Wine can run **some** Windows applications, not all. EXE/MSI compatibility varies by app, architecture, required drivers, DRM, and other dependencies. This is not Windows and does not promise universal Windows application support.

## Storage safety

The live image includes filesystem utilities, but it does not automatically install, repartition, or format a disk. Installing is destructive if the wrong target is selected; back up data and inspect the target carefully before confirming any installation.
