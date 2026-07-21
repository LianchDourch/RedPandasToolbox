#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RedPandas::RedPandasToolbox" for configuration ""
set_property(TARGET RedPandas::RedPandasToolbox APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(RedPandas::RedPandasToolbox PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/libRedPandasToolbox.dll.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Qt6::Core"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/libRedPandasToolbox.dll"
  )

list(APPEND _cmake_import_check_targets RedPandas::RedPandasToolbox )
list(APPEND _cmake_import_check_files_for_RedPandas::RedPandasToolbox "${_IMPORT_PREFIX}/lib/libRedPandasToolbox.dll.a" "${_IMPORT_PREFIX}/bin/libRedPandasToolbox.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
