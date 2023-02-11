
enable_language("RC")

add_subdirectory(components)

add_executable(${PROJECT_NAME} WIN32
    main.c
    ui.c
)
target_link_libraries(${PROJECT_NAME} PUBLIC display_lock_components)
target_include_directories(display_lock_components PUBLIC resources)

add_subdirectory(resources)
