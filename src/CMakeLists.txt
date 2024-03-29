cmake_minimum_required(VERSION 3.10)

project(my-time C CXX ASM)

set(NRF_PROJECT_NAME my-time)
set(NRF_BOARD pca10040)

if (DEFINED ARM_NONE_EABI_TOOLCHAIN_PATH)
    set(ARM_NONE_EABI_TOOLCHAIN_BIN_PATH ${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin)
endif ()

set(OPENOCD_BIN_PATH "openocd.exe")

set(NRFUTIL "nrfutil")

# Setup .toolchain
include(${CMAKE_SOURCE_DIR}/cmake-nRF5x/arm-gcc-toolchain.cmake)

if (NOT DEFINED ARM_GCC_TOOLCHAIN)
    message(FATAL_ERROR "The toolchain must be set up before calling this macro")
endif ()

set(NRFX_SOURCE_FILES
    libs/nRF52/cores/nRF5/nordic/nrfx/mdk/system_nrf52.c
    libs/nRF52/cores/nRF5/linker/gcc_startup_nrf52.S

    libs/nRF52/libraries/Adafruit_LittleFS/src/Adafruit_LittleFS.cpp
    libs/nRF52/libraries/Adafruit_LittleFS/src/Adafruit_LittleFS_File.cpp
    libs/nRF52/libraries/Adafruit_LittleFS/src/littlefs/lfs.c
    libs/nRF52/libraries/Adafruit_LittleFS/src/littlefs/lfs_util.c
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEAdvertising.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLECharacteristic.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEClientCharacteristic.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEClientService.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEConnection.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEDiscovery.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEGatt.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEPeriph.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLESecurity.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEService.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/BLEUuid.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/bluefruit.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/utility/AdaMsg.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/utility/bonding.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/utility/bootloader_util.c
    libs/nRF52/libraries/Bluefruit52Lib/src/services/BLEUart.cpp
    libs/nRF52/libraries/Bluefruit52Lib/src/services/BLEDfu.cpp
    libs/nRF52/libraries/InternalFileSytem/src/flash/flash_cache.c
    libs/nRF52/libraries/InternalFileSytem/src/flash/flash_nrf5x.c
    libs/nRF52/libraries/InternalFileSytem/src/InternalFileSystem.cpp
    libs/nRF52/libraries/SPI/SPI.cpp
    libs/nRF52/libraries/SPI/SPI_nrf52832.cpp
    #libs/nRF52/libraries/Wire/Wire_nRF52.cpp
    libs/nRF52/cores/nRF5/freertos/portable/CMSIS/nrf52/port_cmsis.c
    libs/nRF52/cores/nRF5/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c
    libs/nRF52/cores/nRF5/freertos/portable/GCC/nrf52/port.c
    libs/nRF52/cores/nRF5/freertos/Source/croutine.c
    libs/nRF52/cores/nRF5/freertos/Source/event_groups.c
    libs/nRF52/cores/nRF5/freertos/Source/list.c
    libs/nRF52/cores/nRF5/freertos/Source/portable/MemMang/heap_3.c
    libs/nRF52/cores/nRF5/freertos/Source/queue.c
    libs/nRF52/cores/nRF5/freertos/Source/stream_buffer.c
    libs/nRF52/cores/nRF5/freertos/Source/tasks.c
    libs/nRF52/cores/nRF5/freertos/Source/timers.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_gpiote.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_power.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_pwm.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_qspi.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_spim.c
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/nrfx_timer.c
    libs/nRF52/cores/nRF5/sysview/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c
    libs/nRF52/cores/nRF5/sysview/SEGGER/SEGGER_RTT.c
    libs/nRF52/cores/nRF5/sysview/SEGGER/SEGGER_RTT_printf.c
    libs/nRF52/cores/nRF5/sysview/SEGGER/SEGGER_SYSVIEW.c
    libs/nRF52/cores/nRF5/sysview/SEGGER_SYSVIEW_FreeRTOS.c
    libs/nRF52/cores/nRF5/utility/AdaCallback.c
    libs/nRF52/cores/nRF5/utility/adafruit_fifo.cpp
    libs/nRF52/cores/nRF5/utility/debug.cpp
    libs/nRF52/cores/nRF5/utility/SoftwareTimer.cpp
    libs/nRF52/cores/nRF5/utility/utilities.c
    libs/nRF52/cores/nRF5/abi.cpp
    libs/nRF52/cores/nRF5/delay.c
    libs/nRF52/cores/nRF5/variant.cpp
    libs/nRF52/cores/nRF5/HardwarePWM.cpp
    libs/nRF52/cores/nRF5/hooks.c
    libs/nRF52/cores/nRF5/IPAddress.cpp
    libs/nRF52/cores/nRF5/itoa.c
    libs/nRF52/cores/nRF5/new.cpp
    libs/nRF52/cores/nRF5/Print.cpp
    libs/nRF52/cores/nRF5/pulse.c
    libs/nRF52/cores/nRF5/RingBuffer.cpp
    libs/nRF52/cores/nRF5/rtos.cpp
    libs/nRF52/cores/nRF5/Stream.cpp
    libs/nRF52/cores/nRF5/WInterrupts.c
    libs/nRF52/cores/nRF5/wiring.c
    libs/nRF52/cores/nRF5/wiring_analog.cpp
    libs/nRF52/cores/nRF5/wiring_analog_nRF52.c
    libs/nRF52/cores/nRF5/wiring_digital.c
    libs/nRF52/cores/nRF5/wiring_private.c
    libs/nRF52/cores/nRF5/wiring_shift.c
    libs/nRF52/cores/nRF5/WMath.cpp
    libs/nRF52/cores/nRF5/WString.cpp
)

set(LVGL_SRC
    libs/lvgl/src/core/lv_disp.c
    libs/lvgl/src/core/lv_group.c
    libs/lvgl/src/core/lv_indev.c
    libs/lvgl/src/core/lv_indev_scroll.c
    libs/lvgl/src/core/lv_obj.c
    libs/lvgl/src/core/lv_obj_class.c
    libs/lvgl/src/core/lv_obj_draw.c
    libs/lvgl/src/core/lv_obj_pos.c
    libs/lvgl/src/core/lv_obj_scroll.c
    libs/lvgl/src/core/lv_obj_style.c
    libs/lvgl/src/core/lv_obj_style_gen.c
    libs/lvgl/src/core/lv_obj_tree.c
    libs/lvgl/src/core/lv_event.c
    libs/lvgl/src/core/lv_refr.c
    libs/lvgl/src/core/lv_theme.c
    libs/lvgl/src/draw/lv_draw_arc.c
    libs/lvgl/src/draw/lv_draw_blend.c
    libs/lvgl/src/draw/lv_draw_img.c
    libs/lvgl/src/draw/lv_draw_label.c
    libs/lvgl/src/draw/lv_draw_line.c
    libs/lvgl/src/draw/lv_draw_mask.c
    libs/lvgl/src/draw/lv_draw_rect.c
    libs/lvgl/src/draw/lv_draw_triangle.c
    libs/lvgl/src/draw/lv_img_buf.c
    libs/lvgl/src/draw/lv_img_cache.c
    libs/lvgl/src/draw/lv_img_decoder.c
    libs/lvgl/src/font/lv_font.c
    libs/lvgl/src/font/lv_font_fmt_txt.c
    libs/lvgl/src/font/lv_font_loader.c
    libs/lvgl/src/font/lv_font_montserrat_24.c
    libs/lvgl/src/font/lv_font_unscii_8.c
    libs/lvgl/src/font/lv_font_unscii_16.c
    libs/lvgl/src/hal/lv_hal_disp.c
    libs/lvgl/src/hal/lv_hal_indev.c
    libs/lvgl/src/hal/lv_hal_tick.c
    libs/lvgl/src/misc/lv_anim.c
    libs/lvgl/src/misc/lv_area.c
    libs/lvgl/src/misc/lv_async.c
    libs/lvgl/src/misc/lv_bidi.c
    libs/lvgl/src/misc/lv_color.c
    libs/lvgl/src/misc/lv_fs.c
    libs/lvgl/src/misc/lv_gc.c
    libs/lvgl/src/misc/lv_ll.c
    libs/lvgl/src/misc/lv_log.c
    libs/lvgl/src/misc/lv_math.c
    libs/lvgl/src/misc/lv_mem.c
    libs/lvgl/src/misc/lv_printf.c
    libs/lvgl/src/misc/lv_style.c
    libs/lvgl/src/misc/lv_style_gen.c
    libs/lvgl/src/misc/lv_timer.c
    libs/lvgl/src/misc/lv_tlsf.c
    libs/lvgl/src/misc/lv_txt.c
    libs/lvgl/src/misc/lv_txt_ap.c
    libs/lvgl/src/misc/lv_utils.c
    libs/lvgl/src/widgets/lv_arc.c
    libs/lvgl/src/widgets/lv_bar.c
    libs/lvgl/src/widgets/lv_btn.c
    libs/lvgl/src/widgets/lv_btnmatrix.c
    libs/lvgl/src/widgets/lv_canvas.c
    libs/lvgl/src/widgets/lv_checkbox.c
    libs/lvgl/src/widgets/lv_dropdown.c
    libs/lvgl/src/widgets/lv_img.c
    libs/lvgl/src/widgets/lv_label.c
    libs/lvgl/src/widgets/lv_line.c
    libs/lvgl/src/widgets/lv_roller.c
    libs/lvgl/src/widgets/lv_slider.c
    libs/lvgl/src/widgets/lv_switch.c
    libs/lvgl/src/widgets/lv_table.c
    libs/lvgl/src/widgets/lv_textarea.c
    libs/lvgl/src/extra/lv_extra.c
    libs/lvgl/src/extra/layouts/flex/lv_flex.c
    libs/lvgl/src/extra/layouts/grid/lv_grid.c
    libs/lvgl/src/extra/themes/basic/lv_theme_basic.c
    libs/lvgl/src/extra/themes/default/lv_theme_default.c
    libs/lvgl/src/extra/themes/mono/lv_theme_mono.c
    libs/lvgl/src/extra/widgets/animimg/lv_animimg.c
    libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.c
    libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.c
    libs/lvgl/src/extra/widgets/calendar/lv_calendar.c
    libs/lvgl/src/extra/widgets/chart/lv_chart.c
    libs/lvgl/src/extra/widgets/colorwheel/lv_colorwheel.c
    libs/lvgl/src/extra/widgets/imgbtn/lv_imgbtn.c
    libs/lvgl/src/extra/widgets/keyboard/lv_keyboard.c
    libs/lvgl/src/extra/widgets/led/lv_led.c
    libs/lvgl/src/extra/widgets/list/lv_list.c
    libs/lvgl/src/extra/widgets/meter/lv_meter.c
    libs/lvgl/src/extra/widgets/msgbox/lv_msgbox.c
    libs/lvgl/src/extra/widgets/span/lv_span.c
    libs/lvgl/src/extra/widgets/spinbox/lv_spinbox.c
    libs/lvgl/src/extra/widgets/spinner/lv_spinner.c
    libs/lvgl/src/extra/widgets/tabview/lv_tabview.c
    libs/lvgl/src/extra/widgets/tileview/lv_tileview.c
    libs/lvgl/src/extra/widgets/win/lv_win.c
)

include_directories(
    .
    ../
    libs/
    libs/nRF52/cores/nRF5
    libs/nRF52/cores/nRF5/freertos
    libs/nRF52/cores/nRF5/freertos/config
    libs/nRF52/cores/nRF5/freertos/License
    libs/nRF52/cores/nRF5/freertos/portable
    libs/nRF52/cores/nRF5/freertos/portable/CMSIS
    libs/nRF52/cores/nRF5/freertos/portable/CMSIS/nrf52
    libs/nRF52/cores/nRF5/freertos/portable/GCC
    libs/nRF52/cores/nRF5/freertos/portable/GCC/nrf52
    libs/nRF52/cores/nRF5/freertos/Source
    libs/nRF52/cores/nRF5/freertos/Source/include
    libs/nRF52/cores/nRF5/freertos/Source/portable
    libs/nRF52/cores/nRF5/freertos/Source/portable/MemMang
    libs/nRF52/cores/nRF5/linker
    libs/nRF52/cores/nRF5/nordic
    libs/nRF52/cores/nRF5/nordic/nrfx
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/include
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src
    libs/nRF52/cores/nRF5/nordic/nrfx/drivers/src/prs
    libs/nRF52/cores/nRF5/nordic/nrfx/hal
    libs/nRF52/cores/nRF5/nordic/nrfx/helpers
    libs/nRF52/cores/nRF5/nordic/nrfx/mdk
    libs/nRF52/cores/nRF5/nordic/nrfx/soc
    libs/nRF52/cores/nRF5/nordic/softdevice
    libs/nRF52/cores/nRF5/nordic/softdevice/s112_nrf52_6.1.1_API
    libs/nRF52/cores/nRF5/nordic/softdevice/s112_nrf52_6.1.1_API/include
    libs/nRF52/cores/nRF5/nordic/softdevice/s112_nrf52_6.1.1_API/include/nrf52
    libs/nRF52/cores/nRF5/sysview
    libs/nRF52/cores/nRF5/sysview/Config
    libs/nRF52/cores/nRF5/sysview/SEGGER
    libs/nRF52/cores/nRF5/utility
    libs/nRF52/libraries
    libs/nRF52/libraries/Adafruit_LittleFS
    libs/nRF52/libraries/Adafruit_LittleFS/src
    libs/nRF52/libraries/Adafruit_LittleFS/src/littlefs
    libs/nRF52/libraries/Adafruit_nRFCrypto
    libs/nRF52/libraries/Adafruit_nRFCrypto/src
    libs/nRF52/libraries/Adafruit_nRFCrypto/src/cortex-m4
    libs/nRF52/libraries/Adafruit_nRFCrypto/src/cortex-m4/fpv4-sp-d16-hard
    libs/nRF52/libraries/Adafruit_nRFCrypto/src/ecc
    libs/nRF52/libraries/Adafruit_nRFCrypto/src/nrf_cc310
    libs/nRF52/libraries/Adafruit_nRFCrypto/src/nrf_cc310/include
    libs/nRF52/libraries/Bluefruit52Lib
    libs/nRF52/libraries/Bluefruit52Lib/src
    libs/nRF52/libraries/Bluefruit52Lib/src/utility
    libs/nRF52/libraries/Bluefruit52Lib/src/services
    libs/nRF52/libraries/InternalFileSytem
    libs/nRF52/libraries/InternalFileSytem/src
    libs/nRF52/libraries/InternalFileSytem/src/flash
    libs/nRF52/libraries/SPI
    #libs/nRF52/libraries/Wire
    libs/nRF52/cmsis/include

    libs/lvgl/src
    libs/lvgl/src/core
    libs/lvgl/src/draw
    libs/lvgl/src/extra
    libs/lvgl/src/extra/layouts
    libs/lvgl/src/extra/layouts/flex
    libs/lvgl/src/extra/layouts/grid
    libs/lvgl/src/extra/themes
    libs/lvgl/src/extra/themes/basic
    libs/lvgl/src/extra/themes/default
    libs/lvgl/src/extra/themes/mono
    libs/lvgl/src/extra/widgets
    libs/lvgl/src/extra/widgets/animimg
    libs/lvgl/src/extra/widgets/calendar
    libs/lvgl/src/extra/widgets/chart
    libs/lvgl/src/extra/widgets/colorwheel
    libs/lvgl/src/extra/widgets/imgbtn
    libs/lvgl/src/extra/widgets/keyboard
    libs/lvgl/src/extra/widgets/led
    libs/lvgl/src/extra/widgets/list
    libs/lvgl/src/extra/widgets/meter
    libs/lvgl/src/extra/widgets/msgbox
    libs/lvgl/src/extra/widgets/span
    libs/lvgl/src/extra/widgets/spinbox
    libs/lvgl/src/extra/widgets/spinner
    libs/lvgl/src/extra/widgets/tabview
    libs/lvgl/src/extra/widgets/tileview
    libs/lvgl/src/extra/widgets/win
    libs/lvgl/src/font
    libs/lvgl/src/hal
    libs/lvgl/src/misc
    libs/lvgl/src/widgets

    libs/hrs3300/src

    smartwatch
    smartwatch/applications
    smartwatch/modules/
    smartwatch/modules/lvgl
    smartwatch/modules/lvgl/theme
    smartwatch/modules/backlight
    smartwatch/modules/display
    smartwatch/modules/battery
    smartwatch/modules/pinio
    smartwatch/modules/rtc_time
    smartwatch/modules/touch
    smartwatch/modules/vibration
    smartwatch/modules/watchdog
    #smartwatch/modules/step_count
    #smartwatch/modules/hr
    smartwatch/services/
    smartwatch/services/notification
    smartwatch/services/weather
    smartwatch/services/calls
)

list(APPEND SOURCE_FILES
    main.c

    libs/hrs3300/src/hrs3300.c
    #libs/hrs3300/src/HRS3300lib.cpp

    smartwatch/modules/pinio/fast_spi.cpp
    smartwatch/modules/pinio/i2c.cpp
    smartwatch/modules/pinio/I2S.cpp
    smartwatch/modules/display/display.cpp
    smartwatch/modules/backlight/backlight.cpp
    smartwatch/modules/touch/touch.cpp
    smartwatch/modules/watchdog/watchdog.cpp
    smartwatch/modules/battery/battery.cpp
    smartwatch/modules/vibration/vibration.cpp
    smartwatch/modules/rtc_time/rtc_time.cpp
    #smartwatch/modules/step_count/bma4.c
    #smartwatch/modules/step_count/bma421.c
    #smartwatch/modules/step_count/step_count.cpp

    #smartwatch/modules/hr/hr.cpp

    smartwatch/modules/lvgl/theme/lv_font_24.c
    smartwatch/modules/lvgl/theme/lv_font_38.c
    smartwatch/modules/lvgl/theme/lv_font_sys_48.c
    smartwatch/modules/lvgl/theme/lv_font_clock_42.c
    #smartwatch/modules/lvgl/theme/lv_font_clock_76.c
    smartwatch/modules/lvgl/theme/lv_font_clock_90.c
    smartwatch/modules/lvgl/theme/lv_theme_pinetime.c
    smartwatch/modules/lvgl/lvglmodule.cpp

    smartwatch/services/notification/notification.cpp

    smartwatch/utils.cpp
    smartwatch/base.cpp
    smartwatch/smartwatch.cpp

)


link_directories(
    ../
)

set(COMMON_FLAGS -MP -MD -mthumb -mabi=aapcs -Wall -Wno-unknown-pragmas -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wreturn-type -Werror=return-type)
add_definitions(-DNRF52 -DNRF52832 -DNRF52832_XXAA -DNRF52_PAN_74 -DNRF52_PAN_64 -DNRF52_PAN_12 -DNRF52_PAN_58 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_15 -DNRF52_PAN_20 -DNRF52_PAN_55 -DBOARD_PCA10040)
add_definitions(-DFREERTOS)
add_definitions(-D__STACK_SIZE=0x1000)
add_definitions(-D__HEAP_SIZE=0x1500)
add_definitions(-DS112)
add_definitions(-DSOFTDEVICE_PRESENT)
add_definitions(-DF_CPU=64000000)

# Release
set(COMMON_FLAGS_OPT -Os -g3 -fno-exceptions -fno-non-call-exceptions)

add_definitions(-DCFG_DEBUG=0)

# NRF SDK
add_library(nrf-sdk STATIC ${NRFX_SOURCE_FILES})
target_include_directories(nrf-sdk SYSTEM PUBLIC . ../)
target_include_directories(nrf-sdk SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(nrf-sdk PRIVATE
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT} -fno-rtti>
            $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )


# LVGL
add_library(lvgl STATIC ${LVGL_SRC})
target_include_directories(lvgl SYSTEM PUBLIC . ../)
target_include_directories(lvgl SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(lvgl PRIVATE
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT} -fno-rtti>
            $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

# Build binary
set(EXECUTABLE_APP_NAME "my-time")
set(EXECUTABLE_APP_FILE_NAME ${EXECUTABLE_APP_NAME})
set(IMAGE_APP_FILE_NAME image-${EXECUTABLE_APP_NAME}.bin)
set(DFU_FILE_NAME dfu-${EXECUTABLE_APP_NAME}.zip)
set(NRF5_LINKER_SCRIPT_MCUBOOT "${CMAKE_SOURCE_DIR}/src/libs/nRF52/cores/nRF5/linker/nrf52832_s112_v6.ld")
add_executable(${EXECUTABLE_APP_NAME} ${SOURCE_FILES})
#${CMAKE_SOURCE_DIR}/src/libs/hrs3300/src/cortex-m4/libheart.a
target_link_libraries(${EXECUTABLE_APP_NAME} nrf-sdk lvgl ${CMAKE_SOURCE_DIR}/src/libs/hrs3300/src/cortex-m4/libheart.a)
set_target_properties(${EXECUTABLE_APP_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_APP_FILE_NAME})
target_compile_options(${EXECUTABLE_APP_NAME} PUBLIC
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT} -fno-rtti>
            $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

set_target_properties(${EXECUTABLE_APP_NAME} PROPERTIES
        SUFFIX ".out"
        LINK_FLAGS "-mthumb -mabi=aapcs -L ${CMAKE_SOURCE_DIR}/src/libs/nRF52/cores/nRF5/nordic/nrfx/mdk -L ${CMAKE_SOURCE_DIR}/src/libs/nRF52/cores/nRF5/linker -T ${NRF5_LINKER_SCRIPT_MCUBOOT} -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--print-memory-usage -Wl,-Map=${EXECUTABLE_APP_FILE_NAME}.map"
        )

add_custom_command(TARGET ${EXECUTABLE_APP_NAME}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_APP_FILE_NAME}.out
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE_APP_FILE_NAME}.out "${EXECUTABLE_APP_FILE_NAME}.bin"
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_APP_FILE_NAME}.out "${EXECUTABLE_APP_FILE_NAME}.hex"
        COMMENT "post build steps for ${EXECUTABLE_APP_FILE_NAME}"
        )


