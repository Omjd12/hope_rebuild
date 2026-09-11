# hope_rebuild

A ground-up rewrite of **Hope**, a heading-controlled differential-drive robot built on ESP32. This version restructures the original Hope firmware into clean, reusable C++ classes — MPU6050 fusion, PID heading control, motor driving, and Wi-Fi/TCP telemetry — and adds a Python-side live plotter for tuning.

## Overview

The robot uses an MPU6050 (via its onboard DMP) to estimate yaw, and a PID controller to hold or steer toward a target heading by differentially driving two motors through an H-bridge. Yaw and timing data are streamed live over a raw TCP socket so they can be visualized on a computer while tuning.

## Hardware

- ESP32 dev board
- MPU6050 (IMU with onboard Digital Motion Processor)
- Dual H-bridge motor driver (2 motors, 4 direction pins + 2 PWM pins)
- Differential-drive chassis

**Default pin mapping** (see `hope_rebuild.ino`):
| Signal | Pin |
|---|---|
| Motor A dir (a1, a2) | 26, 27 |
| Motor B dir (b1, b2) | 25, 33 |
| PWM 1, PWM 2 | 13, 32 |

## Firmware structure

| File | Responsibility |
|---|---|
| `hope_rebuild.ino` | Main sketch: setup/calibration and the control loop |
| `Angle.h` | `mpu` class — wraps `MPU6050_6Axis_MotionApps20`, handles DMP init, yaw extraction, gyro-rate reading, and startup calibration/offset removal |
| `Control.h` | `pid` class — a standard PID computed on `(error, dt)`, output constrained to ±155 (PWM-safe range) |
| `Motor.h` | `motor` class — direction + PWM control for a differential drive (`forward`, `backward`, `left`, `right`, `turn`) |
| `TCP.h` | `tcp` class — connects to Wi-Fi and runs a `WiFiServer` on port `1234` that streams `angle,time` pairs to any connected client |
| `Web_controller.h` | Web-based control interface/handlers for driving the robot over Wi-Fi |
| `plot.py` | PyQtGraph client that connects to the ESP32's TCP server and plots live yaw vs. time for debugging/tuning |
| `.theia/` | Eclipse Theia IDE workspace config |

## How it works

1. **Setup**: Motor PWM pins are configured, Wi-Fi connects (`tcp::connect`), the TCP server starts, and the MPU6050 + DMP are initialized.
2. **Calibration**: `mpu::caliberate()` warms up the DMP, then samples ~1000 readings to compute a yaw offset and gyro-rate offset, canceling out drift/bias before the loop starts.
3. **Control loop** (`loop()` in `hope_rebuild.ino`):
   - Reads a new DMP packet when available (throttled to ~100 Hz via a 10 ms `dt` check).
   - Computes heading `error = angle - target`.
   - Feeds `error` and `dt` into the PID (`pid::compute`) to get a motor correction `output`.
   - If the heading error is outside a ±5° deadband, the robot turns in place (`motor::turn`) toward the target; otherwise it drives forward while applying the PID correction differentially to each side (`motor::forward`).
   - Streams the current angle and timestamp to any connected TCP client.

## Getting started

### Firmware
1. Open `hope_rebuild.ino` in the Arduino IDE (or the included Theia workspace) with ESP32 board support installed.
2. Install the Arduino libraries: `I2Cdevlib` (`I2Cdev`, `MPU6050_6Axis_MotionApps20`), and the built-in `WiFi.h`.
3. Update the Wi-Fi SSID/password passed to `tcp server(...)` in `hope_rebuild.ino`.
4. Wire the MPU6050 over I2C and the motor driver to the pins listed above.
5. Flash the sketch. On boot it will connect to Wi-Fi, print its IP over Serial, calibrate the IMU, and start the control loop.

### Live plotting (optional)
1. `pip install pyqtgraph pyqt5`
2. In `plot.py`, set `host` to the ESP32's IP address (printed over Serial on boot).
3. Run `python plot.py` to see live yaw vs. time while the robot is running — useful for tuning the PID gains in `Control.h`.

## Tuning

PID gains are set where the controller is constructed in `hope_rebuild.ino`:
```cpp
pid p(40, 0., 1.3); // Kp, Ki, Kd
```
Adjust these and re-flash to tune heading-hold/turning behavior. The `target` variable sets the desired heading in degrees.

## Status / notes

- This is an active robotics project; some code paths (e.g. `backward()`) are present but not yet wired into the main loop.
- No license file is currently included.
