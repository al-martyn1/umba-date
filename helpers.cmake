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

        #[build] TARGET_FILE: "C:/work/projects/firmware/firmware_ES_4007_BMCL/.out/GCC 13.2 STM32/none-arm/Debug/ES_4007_BMCL_R_1_0_DBG8M.elf" 
        #[build] TARGET_FILE_NAME: ES_4007_BMCL_R_1_0_DBG8M.elf 
        #[build] TARGET_FILE_DIR: "C:/work/projects/firmware/firmware_ES_4007_BMCL/.out/GCC 13.2 STM32/none-arm/Debug" 
        #[build] TARGET_FILE_BASE_NAME: ES_4007_BMCL_R_1_0_DBG8M 
        #COMMAND echo TARGET_FILE: $<TARGET_FILE:${TARGET}>
        #COMMAND echo TARGET_FILE_NAME: $<TARGET_FILE_NAME:${TARGET}>
        #COMMAND echo TARGET_FILE_DIR: $<TARGET_FILE_DIR:${TARGET}>
        #COMMAND echo TARGET_FILE_BASE_NAME: $<TARGET_FILE_BASE_NAME:${TARGET}>
        #COMMAND echo TARGET_FILE_PREFIX: $<TARGET_FILE_PREFIX:${TARGET}>

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