add_custom_target(FLASH_ERASE
        COMMAND ${OPENOCD_BIN_PATH} -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c init -c halt -c 'nrf5 mass_erase' -c reset -c shutdown
        COMMENT "## Erasing flashing"
        )

add_custom_target(FLASH_SOFTDEVICE
        COMMAND ${OPENOCD_BIN_PATH} -c "tcl_port disabled" -c "gdb_port 3333" -c "telnet_port 4444" -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c "program \"${CMAKE_SOURCE_DIR}/src/libs/nRF52/softdevice/s112/hex/s112_nrf52_6.1.1_softdevice.hex\"" -c reset -c shutdown
        COMMENT "## Flashing SoftDevice s112_nrf52_6.1.1_softdevice.hex"
        )

add_custom_target(MAKE_BOOT
        DEPENDS ${EXECUTABLE_APP_NAME}
        COMMAND ${NRFUTIL} settings generate --family NRF52 --application ${EXECUTABLE_APP_NAME}.hex --app-boot-validation VALIDATE_GENERATED_CRC --application-version 0x02 --bootloader-version 0x02 --bl-settings-version 2 --softdevice ${CMAKE_SOURCE_DIR}/src/libs/nRF52/softdevice/s112/hex/s112_nrf52_6.1.1_softdevice.hex dfu_settings.hex
        COMMAND hexmerge.py --overlap=replace ${CMAKE_SOURCE_DIR}/src/libs/nRF52/softdevice/s112/hex/s112_nrf52_6.1.1_softdevice.hex ${CMAKE_SOURCE_DIR}/bootloader/bootloader.hex ${EXECUTABLE_APP_NAME}.hex dfu_settings.hex -o ${EXECUTABLE_APP_NAME}.boot.hex
        COMMENT "## Build Bootloader and App for flashing"
        )

