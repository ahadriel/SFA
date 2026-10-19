#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

if [[ "$(id -u)" -ne 0 ]]; then
  echo "Run this script as root, for example: sudo ./scripts/build-iso.sh"
  exit 1
fi
if ! command -v lb >/dev/null 2>&1; then
  echo "Missing live-build. On Debian/Ubuntu, install live-build and debootstrap first."
  exit 1
fi

chmod 0755 config/includes.chroot/usr/local/sbin/hybrid

lb config \
  --distribution bookworm \
  --architectures amd64 \
  --binary-images iso-hybrid \
  --bootloader grub \
  --debian-installer false \
  --archive-areas "main contrib non-free-firmware" \
  --iso-application "Hybrid OS" \
  --iso-volume "HYBRID_OS"

lb build
echo
echo "Build finished. Look for a generated .iso in the repository root."
