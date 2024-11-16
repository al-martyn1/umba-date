# Umba-date - Date and Time Generation Utility

## Description

Pirpouse: use it in your build pipeline to generate macros `UMBA_DATE__` and `UMBA_TIME__` 
to insert them to your code, and for add the same time and date to the output file name.
Generate it single time, and use it for all of your targets (unlike the __DATE__ and __TIME__
macros which are has different values for each C/C++ file, and also different with `date` output).

Compile it using VSCode (type `code .` in the command prompt) or by calling `umba-date.bat` 
batch file (requires `g++` available in your PATH).

Run `test.bat` to see how it works.

```
Usage: umba-date [-lctime|-localtime|-gmtime] [-filename] -Type1 File1.txt [-Type2 File2.txt...]
  Where:
    -lctime,
    -localtime     - set local time to output
    -gmtime        - set GMT time to output
    -filename      - use file name compatible date and time format. Date is also in form: 'YYYY-MM-DD' for best sorting
    -notfilename   - turns '-filename' mode OFF
    -date          - simple date, no quotes, no linefeed
    -time          - simple time, no quotes, no linefeed
    -datelf        - simple date with linefeed
    -timelf        - simple time with linefeed
    -dateq         - simple date in quotes
    -timeq         - simple time in quotes
    -dateqlf       - simple date in quotes with linefeed
    -timeqlf       - simple time in quotes with linefeed
    -datepp        - date as C/C++ macro expanding to string
    -timepp        - time as C/C++ macro expanding to string
    -macroprefix   - used datepp/timepp files are generated
    -macrosuffix   - used datepp/timepp files are generated
    -bat           - write batch file which performs copy output to release archive
    -sh            - write batch file which performs copy output to release archive
    -prefix        - used while batch/sh file generated, must contain full path and name of the built file
    -targetprefix  - used while batch/sh file generated, must contain full path and name of the archive target file
    -suffix        - used while batch/sh file generated, must contain extention of the built file
```

Example: **test.bat**
```
umba-date.exe -gmtime  ^
-macroprefix TEST_     ^
-macrosuffix ___       ^
-date      gmdate.txt  ^
-time      gmtime.txt  ^
-localtime             ^
-date      date.txt    ^
-time      time.txt    ^
-filename              ^
-date      fndate.txt  ^
-time      fntime.txt  ^
-notfilename           ^
-datelf    datelf.txt  ^
-timelf    timelf.txt  ^
-dateq     dateq.txt   ^
-timeq     timeq.txt   ^
-dateqlf   dateqlf.txt ^
-timeqlf   timeqlf.txt ^
-datepp    datepp.txt  ^
-timepp    timepp.txt  ^
-prefix    path/test   ^
-suffix    .exe        ^
-targetprefix another-path\test  ^
-bat       copy_to_archive.bat   ^
-sh        copy_to_archive.sh
```


## Usage with CMake

1) Add `CMake` functions from the `helpers.cmake`:
```cmake
function(umba_generate_umba_date_umba_time_for_target TARGET)

    if (STM32)
        set(ARCHIVE_EXT .hex)
    elseif(WIN32)
        set(ARCHIVE_EXT .exe)
    else()
        set(ARCHIVE_EXT "")
    endif()

    # Check for optional second parameter to forc set up ARCHIVE_EXT
    if(${ARGC} GREATER 1) 
      set(ARCHIVE_EXT ${ARGV1})
    endif()

    set(DATE_H   umba_date__.h)
    set(TIME_H   umba_time__.h)
    set(DATE_TXT umba_date.txt)
    set(TIME_TXT umba_time.txt)

    add_custom_command(
      TARGET ${TARGET}
      PRE_BUILD
        COMMAND umba-date -localtime
          -macroprefix UMBA_
          -macrosuffix __
          -datepp "$<TARGET_FILE_DIR:${TARGET}>/${DATE_H}" 
          -timepp "$<TARGET_FILE_DIR:${TARGET}>/${TIME_H}" 
          -filename
          -date "$<TARGET_FILE_DIR:${TARGET}>/${DATE_TXT}" 
          -time "$<TARGET_FILE_DIR:${TARGET}>/${TIME_TXT}"
          -suffix       ${ARCHIVE_EXT}
          -prefix       "$<TARGET_FILE_DIR:${TARGET}>/$<TARGET_FILE_BASE_NAME:${TARGET}>"
          -targetprefix "$<TARGET_FILE_DIR:${TARGET}>/$<TARGET_FILE_BASE_NAME:${TARGET}>_$<CONFIG>"
          -bat $<TARGET_FILE_DIR:${TARGET}>/archive_local.bat
          -sh  $<TARGET_FILE_DIR:${TARGET}>/archive_local.sh
          -targetprefix "${BUILD_ARCHIVE_PATH}/$<TARGET_FILE_BASE_NAME:${TARGET}>_$<CONFIG>"
          -bat $<TARGET_FILE_DIR:${TARGET}>/archive.bat
          -sh  $<TARGET_FILE_DIR:${TARGET}>/archive.sh
    )

    target_include_directories(${TARGET} PRIVATE ${CMAKE_BINARY_DIR})

endfunction()


function(umba_add_archive_local_option TARGET)

  if(CMAKE_HOST_WIN32)
    add_custom_command(
      TARGET ${TARGET}
      POST_BUILD
      COMMAND cmd /c $<TARGET_FILE_DIR:${TARGET}>/archive_local.bat
    )
  else()
    add_custom_command(
      TARGET ${TARGET}
      POST_BUILD
      COMMAND bash -c $<TARGET_FILE_DIR:${TARGET}>/archive_local.sh
    )
  endif()

endfunction()


function(umba_add_archive_option TARGET)

  if(NOT BUILD_ARCHIVE_PATH)

    message(NOTICE "! BUILD_ARCHIVE_PATH not specified !")
    add_custom_command(
      TARGET ${TARGET}
      POST_BUILD
      COMMAND echo ! BUILD_ARCHIVE_PATH is not set!
    )

  else()

    if(CMAKE_HOST_WIN32)
      add_custom_command(
        TARGET ${TARGET}
        POST_BUILD
        COMMAND cmd /c $<TARGET_FILE_DIR:${TARGET}>/archive.bat
      )
    else()
      add_custom_command(
        TARGET ${TARGET}
        POST_BUILD
        COMMAND bash -c $<TARGET_FILE_DIR:${TARGET}>/archive.sh
      )
    endif()

  endif()

endfunction()
```
2) Configure your target:
```cmake
add_executable("MyFirmware"  ${SOURCES} )
umba_generate_umba_date_umba_time_for_target("MyFirmware")
umba_add_archive_local_option("MyFirmware")
umba_add_archive_option("MyFirmware")
```
3) Write your `build_datetime.h` C/C++ header file:
```
#pragma once

// Requires umba-date utility available in PATH (see https://github.com/al-martyn1/umba-date)

// https://en.cppreference.com/w/cpp/preprocessor/include

#if defined(__has_include) && __has_include("umba_date__.h")
    #include "umba_date__.h"
    #define BUILD_DATE  UMBA_DATE__
#else
    #define BUILD_DATE  __DATE__
#endif

#if defined(__has_include) && __has_include("umba_time__.h")
    #include "umba_time__.h"
    #define BUILD_TIME  UMBA_TIME__
#else
    #define BUILD_TIME  __TIME__
#endif

```
4) Enjoy it!