add_custom_target(MAKE_PKG
        DEPENDS ${EXECUTABLE_APP_NAME}
        COMMAND ${NRFUTIL} pkg generate --key-file /mnt/d/Work/PineTime/nordic_pem_keys/pinetime.pem --app-boot-validation VALIDATE_GENERATED_CRC --hw-version 52 --sd-req 0xB8 --application-version 0x02 --application ${EXECUTABLE_APP_NAME}.hex ${EXECUTABLE_APP_NAME}.dfu.zip
        COMMENT "## Build .zip for DFU"
        )

add_custom_target(MAKE_PKG_OLD
        DEPENDS ${EXECUTABLE_APP_NAME}
        COMMAND ${NRFUTIL} pkg generate --key-file /mnt/d/Work/PineTime/nordic_pem_keys/pinetime.pem --app-boot-validation VALIDATE_GENERATED_CRC --hw-version 52 --sd-req 0x0103 --application-version-string "0.1.0" --application ${EXECUTABLE_APP_NAME}.hex ${EXECUTABLE_APP_NAME}.dfu.zip
        COMMENT "## Build .zip for DFU"
        )

add_custom_target(MAKE_BOOT_PKG
        COMMAND ${NRFUTIL} settings generate --family NRF52 --application ${EXECUTABLE_APP_NAME}.hex --app-boot-validation VALIDATE_GENERATED_CRC --application-version 0x02 --bootloader-version 0x03 --bl-settings-version 2 --softdevice ${CMAKE_SOURCE_DIR}/src/libs/nRF52/softdevice/s112/hex/s112_nrf52_6.1.1_softdevice.hex boot_settings.hex
        COMMAND hexmerge.py --overlap=replace ${CMAKE_SOURCE_DIR}/bootloader/bootloader.hex boot_settings.hex -o ${EXECUTABLE_APP_NAME}.boot.hex
        COMMAND ${NRFUTIL} pkg generate --key-file /mnt/d/Work/PineTime/nordic_pem_keys/pinetime.pem --hw-version 52 --sd-req 0xB8 --bootloader-version 0x03 --bootloader ${EXECUTABLE_APP_NAME}.boot.hex ${EXECUTABLE_APP_NAME}.bootloader.dfu.zip
        COMMENT "## Build bootloader .zip for DFU"
        )

