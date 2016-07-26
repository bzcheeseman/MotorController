cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME MotorController)
project(${PROJECT_NAME})

find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile_mac @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile_mac
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
endif(DOXYGEN_FOUND)


set(${CMAKE_PROJECT_NAME}_BOARD megaADK)
set(${CMAKE_PROJECT_NAME}_PORT COM8)
set(mega.build.mcu atmega2560)
set(mega.upload.protocol wiring)
set(mega.upload.speed 115200)
LINK_DIRECTORIES(C://Arduino//libraries)

set(${CMAKE_PROJECT_NAME}_SKETCH MotorController.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})

execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/scripts/serialLogging.py ${CMAKE_PROJECT_NAME}_PORT)
