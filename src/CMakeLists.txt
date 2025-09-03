# CMakeList.txt : CMake project for NES_Emulator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(NES_core
    "Bus.cpp" "Bus.h" 
    "mos6502.cpp" "mos6502.h"
    "opcode.h" "opcode.cpp"
    "opcode_lookup.cpp" 
 "Cartridge.cpp" "Cartridge.h"   "Mapper.h")

add_executable (NES_Emulator "NES_Emulator.cpp" "NES_Emulator.h" )
target_link_libraries(NES_Emulator PRIVATE NES_core)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(NES_Emulator PRIVATE SDL3::SDL3)


find_package(SDL3_ttf CONFIG REQUIRED)
target_link_libraries(NES_Emulator PRIVATE SDL3_ttf::SDL3_ttf)

target_include_directories(NES_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NES_Emulator PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
# Copy font file to output directory after build
set(FONT_FILE "${CMAKE_SOURCE_DIR}/assets/fonts/Ticketing.ttf")
set(FONT_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets/fonts")

set(NESTEST_FILE "${CMAKE_SOURCE_DIR}/assets/ROMs/nestest.nes")
set(NESTEST_FILE_DEST "${CMAKE_CURRENT_BINARY_DIR}/assets/ROMs")

add_custom_command(
    TARGET NES_Emulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${FONT_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FONT_FILE}" "${FONT_DEST_DIR}/Ticketing.ttf"
)

add_custom_command(
    TARGET NES_Emulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${NESTEST_FILE_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NESTEST_FILE}" "${NESTEST_FILE_DEST}/nestest.nes"
)