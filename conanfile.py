# This file is managed by Conan, contents will be overwritten.
# To keep your changes, remove these comment lines, but the plugin won't be able to modify your requirements

import os
from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain
from conan.tools.env import VirtualRunEnv


class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

        virtual_build_env = VirtualRunEnv(self)
        env = virtual_build_env.environment()
        cmake_binary_dir = os.path.abspath(os.path.normpath(os.path.join(self.build_folder, "..", "..")))
        env.append_path("PATH", os.path.join(cmake_binary_dir, "bin"))
        env.vars(self, scope="run").save_script(virtual_build_env._filename)
        env.vars(self, scope="run").save_script(virtual_build_env._filename + ".sh")
        env.vars(self, scope="run").save_script(virtual_build_env._filename + ".ps1")

    def configure(self):
        qt_options = self.conan_data["options"][0]["qt"]

        for option in qt_options:
            for option_name, option_value in option.items():
                if option_name == "opengl" and self.settings.os != "Windows":
                    option_value = "desktop"
                setattr(self.options["qt"], option_name, option_value)
                self.output.info(f"Set Qt option '{option_name}' to {option_value}")

    def requirements(self):
        requirements = self.conan_data.get("requirements", [])
        for requirement in requirements:
            self.requires(requirement)
