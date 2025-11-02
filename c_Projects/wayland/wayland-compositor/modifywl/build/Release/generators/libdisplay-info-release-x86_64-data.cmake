########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(libdisplay-info_COMPONENT_NAMES "")
if(DEFINED libdisplay-info_FIND_DEPENDENCY_NAMES)
  list(APPEND libdisplay-info_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES libdisplay-info_FIND_DEPENDENCY_NAMES)
else()
  set(libdisplay-info_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(libdisplay-info_PACKAGE_FOLDER_RELEASE "/home/mdmmj/.conan2/p/b/libdiabcce373da2e7/p")
set(libdisplay-info_BUILD_MODULES_PATHS_RELEASE )


set(libdisplay-info_INCLUDE_DIRS_RELEASE "${libdisplay-info_PACKAGE_FOLDER_RELEASE}/include")
set(libdisplay-info_RES_DIRS_RELEASE )
set(libdisplay-info_DEFINITIONS_RELEASE )
set(libdisplay-info_SHARED_LINK_FLAGS_RELEASE )
set(libdisplay-info_EXE_LINK_FLAGS_RELEASE )
set(libdisplay-info_OBJECTS_RELEASE )
set(libdisplay-info_COMPILE_DEFINITIONS_RELEASE )
set(libdisplay-info_COMPILE_OPTIONS_C_RELEASE )
set(libdisplay-info_COMPILE_OPTIONS_CXX_RELEASE )
set(libdisplay-info_LIB_DIRS_RELEASE "${libdisplay-info_PACKAGE_FOLDER_RELEASE}/lib")
set(libdisplay-info_BIN_DIRS_RELEASE )
set(libdisplay-info_LIBRARY_TYPE_RELEASE STATIC)
set(libdisplay-info_IS_HOST_WINDOWS_RELEASE 0)
set(libdisplay-info_LIBS_RELEASE display-info)
set(libdisplay-info_SYSTEM_LIBS_RELEASE m)
set(libdisplay-info_FRAMEWORK_DIRS_RELEASE )
set(libdisplay-info_FRAMEWORKS_RELEASE )
set(libdisplay-info_BUILD_DIRS_RELEASE )
set(libdisplay-info_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(libdisplay-info_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${libdisplay-info_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${libdisplay-info_COMPILE_OPTIONS_C_RELEASE}>")
set(libdisplay-info_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libdisplay-info_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libdisplay-info_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libdisplay-info_EXE_LINK_FLAGS_RELEASE}>")


set(libdisplay-info_COMPONENTS_RELEASE )