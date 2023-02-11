
enable_language("RC")

add_executable(${PROJECT_NAME} WIN32
    DisplayLock.c
    ui.c
    menu.c
    notify.c
    settings.c
    update.c
    win.c
    applications.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE version Shlwapi winhttp iphlpapi)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DUNICODE)

add_subdirectory(resources)
