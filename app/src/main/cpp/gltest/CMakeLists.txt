cmake_minimum_required(VERSION 3.4.1)

# Submodule library
add_library(gltest
            STATIC

            # Source
            gltest.c)

# Linking
target_link_libraries(gltest

                      # NDK libraries
                      log
                      GLESv2)
