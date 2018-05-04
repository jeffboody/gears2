cmake_minimum_required(VERSION 3.4.1)

# Main library
add_library(GearsES2eclair
            SHARED

            # Source
            src/main/cpp/android_jni.c)

# Submodules
add_subdirectory("src/main/cpp/gltest")

# Linking
target_link_libraries(GearsES2eclair

                      # Submodule libraries
                      gltest

                      # NDK libraries
                      log
                      GLESv2)
