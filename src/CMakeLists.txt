
enable_language("RC")

add_subdirectory(components)

add_executable(${PROJECT_NAME} WIN32
    main.c
    ui.c
)
target_include_directories(display_lock_components PUBLIC resources)
target_link_libraries(${PROJECT_NAME} PUBLIC display_lock_components)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
    VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
    VERSION_REVISION=${CMAKE_PROJECT_VERSION_PATCH}
)

add_subdirectory(procedures)
add_subdirectory(resources)
