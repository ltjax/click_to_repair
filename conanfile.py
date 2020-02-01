from conans import ConanFile, CMake

class ClickToRepairConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def requirements(self):
        requirements = []
        if self.settings.os == "Linux":
            # Please install glew and SDL2 from system libraries
            # e.g. 
            #   sudo apt-get install libglew-dev libsdl2-dev
            requirements = ["assimp/5.0.1","entt/3.2.2"]
        else:
            requirements = ["assimp/5.0.1", "entt/3.2.2",
                "glew/2.1.0@bincrafters/stable","sdl2/2.0.10@bincrafters/stable"]

        for x in requirements:
            self.requires(x)
