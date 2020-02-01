from conans import ConanFile, CMake

class ClickToRepairConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def requirements(self):
        if self.settings.os == "Linux":
            # Please install glew and SDL2 from system libraries
            # e.g. 
            #   sudo apt-get install libglew-dev libsdl2-dev
            self.requires("assimp/5.0.1", "entt/3.2.2" )
        else:
            self.requires("glew/2.1.0@bincrafters/stable","sdl2/2.0.10@bincrafters/stable","assimp/5.0.1", "entt/3.2.2")
