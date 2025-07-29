import argparse
import os
from numpy import pi, polymul
from scipy.signal import bilinear, tf2sos

def parse_cli():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--fs", type=int, default=44100,
        help="Sample rate used for calculation. (default: 44100)"
    )
    parser.add_argument(
        "--output_coeffs", type=str, required=True,
        help="Output path for generated C++ header file"
    )
    args = parser.parse_args()
    return args.fs, args.output_coeffs

def A_Weighting(fs):
    f1, f2, f3, f4 = 20.598997, 107.65265, 737.86223, 12194.217
    A1000 = 1.9997

    NUMs = [(2 * pi * f4)**2 * (10**(A1000 / 20)), 0, 0, 0, 0]
    DENs = polymul([1, 4*pi*f4, (2*pi*f4)**2], [1, 4*pi*f1, (2*pi*f1)**2])
    DENs = polymul(polymul(DENs, [1, 2*pi*f3]), [1, 2*pi*f2])
    b, a = bilinear(NUMs, DENs, fs)

    sos = tf2sos(b, a)
    b_list, a_list = [], []
    for stage in sos.tolist():
        b_list.append(stage[0:3])
        a_list.append(stage[3:])
    return b_list, a_list

def C_Weighting(fs):
    f1, f4 = 20.598997, 12194.217
    C1000 = 0.0619

    NUMs = [(2 * pi * f4)**2 * (10**(C1000 / 20.0)), 0, 0]
    DENs = polymul([1, 4*pi*f4, (2*pi*f4)**2], [1, 4*pi*f1, (2*pi*f1)**2])
    b, a = bilinear(NUMs, DENs, fs)

    sos = tf2sos(b, a)
    b_list, a_list = [], []
    for stage in sos.tolist():
        b_list.append(stage[0:3])
        a_list.append(stage[3:])
    return b_list, a_list

def write_combined_header(path: str, filters: dict):
    with open(path, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <array>\n")
        f.write("#include <cstddef>\n\n")

        f.write("struct BiquadCoeffs {\n")
        f.write("    float a0, a1, a2;\n")
        f.write("    float b0, b1, b2;\n")
        f.write("};\n\n")

        for name, (b_stages, a_stages) in filters.items():
            name = name.upper()
            num_stages = len(b_stages)

            f.write(f"// {name}-weighting filter coefficients\n")
            f.write(f"constexpr std::size_t {name}_NUM_STAGES = {num_stages};\n\n")

            f.write(f"constexpr std::array<BiquadCoeffs, {name}_NUM_STAGES> {name}_COEFFS = {{\n")
            f.write("    {\n")
            for b, a in zip(b_stages, a_stages):
                f.write(f"        {{ {a[0]:.8f}f, {a[1]:.8f}f, {a[2]:.8f}f, "
                        f"{b[0]:.8f}f, {b[1]:.8f}f, {b[2]:.8f}f }},\n")
            f.write("    }\n};\n\n")

def main():
    fs, output_path = parse_cli()

    filters = {
        "A": A_Weighting(fs),
        "C": C_Weighting(fs),
    }

    out_file = os.path.join(output_path, "filter_coeffs.h")
    write_combined_header(out_file, filters)

if __name__ == "__main__":
    main()
