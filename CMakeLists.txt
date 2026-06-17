cmake_minimum_required(VERSION 3.20)
project(dryox VERSION 1.2.1 LANGUAGES C)
enable_testing()

add_library(dryoutils SHARED
    src/utils/dryo_mkdirp.c
)
target_include_directories(dryoutils PRIVATE
    include
    vendor
    include/dryox
)
target_compile_options(dryoutils PRIVATE
    -Wall
    -Wextra
    -fPIC
)

add_library(dryox SHARED
    src/dryologging.c
    src/dryoinit.c
)
target_include_directories(dryox PRIVATE
    include
    vendor
    include/dryox
)
target_compile_options(dryox PRIVATE
    -Wall
    -Wextra
    -fPIC
)
target_link_libraries(dryox PRIVATE dryoutils)

add_executable(testall
    src/tests/dryo_mkdirp.c
    src/tests/dryoinit.c
    src/tests/dryologging.c
    src/tests/testall.c
)
target_include_directories(testall PRIVATE
    include
    vendor
    include/dryox
)
target_link_libraries(testall PRIVATE dryox dryoutils)

add_test(NAME testall COMMAND testall)

include(GNUInstallDirs)
install(TARGETS dryox dryoutils
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/dryox
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
