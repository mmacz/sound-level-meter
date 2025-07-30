from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain, CMakeDeps
import os
import subprocess
import sys
import shutil

class SoundLevelMeterConan(ConanFile):
    name = "sound-level-meter"
    version = "1.0.0"
    license = "MIT"
    topics = ("audio", "sound", "meter")
    url = "https://github.com/mmacz/filtering.git"
    author = "mmacz"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [ True, False ],
        "fPIC": [ True, False],
        "fs": [ 16000, 44100, 48000 ],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "fs": 16000,
    }

    def __run_filtergen(self, fs=44100, output_dir = None):
        venv_path = os.path.join(self.build_folder, "build_venv")

        is_windows = os.name == "nt"
        bin_dir = "Scripts" if is_windows else "bin"

        pip_path = os.path.join(venv_path, bin_dir, "pip")
        python_path = os.path.join(venv_path, bin_dir, "python")

        if not os.path.exists(venv_path):
            subprocess.check_call([sys.executable, "-m", "venv", venv_path])
            subprocess.check_call([pip_path, "install", "-r",  os.path.join(self.source_folder, "requirements.txt")])

        if output_dir is None:
            output_dir = os.path.join(self.build_folder, "coeffs")
        os.makedirs(output_dir, exist_ok=True)

        self.output.info(f"Generating filter coefficients in {output_dir}")
        subprocess.check_call([
            python_path,
            os.path.join(self.source_folder, "filtergen.py"),
            "--fs", str(self.options.fs),
            "--output_coeffs", output_dir
        ])

    def layout(self):
        cmake_layout(self, src_folder=".")

    def __symlink_compile_commands(self):
        cc = os.path.join(self.build_folder, "compile_commands.json")
        dst = os.path.join(self.source_folder, "compile_commands.json")

        if os.path.exists(dst) or os.path.islink(dst):
            os.remove(dst)

        try:
            if self.settings.os == "Windows":
                subprocess.check_call(["mklink", "/H", dst, cc], shell=True)
            else:
                os.symlink(cc, dst)
                
        except Exception as e:
            self.output.warning(f"Failed to symlink compile_commands.json: {e}")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20 <4]")
        self.tool_requires("ninja/[>=1.10 <2]")
        if self.settings.get_safe("arch") == "xtensa":
            self.tool_requires("esp-idf/5.1.1")

    def requirements(self):
        if not self.settings.get_safe("arch") == "xtensa":
            self.requires("libsndfile/1.2.2")
            self.requires("cxxopts/3.1.1")

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.preprocessor_definitions["SOUND_LEVEL_METER_FS"] = self.options.get_safe("fs")
        tc.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        if self.settings.get_safe("arch") == "xtensa":
            tc.cache_variables["NATIVE_BUILD"] = "OFF"
        else:
            tc.cache_variables["NATIVE_BUILD"] = "ON"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self);
        cmake.configure()
        self.__symlink_compile_commands()
        self.__run_filtergen(fs=self.options.fs)
        cmake.build()

    def package_info(self):
        pass

    def package(self):
        pass


