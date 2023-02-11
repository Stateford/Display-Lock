target_sources(${PROJECT_NAME} PRIVATE
    app_settings.c
    update_dialog.c
    about_dialog.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
