cmake_minimum_required(VERSION 3.0)

project(bth VERSION 1.2 LANGUAGES C)

add_executable(${PROJECT_NAME}-${PROJECT_VERSION} "${CMAKE_SOURCE_DIR}/bth.c")
target_compile_definitions(${PROJECT_NAME}-${PROJECT_VERSION} PUBLIC VERSION="${PROJECT_VERSION}")
install(
    TARGETS ${PROJECT_NAME}-${PROJECT_VERSION}
    RUNTIME DESTINATION bin
)
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \${CMAKE_INSTALL_PREFIX}/bin/bth-${PROJECT_VERSION} \${CMAKE_INSTALL_PREFIX}/bin/bth)")

if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake")
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake
        COMMENT "Uninstalling ${PROJECT_NAME}-${PROJECT_VERSION}"
    )
endif()
