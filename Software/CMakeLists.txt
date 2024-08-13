# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:\\Program Files\\Raspberry Pi\\Pico SDK v1.5.1\\pico-sdk")
set(PICO_EXTRAS_PATH "C:\\Users\\Kurosaki\\Documents\\Pico-v1.5.1\\pico-extras")


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

project(OOO_RFID C CXX ASM)

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/board)
set(PICO_BOARD "pico")

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(OOO_RFID)

pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=2)

pico_set_boot_stage2(OOO_RFID slower_boot2)

#pico_generate_pio_header(OOO_RFID ${CMAKE_CURRENT_LIST_DIR}/lcddata.pio)

#pico_generate_pio_header(OOO_RFID ${CMAKE_CURRENT_LIST_DIR}/vinfsm.pio)

target_sources(OOO_RFID PRIVATE
        core1.cpp
        main.cpp
        rfid_rc522/rc522.c
        #lcd.c
        #vin.c
        )

pico_set_program_name(OOO_RFID "OOO_RFID")
pico_set_program_version(OOO_RFID "0.4")

pico_enable_stdio_uart(OOO_RFID 0)
pico_enable_stdio_usb(OOO_RFID 1)

target_include_directories(OOO_RFID PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add the standard library to the build
target_link_libraries(OOO_RFID pico_stdlib pico_multicore hardware_dma hardware_spi pico_time hardware_pwm hardware_gpio)

pico_add_extra_outputs(OOO_RFID)

add_compile_options(-Ofast)
