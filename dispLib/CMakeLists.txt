project(displaylib)

add_library(${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/menu.c
    ${CMAKE_CURRENT_LIST_DIR}/src/notify.c
    ${CMAKE_CURRENT_LIST_DIR}/src/settings.c
    ${CMAKE_CURRENT_LIST_DIR}/src/update.c
    ${CMAKE_CURRENT_LIST_DIR}/src/win.c
    ${CMAKE_CURRENT_LIST_DIR}/src/applications.c
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC Shlwapi winhttp iphlpapi)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DUNICODE)
target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
