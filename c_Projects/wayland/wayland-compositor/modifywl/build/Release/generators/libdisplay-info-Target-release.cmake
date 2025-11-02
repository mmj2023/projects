# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libdisplay-info_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(libdisplay-info_FRAMEWORKS_FOUND_RELEASE "${libdisplay-info_FRAMEWORKS_RELEASE}" "${libdisplay-info_FRAMEWORK_DIRS_RELEASE}")

set(libdisplay-info_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libdisplay-info_DEPS_TARGET)
    add_library(libdisplay-info_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libdisplay-info_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${libdisplay-info_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${libdisplay-info_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libdisplay-info_DEPS_TARGET to all of them
conan_package_library_targets("${libdisplay-info_LIBS_RELEASE}"    # libraries
                              "${libdisplay-info_LIB_DIRS_RELEASE}" # package_libdir
                              "${libdisplay-info_BIN_DIRS_RELEASE}" # package_bindir
                              "${libdisplay-info_LIBRARY_TYPE_RELEASE}"
                              "${libdisplay-info_IS_HOST_WINDOWS_RELEASE}"
                              libdisplay-info_DEPS_TARGET
                              libdisplay-info_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "libdisplay-info"    # package_name
                              "${libdisplay-info_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libdisplay-info_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Release ########################################
    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Release>:${libdisplay-info_OBJECTS_RELEASE}>
                 $<$<CONFIG:Release>:${libdisplay-info_LIBRARIES_TARGETS}>
                 )

    if("${libdisplay-info_LIBS_RELEASE}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET libdisplay-info::libdisplay-info
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     libdisplay-info_DEPS_TARGET)
    endif()

    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Release>:${libdisplay-info_LINKER_FLAGS_RELEASE}>)
    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Release>:${libdisplay-info_INCLUDE_DIRS_RELEASE}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Release>:${libdisplay-info_LIB_DIRS_RELEASE}>)
    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Release>:${libdisplay-info_COMPILE_DEFINITIONS_RELEASE}>)
    set_property(TARGET libdisplay-info::libdisplay-info
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Release>:${libdisplay-info_COMPILE_OPTIONS_RELEASE}>)

########## For the modules (FindXXX)
set(libdisplay-info_LIBRARIES_RELEASE libdisplay-info::libdisplay-info)
