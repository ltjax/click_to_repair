

add_executable(click_to_repair WIN32
  click_to_repair.cpp)

target_link_libraries(click_to_repair 
    PUBLIC libonut
)
