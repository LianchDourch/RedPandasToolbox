#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RedPandas::RedPandasToolbox" for configuration "Debug"
set_property(TARGET RedPandas::RedPandasToolbox APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RedPandas::RedPandasToolbox PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libRedPandasToolbox.dll.a"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libRedPandasToolbox.dll"
  )

list(APPEND _cmake_import_check_targets RedPandas::RedPandasToolbox )
list(APPEND _cmake_import_check_files_for_RedPandas::RedPandasToolbox "${_IMPORT_PREFIX}/lib/libRedPandasToolbox.dll.a" "${_IMPORT_PREFIX}/bin/libRedPandasToolbox.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
