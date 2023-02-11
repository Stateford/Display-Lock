
enable_language("RC")

add_subdirectory(components)

add_executable(${PROJECT_NAME} WIN32
    main.c
    ui.c
)
target_include_directories(display_lock_components PUBLIC resources)
target_link_libraries(${PROJECT_NAME} PUBLIC display_lock_components)

add_subdirectory(procedures)
add_subdirectory(resources)
