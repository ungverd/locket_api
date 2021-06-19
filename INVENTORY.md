# Devices

## Programmers

### Ostranna Alpha

Ostranna Teco v.1. Has some weird additional wire on top.

### Ostranna Beta

Ostranna Teco v.3.

### ST-Link Alpha

## Lockets

### Locket Alpha

Ostranna Locket v5. No battery holder. No case.

### Locket Beta

Ostranna Locket v5. Battery holder firmly attached. No case.

### Locket Gamma

Ostranna Locket v5. In case. Physically damaged (some components are missing),
probably won't work without repairs.

## USB hosts

### USB Host Alpha

Ostranna USB Host v5.

### USB Host Beta

KvantDuino.

# Testing notes.

## Tests.

### OpenOCD test

Steps:
* Connect programmer to the computer.
* Connect device to programmer.
* Turn power on (on Ostranna programmers - press button, on ST-Link ones - no action needed)
* Run `openocd -f interface/stlink.cfg  -f target/stm32l1.cfg`, it should result in `Info : Listening on port 3333 for gdb connections`.
  Potential problems:
  * `Error: target voltage may be too low for reliable debugging`: Have turned power on?

Last known state:
* Locket Alpha + Ostranna Alpha - OK
* Locket Alpha + Ostranna Beta - OK