set(header
    djvRla.h
    djvRlaLoad.h
    djvRlaPlugin.h)

set(source
    djvRla.cpp
    djvRlaLoad.cpp
    djvRlaPlugin.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djvRlaPlugin
    ${djvCoreIncludeDirs})

add_library(djvRlaPlugin MODULE ${header} ${source})

target_link_libraries(djvRlaPlugin ${djvCoreLibs})

set_target_properties(djvRlaPlugin PROPERTIES FOLDER plugins)

if (djvInstallRpath)

    set_target_properties(
        djvRlaPlugin PROPERTIES
        INSTALL_RPATH "${djvInstallRpath}")

endif (djvInstallRpath)

install(
    TARGETS djvRlaPlugin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
