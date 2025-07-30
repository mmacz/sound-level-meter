# Sound Level Meter (SLM) – DSP Library for Audio Level Analysis
<p align="center">
  <img src="assets/slm_logo.png" alt="SLM Logo" width="200"/>
</p>

<h1 align="center">Sound Level Meter (SLM)</h1>
<p align="center">
  <em>Modular DSP library for audio level estimation, with A/C/Z filtering and SPL computation.</em>
</p>

---

## 🎯 Project Goals

- ✅ Reusable, embeddable **DSP library** (filters, SPL estimation)
- ✅ Implements **IEC 61672-1**: A/C/Z frequency weightings, Fast/Slow/Impulse time averaging
- ✅ Optional CLI tool for WAV analysis
- ✅ Cross-platform builds via **CMake + Conan**
- 🚀 ESP32 firmware support planned (I2S mic + recording noise above -55dB Leq/SPL to be decided)

---

## 📦 Project Structure

```
.
├── src/
│   ├── slm/           # Core DSP logic (filters, SPL estimation)
│   ├── filtering/     # Auto-generated filter coefficients
│   └── native_app/    # Minimal CLI app for desktop testing
├── conanfile.py       # Dependency declarations
├── filtergen.py       # Auto-executed during build (do not run manually)
└── CMakeLists.txt     # Top-level build system
```

---

## 🚀 Quickstart (Desktop)

```bash
git clone https://github.com/mmacz/sound-level-meter.git
cd sound-level-meter
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
./bin/slm input.wav --freq a --time fast --ref-level 2e-5
```

> The CLI tool is intended for validation and testing only — the core library is the primary product.

---

## 📚 Library Features

- A / C / Z frequency weighting filters (based on IEC 61672-1)
- Time weighting approximations: Fast (125 ms), Slow (1 s), Impulse (35 ms)
- Block-based sample processing (default: 512 frames)
- Configurable reference pressure level (e.g., `2e-5` = 94 dB SPL)
- Portable design, with minimal heap usage  
  (runtime reconfiguration handled via `std::unique_ptr`)
- Suitable for embedded systems with floating-point support

---

## 📐 Standards

This library is based on:

> **IEC 61672-1:2013**  
> *Electroacoustics – Sound level meters – Part 1: Specifications*

Implemented aspects:

- Frequency weightings A, C, Z
- Time averaging approximations for Fast, Slow, and Impulse
- SPL estimation using filtered amplitude and reference calibration

> ⚠️ This is not a certified SLM. Use it for prototyping, firmware development, or signal analysis — not for legal compliance.

---

## 📏 SPL Estimation

The SPL is computed by filtering each audio block and evaluating its peak (absolute) amplitude relative to a reference level:

```text
SPL = 20 × log₁₀(amp / reference_level)
```

Where:
- `amp` – peak filtered sample amplitude (not RMS)
- `reference_level` – float value corresponding to 94 dB SPL (default: `2e-5`)

This maps digital audio amplitudes to real-world sound levels.

| SPL (dB) | Float amplitude | Description         |
|----------|------------------|----------------------|
| 94       | 0.00002 (2e-5)   | Calibration tone     |
| 114      | ~0.002           | Loud conversation    |
| 134      | ~0.02            | Heavy machinery      |
| 188      | 1.0              | Full-scale input     |

You can override the reference via:

```bash
--ref-level 0.00002
```

---

## 🔧 Filter Design

All filter coefficients for A/C/Z weightings are **generated automatically during build** via `filtergen.py`. No manual action is required — this is handled by the CMake/Conan build process.

---

## 🧪 Testing

The CLI tool allows quick validation of the processing pipeline:

```bash
./bin/slm_reference test_signal.wav --freq a --time fast --ref-level 2e-5
```

Output includes SPL estimation and configuration summary.  
Test WAVs can be generated using tools like `sox`, `audacity`, or scripting.

---

## 🛠️ Embedded Support

The library in `src/slm/` is portable across:

- ESP32 with `esp-idf` + Conan
- FreeRTOS or bare-metal targets
- Linux/macOS/Windows for host-side testing

Planned features:
- ESP32 firmware with I2S mic input
- Storing recorded audio data in remote server
- Offline logging to SD card / flash

---

## 🤝 Contributions

Contributions welcome — especially for:

- Real-time time-weighting accuracy
- Fixed-point or CMSIS-DSP adaptation
- ESP32 firmware layer
- CI pipelines and waveform-based testing

Feel free to open issues or PRs with ideas or improvements.

---

## 📖 License

MIT — see [LICENSE](LICENSE)

---
