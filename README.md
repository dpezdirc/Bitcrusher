# Bitcrusher

**Bitcrusher** is a *Biscuit-style* bitcrusher audio plugin inspired by the classic hardware unit of the same name.  
Instead of using a traditional bit-depth reduction control, Bitcrusher gives you direct access to individual bits of the signal, allowing for precise and often unpredictable digital degradation.

## Overview

Bitcrusher works by converting the incoming audio signal into an **8-bit representation**. Each of the 8 bits can then be individually enabled or disabled via a user-controlled bit mask. This mask is applied to every sample using a selectable **bitwise operation**.

The more bits that are disabled (or manipulated) the more the output signal is degraded, producing anything from subtle digital grit to extreme lo-fi destruction.

## How It Works

1. Incoming audio samples are converted to an **8-bit integer representation**.
2. The user defines the state of each bit (on/off) using the UI.
3. These states are converted into an **8-bit mask**.
4. The mask is applied to each sample using one of two operations:
   - **Bitwise AND**
   - **Bitwise XOR**
5. The processed signal is converted back to audio output.

## Bitwise Operations

### AND
- Preserves only the bits that are enabled in both the input sample and the mask.
- Typically results in predictable, stepped distortion.
- Lower bits being disabled introduces a gating effect (lower amplitude signals get muted)

### XOR
- Flips bits where the mask bit is set to `1`.
- Can introduce aggressive, non-linear artifacts.
- Especially sensitive to changes in higher-order bits.

> ⚠️ **Important:**  
> The XOR operation can produce extremely loud and harsh outputs when higher-order (MSB) bits are toggled. To prevent excessive noise and potential speaker damage, the **first three most significant bits are disabled** and have no effect in XOR mode.

## Processing Example

Below is an example illustrating how a single 8-bit input sample is processed.

| Bit          | 1 (MSB) | 2 | 3 | 4 | 5 | 6 | 7 | 8 (LSB) |
|--------------|--:|--:|--:|--:|--:|--:|--:|--:|
| Input Sample | 0 | 1 | 0 | 1 | 1 | 0 | 1 | 0 |
| Bit Mask     | 0 | 0 | 1 | 1 | 0 | 1 | 0 | 1 |
| AND Output   | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |
| XOR Output   | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 |

Note how the same mask produces dramatically different results depending on the selected operation.

## Controls

### Bit Mask
- 8 toggle buttons representing each bit (MSB → LSB)
- Defines the bit mask applied to all processed samples

### Operation
- Dropdown selector:
  - **AND**
  - **XOR**
- Determines how the bit mask interacts with the input signal

### Wet/Dry Mix
- Blends processed signal with the original input
- **TODO: Not yet implemented**

---

## Installation

### Prebuilt Binary (Windows Only)

Currently, prebuilt binaries are provided **for Windows only**.

1. Download the latest **VST3** binary from the **Releases** page.
2. Copy the `.vst3` file to: `C:\Program Files\Common Files\VST3`
3. Rescan plugins in your DAW if necessary.

### macOS / Linux

Prebuilt binaries are not currently available for macOS or Linux.

To use Stutter on these platforms, you must build the plugin from source.

## Building from Source

JUCE is included with the respository as a git submodule, therefore building the plugin is possible without an existing JUCE installation.

### Prerequisites

- **Projucer** executable
  - The JUCE repository does **not** contain a prebuilt Projucer executable; you must build from source or download it separately in a JUCE release.
- A supported IDE (e.g. Visual Studio or Xcode)

### Steps

1. Clone the repository **with submodules**:

   - ```git clone --recurse-submodules https://github.com/dpezdirc/Bitcrusher.git```

2. Open the `.jucer` file in **Projucer**.
   - The project is preconfigured to reference the JUCE modules from the included submodule directory; no modification of the `.jucer` file is required.

4. Generate the project files for your platform (e.g. Visual Studio / Xcode).

5. Build the plugin in your IDE of choice.

---

## License

This project is licensed under the **GNU Affero General Public License v3.0 (AGPLv3)**.  
See the `LICENSE` file for full license terms.

This plugin is built using the **JUCE** framework, which is included in this repository as a git submodule.  
JUCE is dual-licensed under the **AGPLv3** and a commercial license. This project uses JUCE under the terms of the **AGPLv3**, in accordance with JUCE’s licensing requirements.  
See the JUCE submodule and its accompanying license file for detailed information.
