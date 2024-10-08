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

        virtual_run_env = VirtualRunEnv(self)
        run_environment = virtual_run_env.environment()

        cmake_binary_dir = os.path.abspath(os.path.normpath(os.path.join(self.build_folder, "..", "..")))
        run_environment.append_path("PATH", os.path.join(cmake_binary_dir, "bin"))

        qt = self.dependencies["qt"]
        if qt:
            run_environment.prepend_path("QML2_IMPORT_PATH", os.path.join(qt.package_folder, "qml"))

        run_environment.vars(self, scope="run").save_script(virtual_run_env._filename)

        if self.settings.os == "Windows":
            run_environment.vars(self, scope="run").save_script(virtual_run_env._filename + ".sh")
            run_environment.vars(self, scope="run").save_script(virtual_run_env._filename + ".ps1")

        doxygen = self.dependencies.build.get("doxygen", None)
        if doxygen:
            doxygen_bin_path = os.path.join(doxygen.package_folder, "bin")
            doxygen_bin_path = doxygen_bin_path.replace("\\", "/")
            find_file_path = os.path.join(self.build_folder, "generators", "FindDoxygen.cmake")
            with open(find_file_path, "w") as f:
                f.write(
                    f"""
# FindDoxygen.cmake
# Locate Doxygen executable
#
#  Doxygen_EXECUTABLE      path to the doxygen executable
#  Doxygen_FOUND           TRUE if Doxygen found

# if (WIN32)
#     find_program(Doxygen_EXECUTABLE NAMES doxygen.exe PATHS {doxygen_bin_path})
# else ()
    find_program(Doxygen_EXECUTABLE NAMES doxygen PATHS {doxygen_bin_path})
# endif ()


if(NOT Doxygen_EXECUTABLE)
    message(FATAL_ERROR "Doxygen not found!")
else()
    set(Doxygen_FOUND TRUE)
    message(STATUS "Doxygen found: {doxygen_bin_path}")
endif()
"""
                )

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

    def build_requirements(self):
        requirements = self.conan_data.get("build_requirements", [])
        for requirement in requirements:
            self.tool_requires(requirement)
