HYBRID OS — DESKTOP EDITION
===========================

Welcome! This is the usable desktop edition of Hybrid OS, built on Debian's
hardware support and package ecosystem with the XFCE desktop.

WHAT WORKS
- Graphical desktop, windows, panels, file manager, terminal and settings.
- Networking through NetworkManager (hardware/driver support permitting).
- Firefox ESR web browser.
- Standard Linux applications and development tools.
- A package manager; install supported Linux software using apt.
- Wine and Winetricks are included as a compatibility layer for some Windows
  applications. Not every EXE/MSI works, and drivers, anti-cheat, DRM and some
  modern apps may not work.
- Filesystem tools are included. Be careful when mounting disks.

LIVE SESSION AND INSTALLATION
The live system runs from the ISO and memory. Changes may be lost at shutdown.
Use the installer from the desktop/menu only if you intentionally want to
install the OS to a disk. Carefully verify the target disk and back up data
first. Selecting the wrong disk can erase its contents. This project does not
silently partition or install to any disk.

UPDATES
For an installed system, use the graphical package manager or:
  sudo apt update
  sudo apt upgrade

The freestanding kernel experiment remains a separate build target in this
repository. This desktop edition is the practical daily-use environment; it
does not pretend that the freestanding prototype already has Linux-level
hardware drivers or application compatibility.
