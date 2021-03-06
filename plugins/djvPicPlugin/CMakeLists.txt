set(header
    djvPic.h
    djvPicLoad.h
    djvPicPlugin.h)

set(source
    djvPic.cpp
    djvPicLoad.cpp
    djvPicPlugin.cpp)

if (Qt5Core_DIR)
    QT5_CREATE_TRANSLATION(qmSource ${source}
        djvPicPlugin_en_US.ts)
else()
    QT4_CREATE_TRANSLATION(qmSource ${source}
        djvPicPlugin_en_US.ts)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djvPicPlugin
    ${djvCoreIncludeDirs})

add_library(djvPicPlugin MODULE ${header} ${source} ${qmSource})

target_link_libraries(djvPicPlugin ${djvCoreLibs})

set_target_properties(djvPicPlugin PROPERTIES FOLDER plugins)

if (djvInstallRpath)

    set_target_properties(
        djvPicPlugin PROPERTIES
        INSTALL_RPATH "${djvInstallRpath}")

endif (djvInstallRpath)

install(
    FILES ${qmSource}
    DESTINATION translations)
install(
    TARGETS djvPicPlugin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
