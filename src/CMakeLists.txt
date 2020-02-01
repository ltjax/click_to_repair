set(TARGET_NAME click_to_repair)

add_executable(${TARGET_NAME} WIN32
  click_to_repair.cpp
  Updater.cpp
  Updater.hpp
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC libonut
    PUBLIC CONAN_PKG::entt
)

target_compile_definitions(${TARGET_NAME}
    PUBLIC NOMINMAX
)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(${TARGET_NAME} 
      PUBLIC dl
  )
endif()