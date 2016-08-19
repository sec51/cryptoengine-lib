string(ASCII 27 ESC)
cmake_minimum_required(VERSION 2.8.12)
project (CryptoEngine)

add_library(${PROJECT_NAME} STATIC $<TARGET_OBJECTS:CryptoEngineLib>)

## The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 1)
set (${PROJECT_NAME}_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

# Set the hostname
site_name(HOSTNAME)


# This is necessary to create  an archive with all the static libs this project use
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE "Debug")
endif()

macro(WARNING_LOG MESSAGE)
    message("-- ${ESC}[31m${MESSAGE}${ESC}[m")
endmacro(WARNING_LOG)

macro(LOG MESSAGE)
    message("-- ${MESSAGE}")
endmacro(LOG)

## Compiler Options
add_compile_options(
        -Wall
        -Wextra
        -Wstrict-aliasing
        -Wno-unused-parameter
        -Wno-unused-result
        -Wno-missing-field-initializers
        -Wno-sign-compare
        -Wnon-virtual-dtor
        -Wchar-subscripts
        -Wpointer-arith
        -Woverloaded-virtual
        -Wformat
        -Wformat-security
        -Werror=format-security
        -fstack-protector-all
        -fPIE
        -fpie
        -fPIC
        -fpic
        -pipe
)

# External libraries

################################
# Google Test
################################
option(GTEST_ENABLED "GTEST_ENABLED" ON)
if(GTEST_ENABLED)
    add_subdirectory (${PROJECT_SOURCE_DIR}/third-party/gtest-1.7.0)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    LOG("GTEST: ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}")
endif()

################################
# Google Log
################################
# include_directories("${PROJECT_SOURCE_DIR}/third-party/glog-0.3.4")

# BoringSSL or OpenSSL
option(BORINGSSL_ENABLED "BORINGSSL_ENABLED" ON)
if(BORINGSSL_ENABLED)
    set(BoringSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/third-party/boringssl")
    LOG("BoringSSL ROOT in ${BoringSSL_ROOT_DIR}")
    find_package(BoringSSL REQUIRED)
    if(BORINGSSL_FOUND)
        LOG("BoringSSL Crypto FOUND in ${BoringSSL_CRYPTO_LIBRARY}")
        LOG("BoringSSL Headers FOUND in ${BoringSSL_INCLUDE_DIR}")
        include_directories(BEFORE SYSTEM ${BoringSSL_INCLUDE_DIR})
        target_include_directories(CryptoEngine BEFORE PUBLIC ${BoringSSL_INCLUDE_DIR})
        target_link_libraries (CryptoEngine ${BoringSSL_CRYPTO_LIBRARY})
    endif()
endif()

### LibreSSL or OpenSSL
option(LIBRESSL_ENABLED "LIBRESSL_ENABLED" OFF)
if(LIBRESSL_ENABLED)
    # Apple has deprecated OpenSSL, static link with brew bottled libressl
    if(APPLE)
        if(LIBRESSL_ENABLED)
            if(NOT DEFINED ENV{BUILD_LINK_SHARED})
                set(CMAKE_FIND_LIBRARY_SUFFIXES .a .dylib .so)
            endif()
            execute_process(COMMAND brew --prefix OUTPUT_VARIABLE BREW_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
            set(OPENSSL_ROOT_DIR "${BREW_PREFIX}/opt/libressl")
        else()
            # Some installs may find the optional brew /usr/local/lib/ as the root.
            set(OPENSSL_ROOT_DIR "/usr")
        endif()
    endif()
    find_package(OpenSSL REQUIRED)
    include_directories("BEFORE ${OPENSSL_INCLUDE_DIR}")
    target_include_directories (CryptoEngine BEFORE PUBLIC ${OPENSSL_INCLUDE_DIR})
    target_link_libraries (CryptoEngine ${OPENSSL_LIBRARIES})
endif()


# LibNaCl - set it to third party so we can cross compile.
LOG("CPU Architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
#set(LibNaCl_ROOT_DIR "${PROJECT_SOURCE_DIR}/third-party/nacl-20110221/build/${HOSTNAME}")
find_package(LibNaCl)
if(LIBNACL_FOUND)
    LOG("LibNaCl ROOT set to: ${LibNaCl_ROOT_DIR}")
    LOG("LibNaCl FOUND in ${LibNaCl_LIBRARY}")
    LOG("LibNaCl Headers FOUND in ${LibNaCl_INCLUDE_DIR}")
    include_directories(${LibNaCl_INCLUDE_DIR})
    target_include_directories(CryptoEngine BEFORE PUBLIC ${LibNaCl_INCLUDE_DIR})
    if(LibNaCl_RANDOM)
        LOG("LibNaCl Random FOUND in ${LibNaCl_RANDOM}")
        target_link_libraries (CryptoEngine ${LibNaCl_LIBRARY} ${LibNaCl_RANDOM} dl)
    else()
        target_link_libraries (CryptoEngine ${LibNaCl_LIBRARY} dl)
    endif()
else()
    set(LibNaCl_ROOT_DIR "${PROJECT_SOURCE_DIR}/third-party/nacl-20110221/build/${HOSTNAME}")
    LOG("LibNaCl root: ${LibNaCl_ROOT_DIR}")
    find_package(LibNaCl)
    if(LIBNACL_FOUND)
        LOG("LibNaCl ROOT set to: ${LibNaCl_ROOT_DIR}")
        LOG("LibNaCl FOUND in ${LibNaCl_LIBRARY}")
        LOG("LibNaCl Headers FOUND in ${LibNaCl_INCLUDE_DIR}")
        include_directories(${LibNaCl_INCLUDE_DIR})
        target_include_directories(CryptoEngine BEFORE PUBLIC ${LibNaCl_INCLUDE_DIR})
        LOG("LibNaCl Random FOUND in ${LibNaCl_RANDOM}")
        if(LibNaCl_RANDOM)
            LOG("LibNaCl Random FOUND in ${LibNaCl_RANDOM}")
            target_link_libraries (CryptoEngine ${LibNaCl_LIBRARY} ${LibNaCl_RANDOM} dl)
        else()
            LOG("Could not find randombytes.o")
            target_link_libraries (CryptoEngine ${LibNaCl_LIBRARY} dl)
        endif()
    endif()
endif()

add_subdirectory (lib)
add_subdirectory (tests)

include_directories(CryptoEngine BEFORE PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(CryptoEngine BEFORE PUBLIC ${PROJECT_SOURCE_DIR}/lib)

# ==============================
add_test (NAME CryptoEngineTest COMMAND ./tests/CryptoEngineTest)
enable_testing()

install (TARGETS CryptoEngine DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE})
install (FILES
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine.h
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine_encrypted_message.h
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine_message.h
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine_file_utils.h
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine_hkdf_utils.h
        ${PROJECT_SOURCE_DIR}/lib/cryptoengine_conversions.h
        ${PROJECT_SOURCE_DIR}/lib/verification_engine.h
        DESTINATION include)