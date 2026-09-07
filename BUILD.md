# Compiling and flashing

How to build `zsa_voyager_domas_source/`, put it on the board, and go back if it goes wrong. What the firmware does is in `FIRMWARE.md`; the by-hand checks to run after a flash are at the end of that file.

## Build

One command, every time, from this folder:

```
./build.sh
```

It compiles, and when it is running inside WSL (`WSL_DISTRO_NAME` is set) it copies the result to `/mnt/c/Users/DomasPetkevičius/Documents/Projects/zsa_voyager_domas.bin`, where the flasher can open it; see **Flash** below for why. Outside WSL it only compiles. By hand, the same two steps are:

```
~/.venvs/qmk/bin/qmk compile -kb zsa/voyager -km domas
cp ~/projects/keyboard/qmk_firmware/zsa_voyager_domas.bin \
   '/mnt/c/Users/DomasPetkevičius/Documents/Projects/zsa_voyager_domas.bin'
```

The first writes `~/projects/keyboard/qmk_firmware/zsa_voyager_domas.bin`.

`qmk compile` runs from any folder. It finds QMK through `user.qmk_home` in `~/.config/qmk/qmk.ini`. `-kb` names the folder under `keyboards/`, `-km` the folder under `keymaps/`.

A clean build prints `Size after:` and copies the `.bin` out. Errors in `keymap.c` are **not** reported under that name: QMK pulls the file into `quantum/keymap_introspection.c` and compiles it there, so that is the filename in the message.

### Setting up the build, once

QMK's own code does most of the work, so both it and the compiler have to be on the machine.

**QMK's code.** ZSA's version, not the original — the original lacks the modules this `keymap.c` needs.

```
git clone --depth 1 --shallow-submodules --recurse-submodules -b firmware25 \
  https://github.com/zsa/qmk_firmware.git ~/projects/keyboard/qmk_firmware
```

**This layout, placed inside it.** A link, not a copy, so there is only ever one set of files to edit:

```
ln -s ~/projects/keyboard/zsa_voyager_xB6Jx_GGRD4w_domas_source/zsa_voyager_domas_source \
      ~/projects/keyboard/qmk_firmware/keyboards/zsa/voyager/keymaps/domas
```

The link's name, `domas`, is what `-km domas` refers to.

**The compiler.** The Voyager runs an STM32 chip, so it needs a compiler that produces ARM code, plus a small C library for a machine with no operating system. QMK ships the script that installs them; run it from the clone:

```
~/projects/keyboard/qmk_firmware/util/qmk_install.sh -y
```

That is the script `qmk setup` calls. On Ubuntu it runs `util/install/debian.sh`. Two of its steps are expected noise here and neither breaks anything: a warning that WSL cannot reach USB devices, which does not matter because flashing happens through a Windows program, and a closing `pip install --user` that fails on Ubuntu 24.04 with `error: externally-managed-environment`, after apt has already finished and for packages the virtual environment below already holds.

Do not run `qmk setup` itself. It downloads the original QMK into `~/qmk_firmware` and repoints `user.qmk_home` at it, which loses the ZSA setup.

By hand, the same two packages:

```
sudo apt install -y gcc-arm-none-eabi libnewlib-arm-none-eabi
```

`libnewlib-arm-none-eabi` is the C library. Ubuntu lists it only as a recommendation of the compiler, not a dependency, so apt normally installs it but a machine set to skip recommendations will not. Without it the build stops on `fatal error: stdint.h: No such file or directory`, which reads like a broken compiler rather than a missing library. `binutils-arm-none-eabi` needs no mention; the compiler package depends on it.

Do not try Homebrew. It has `arm-none-eabi-gcc` but no `newlib` formula, so its compiler cannot build for a machine with no operating system and fails in exactly that way.

**QMK's command line**, in its own virtual environment so it does not depend on whichever Python comes first on `PATH`:

```
python3 -m venv ~/.venvs/qmk
~/.venvs/qmk/bin/pip install qmk
~/.venvs/qmk/bin/qmk config user.qmk_home=~/projects/keyboard/qmk_firmware
```

## Flash

Zapp does the flashing:

```
/mnt/c/Users/DomasPetkevičius/Documents/Projects/zapp.exe
```

It is a command line program, not a window-based one. Run `zapp.exe --help` to see its two commands, `flash` and `update`. Only `flash` is used here; `update` fetches from Oryx and would overwrite this firmware with generated code.

It runs as a Windows program even when started from WSL, so it reaches the keyboard over USB. WSL's own lack of USB access does not apply.

**A Windows program cannot open a WSL path** such as `/home/domas/...`. That is why the build ends with a copy into a Windows folder. Give Zapp a plain filename and run it from that folder:

```
cd '/mnt/c/Users/DomasPetkevičius/Documents/Projects' && ./zapp.exe flash zsa_voyager_domas.bin
```

### Putting the board into flash mode

Zapp waits for the keyboard to appear in flash mode; it does not put it there. Its messages for this are `Waiting for keyboard in bootloader mode...`, `Failed to detect bootloader` and `Timeout waiting for bootloader`.

This layout has no key that enters flash mode. `QK_BOOT` appears nowhere in `keymap.c`, and was absent from the Oryx original too. So the reset button on the board is the only way in. Find it before flashing, not while the keyboard is dead.

### Going back

A bad flash is recoverable. The chip holds two separate programs: the one that receives new firmware, which ZSA calls Ignition, and the keymap firmware. Flashing replaces the second and never the first, so firmware that crashes on startup cannot stop you writing over it. Zapp names both states in its device list, `Voyager (Ignition STM32)` and `Keyboard in Reset Mode (STM32 DFU)`.

`zsa_voyager_E5AmY.bin` in this repo is ZSA's stock default layout for the Voyager, compiled by Oryx (configure.zsa.io/voyager/layouts/E5AmY/B495Jd), kept for exactly this. It types the stock layout, not this one, so it is for a board that will not run the current build, not for daily use. `zsa_voyager_E5AmY.bin.md5` holds its checksum. That file is the bare value with no filename, so `md5sum -c` cannot read it; compare the two by hand:

```
md5sum zsa_voyager_E5AmY.bin
cat zsa_voyager_E5AmY.bin.md5
```

Both must read `59848dce2b0980a6859d0b4bbacfb2b0`.

To go back, copy it to the Windows folder and flash that instead:

```
cp zsa_voyager_E5AmY.bin '/mnt/c/Users/DomasPetkevičius/Documents/Projects/zsa_voyager_E5AmY_RESTORE.bin'
cd '/mnt/c/Users/DomasPetkevičius/Documents/Projects' && ./zapp.exe flash zsa_voyager_E5AmY_RESTORE.bin
```

Then fix the source here and flash a new build.