add_custom_target(FLASH_ALL
        DEPENDS MAKE_BOOT
        COMMAND ${OPENOCD_BIN_PATH} -c "tcl_port disabled" -c "gdb_port 3333" -c "telnet_port 4444" -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c "program \"${EXECUTABLE_APP_NAME}.boot.hex\"" -c reset -c shutdown
        COMMENT "## Flashing ALL (Bootloader, Settings, APP and SoftDevice)"
        )

add_custom_target(FLASH_APP
        DEPENDS ${EXECUTABLE_APP_NAME}
        COMMAND ${NRFUTIL} settings generate --family NRF52 --application ${EXECUTABLE_APP_NAME}.hex --app-boot-validation VALIDATE_GENERATED_CRC --application-version 0x001000 --bootloader-version 0x01 --bl-settings-version 2 --softdevice ${CMAKE_SOURCE_DIR}/src/libs/nRF52/softdevice/s112/hex/s112_nrf52_6.1.1_softdevice.hex dfu_settings.hex
        COMMAND hexmerge.py --overlap=replace ${EXECUTABLE_APP_NAME}.hex dfu_settings.hex -o ${EXECUTABLE_APP_NAME}.app.hex
        COMMAND ${OPENOCD_BIN_PATH} -c "tcl_port disabled" -c "gdb_port 3333" -c "telnet_port 4444" -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c "program \"${EXECUTABLE_APP_NAME}.app.hex\"" -c reset -c shutdown
        COMMENT "## Flashing ${EXECUTABLE_APP_NAME}.hex"
        )