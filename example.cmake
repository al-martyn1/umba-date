add_executable("MyFirmware"  ${SOURCES} )
umba_generate_umba_date_umba_time_for_target("MyFirmware")
umba_add_archive_local_option("MyFirmware")
umba_add_archive_option("MyFirmware")
