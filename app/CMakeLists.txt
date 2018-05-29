cmake_minimum_required(VERSION 3.4.1)

# Defines
# set(TEXGZ_USE_JPEG true)

# Optional libraries
if(TEXGZ_USE_JPEG)
    set(LIBS_JPEG
        libmyjpeg)
endif()

# Compiler options
add_compile_options(-DA3D_GLESv2)

# Main library
add_library(GearsES2eclair
            SHARED

            # Source
            src/main/cpp/android_jni.c
            src/main/cpp/gear.c
            src/main/cpp/gears_renderer.c
            src/main/cpp/gears_overlay.c
            src/main/cpp/gears_layerHud.c)

# Submodules
add_subdirectory("src/main/cpp/a3d")
add_subdirectory("src/main/cpp/libpak")
add_subdirectory("src/main/cpp/texgz")
add_subdirectory("src/main/cpp/libexpat/expat/lib")

# Linking
target_link_libraries(GearsES2eclair

                      # Submodule libraries
                      ${LIBS_JPEG}
                      a3d
                      expat
                      pak
                      texgz

                      # NDK libraries
                      z
                      log
                      GLESv2)
