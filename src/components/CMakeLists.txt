project(display_lock_components)

add_library(${PROJECT_NAME} STATIC
    menu.c
    notify.c
    settings.c
    update.c
    win.c
    applications.c
)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DUNICODE)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC version Shlwapi winhttp iphlpapi)
