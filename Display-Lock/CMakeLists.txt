add_executable(${PROJECT_NAME} WIN32 DisplayLock.c ui.c)
target_link_libraries(${PROJECT_NAME} PRIVATE displaylib version)

add_subdirectory(resources)
