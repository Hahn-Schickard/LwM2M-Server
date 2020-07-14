from conans import ConanFile, CMake, tools
from conans.tools import load
import re
import os


class PackageConan(ConanFile):
    license = "LICENSE"
    url = "https://git.hahn-schickard.de/software-sollutions/application-enginnering/internal/hasll/"
    description = "A Logging library based on SPD Log."
    topics = ("conan", "spdlog", "logging")
    build_requires = "gtest/1.8.1@bincrafters/stable"
    requires = "asio/1.16.0", "HaSLL/0.2.0@hahn-schickard/stable", "nlohmann_json/3.7.3", "pugixml/1.10@bincrafters/stable"
    settings = "cppstd", "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False],
               "fPIC": [True, False]}
    default_options = {"shared": True,
                       "fPIC": True}
    default_user = "Hahn-Schickard"
    exports_sources = "../*", "!../conan/*", "!../build/*", "!../.vscode/*", "!../.gitlab/*", "!../log/*",
    _cmake = None

    def set_name(self):
        current_file_loc = os.path.dirname(os.path.realpath(__file__))
        root_dir = os.path.join(current_file_loc, "..")
        content = load(os.path.join(root_dir, "CMakeLists.txt"))
        name = re.search("set\(THIS (.*)\)", content).group(1)
        self.name = name.strip()

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.verbose = True
        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.names["cmake_find_package_multi"] = self.name
        self.cpp_info.libs = tools.collect_libs(self)
