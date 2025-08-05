# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mariem/esp-idf/components/bootloader/subproject"
  "/home/mariem/test_encoder/build/bootloader"
  "/home/mariem/test_encoder/build/bootloader-prefix"
  "/home/mariem/test_encoder/build/bootloader-prefix/tmp"
  "/home/mariem/test_encoder/build/bootloader-prefix/src/bootloader-stamp"
  "/home/mariem/test_encoder/build/bootloader-prefix/src"
  "/home/mariem/test_encoder/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mariem/test_encoder/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mariem/test_encoder/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
