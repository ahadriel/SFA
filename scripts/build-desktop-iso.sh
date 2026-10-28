#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

for path in config/package-lists/hybrid-desktop.list.chroot config/includes.chroot/usr/share/hybrid/README.txt; do
  [[ -f "$path" ]] || { echo "Missing required desktop build input: $path" >&2; exit 1; }
done

# Separate from the small freestanding kernel ISO: this is the practical desktop build.
# Pin the parent distro to avoid inheriting Ubuntu-specific keyring defaults from CI.
sudo lb config \
  --ignore-system-defaults \
  --distribution bookworm \
  --parent-distribution bookworm \
  --architectures amd64 \
  --binary-images iso-hybrid \
  --bootloader grub \
  --debian-installer live \
  --archive-areas "main contrib non-free non-free-firmware" \
  --keyring-packages "debian-archive-keyring" \
  --mirror-bootstrap "http://deb.debian.org/debian" \
  --mirror-chroot "http://deb.debian.org/debian" \
  --mirror-chroot-security "http://security.debian.org/debian-security" \
  --mirror-binary "http://deb.debian.org/debian" \
  --mirror-binary-security "http://security.debian.org/debian-security" \
  --apt-recommends true \
  --linux-flavours amd64 \
  --iso-application "Hybrid OS Desktop" \
  --iso-publisher "Hybrid OS Project" \
  --iso-volume "HYBRID_DESKTOP"

sudo lb build

mkdir -p isobuilds
image="$(find . -maxdepth 1 -type f -name '*.iso' ! -path './isobuilds/*' -printf '%f\n' | head -n 1)"
if [[ -z "$image" ]]; then
  echo "live-build completed but no ISO was found in the repository root." >&2
  find . -maxdepth 3 -type f -name '*.iso' -print
  exit 1
fi
sudo cp "$image" isobuilds/hybrid-desktop-amd64.iso
sudo chown "$(id -u):$(id -g)" isobuilds/hybrid-desktop-amd64.iso
test -s isobuilds/hybrid-desktop-amd64.iso
