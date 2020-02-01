

add_executable(click_to_repair WIN32
  click_to_repair.cpp)

target_link_libraries(click_to_repair 
    PUBLIC libonut
    PUBLIC CONAN_PKG::entt
)

target_compile_definitions(click_to_repair
    PUBLIC NOMINMAX)

set_property(TARGET click_to_repair PROPERTY CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(click_to_repair 
      PUBLIC dl
  )
endif()