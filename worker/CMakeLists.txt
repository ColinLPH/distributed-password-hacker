cmake_minimum_required(VERSION 3.26)

project(distributed-password-hacker
        VERSION 1
        DESCRIPTION ""
        LANGUAGES C)

set(CMAKE_C_STANDARD 17)

set(SANITIZE TRUE)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(SOURCE_LIST ${SOURCE_DIR}/main.c
        ${SOURCE_DIR}/encrypt_pwd.c
        ${SOURCE_DIR}/parse_args.c
        ${SOURCE_DIR}/worker.c
        ${SOURCE_DIR}/networking.c
)
set(HEADER_LIST ${INCLUDE_DIR}/worker.h
        ${INCLUDE_DIR}/parse_args.h
        ${INCLUDE_DIR}/options.h
        ${INCLUDE_DIR}/encrypt_pwd.h
        ${INCLUDE_DIR}/networking.h

)
include_directories(${INCLUDE_DIR})

# Find OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(distributed-password-hacker ${SOURCE_MAIN} ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(distributed-password-hacker PRIVATE include)
target_include_directories(distributed-password-hacker PRIVATE /usr/local/include)
target_link_directories(distributed-password-hacker PRIVATE /usr/local/lib)
target_link_libraries(distributed-password-hacker OpenSSL::SSL OpenSSL::Crypto)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(distributed-password-hacker PRIVATE /usr/include)
endif ()

set_target_properties(distributed-password-hacker PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})


# Add compiler flags
target_compile_options(distributed-password-hacker PRIVATE
        -Wall              # Enable all compiler warnings
        -Wextra            # Enable extra compiler warnings
        -pedantic          # Enable pedantic mode
        -O2                # Optimization level 2
        -g                 # Generate debug information
        -fPIC              # Generate position-independent code
        # Add more flags as needed
)

set_target_properties(distributed-password-hacker PROPERTIES OUTPUT_NAME "worker")
install(TARGETS distributed-password-hacker DESTINATION bin)

