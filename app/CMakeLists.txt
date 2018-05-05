cmake_minimum_required(VERSION 3.4.1)

# Defines
# set(TEXGZ_USE_JPEG true)

# Optional libraries
if(TEXGZ_USE_JPEG)
    set(LIBS_JPEG
        libmyjpeg)
endif()

# Main library
add_library(GearsES2eclair
            SHARED

            # Source
            src/main/cpp/android_jni.c)

# Submodules
add_subdirectory("src/main/cpp/gltest")
add_subdirectory("src/main/cpp/texgz")

# Linking
target_link_libraries(GearsES2eclair

                      # Submodule libraries
                      ${LIBS_JPEG}
                      gltest
                      texgz

                      # NDK libraries
                      z
                      log
                      GLESv2)
