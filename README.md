# decibici — Cowboy e-bike control for Sailfish OS

**decibici** is a native Sailfish OS application for connecting to and controlling a Cowboy e-bike over Bluetooth Low Energy (BLE).

This repository is a fork of [Michal-Szczepaniak/decibici](https://github.com/Michal-Szczepaniak/decibici), originally developed by **Michał Szczepaniak**. The fork is maintained for compatibility testing and improvements on newer Sailfish OS releases by **ExPLIT** ([explit28](https://github.com/explit28)).

## Features

- Discover a previously paired Cowboy bike and connect over BLE.
- Lock and unlock the bike.
- Switch the bike lights on and off.
- Display live bike data, including battery level, battery voltage, speed, distance, and power.

## Compatibility

| Platform | Status |
| --- | --- |
| Sailfish OS 5.2, Jolla C2 (JP-2601), aarch64, Cowboy 2 | **Tested**: Bluetooth connection, lock/unlock, and lights |
| Other Sailfish OS versions and devices | Not verified with this fork |
| Cowboy bikes other than the bike used for testing | Not verified |

The current tested RPM is **1.1-3**. The tests were performed with a **Cowboy 2**. Compatibility with other bike generations or firmware versions is not guaranteed.

## Installation

1. Download the **aarch64 RPM** from [Releases](https://github.com/explit28/decibici/releases) (or the latest successful [GitHub Actions build](https://github.com/explit28/decibici/actions/workflows/sailfish-rpm.yml)).
2. Transfer the RPM to your Sailfish OS device.
3. Install it with the Sailfish package installer, or from a terminal:

   ```sh
   devel-su pkcon install-local ./decibici-1.1-3.aarch64.rpm
   ```

   Run the command from the directory containing the RPM. Installing packages requires administrator privileges.

4. Pair the Cowboy bike in the system's Bluetooth settings if it is not already paired.
5. Open **decibici**, choose **Connect** from the pull-down menu, and wait until the app reports **Device connected**.

The lock and light switches become available once the bike's BLE services are registered. Keep the bike nearby and ensure Bluetooth is enabled.

## What changed in this fork

- Updated discovery of paired devices using the BlueZ ObjectManager, improving compatibility with Sailfish OS 5.1/5.2.
- Improved BLE service initialization, readiness checks, and diagnostic messages.
- Corrected the light-control packet's Modbus CRC-16 calculation.
- Use BLE Write Without Response for the light-control command and enable UART notifications.
- Replaced the former protobuf decoding dependency in the dashboard path with a C++17-compatible decoder.
- Added automated Sailfish OS **5.2 aarch64** RPM builds with GitHub Actions.

The UART notification change was included in the first version for which the light control was confirmed working on the test bike; the exact reason this was needed has not been isolated.

## Building

This is a Qt/QML application using **Sailfish Silica**, **qble**, and a Sailfish OS SDK toolchain.

- The project file is `decibici.pro`.
- The RPM spec is `rpm/decibici.spec`.
- Repository submodules are required; clone with `git clone --recurse-submodules`.
- The [GitHub Actions workflow](https://github.com/explit28/decibici/blob/master/.github/workflows/sailfish-rpm.yml) builds an aarch64 RPM using the Sailfish OS 5.2 SDK container.

To build locally, use a configured Sailfish OS SDK with the appropriate aarch64 target.

## Troubleshooting

If the bike is not found, check that it is paired in the phone's Bluetooth settings, nearby, and not already connected to another device. On Sailfish OS 5.2, the app discovers paired devices through BlueZ.

For additional diagnostics, close any running instance of decibici, then start it from the terminal and save both standard output and error messages to a log file:

```sh
decibici 2>&1 | tee ~/decibici.log
```

The messages remain visible in the terminal and are also written to `~/decibici.log`. Repeating the command replaces the previous log, so copy an earlier log first if you want to keep it. Connect to the bike, test the lock and lights, and exit the application normally before sharing the log.

The output includes device discovery, service registration, light-command writes, and BLE-UART responses. Avoid publicly posting logs containing Bluetooth device addresses without removing identifying details.

## Upstream and contributions

Original project and author: **Michał Szczepaniak** — [Michal-Szczepaniak/decibici](https://github.com/Michal-Szczepaniak/decibici).

**Tests and Modifications by ExPLIT** — [github.com/explit28](https://github.com/explit28).

Changes in this fork may be proposed upstream after further testing. Issues and reproducible logs are welcome.

## License

Licensed under the **GNU General Public License v3 (GPL-3.0)**, as in the original project. See [COPYING](COPYING).
