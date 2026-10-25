# drone-drop-targeting

Targeting core for a drone-based munition release system. Given a scenario —
release height, wind, and a list of targets — it picks the best shell and
aim point, simulates the drop with wind drift, and writes the outcome to a
file.

This is the computational core of a bigger system that's planned to grow
around it: batch runs over many scenarios, a network service, and an
operator console on top.

## Input: `scenario.txt`

```
<height>
<wind_x> <wind_y>
<targets_count>
<x> <y> <health> <type>
...repeated targets_count times
```

- `height` — release altitude, meters.
- `wind` — wind vector (x, y), m/s.
- Each target: position, health, and type. Type is one of `PERSONNEL`,
  `LOGISTICS`, `APC`, `TANK`, `RADAR` — each worth a fixed point value.

## Output: `result.txt`

```
<impact_x> <impact_y>
<release_x> <release_y>
<shell_name>
<destroyed> <value> <price> <net>
```

- `impact` — the point the shell actually lands on.
- `release` — where the drone needs to be when it drops the shell, to land
  on `impact` given the wind drift.
- `shell_name`, then the outcome: targets destroyed, their total value,
  shell price, and net score (value − price).

## Example

`scenario.txt`:
```
150.0
3.0 4.0
4
0.0 0.0 50 APC
20.0 10.0 50 APC
-15.0 20.0 45 RADAR
300.0 300.0 15 PERSONNEL
```

`result.txt`:
```
0.00 0.00
-16.59 -22.12
HE
3 170 20 150
```

## Build & run

```
g++ -std=c++17 -O2 -o drop main.cpp
./drop
```

Reads `scenario.txt` and writes `result.txt` in the working directory.
