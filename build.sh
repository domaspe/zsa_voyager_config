#!/usr/bin/env sh
# Compile the layout and, when running inside WSL, copy the result where the
# Windows flasher can open it. Zapp cannot read /home/..., see BUILD.md.
set -eu

qmk=~/.venvs/qmk/bin/qmk
bin=~/projects/keyboard/qmk_firmware/zsa_voyager_domas.bin
windows_dir='/mnt/c/Users/DomasPetkevičius/Documents/Projects'

"$qmk" compile -kb zsa/voyager -km domas

if [ -n "${WSL_DISTRO_NAME:-}" ]; then
    cp "$bin" "$windows_dir/"
    echo "Copied to $windows_dir/$(basename "$bin")"
fi
