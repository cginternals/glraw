
# Define function "source_group_by_path with three mandatory arguments (PARENT_PATH, REGEX, GROUP, ...)
# to group source files in folders (e.g. for MSVC solutions).
#
# Example:
# source_group_by_path("${CMAKE_CURRENT_SOURCE_DIR}/src" "\\\\.h$|\\\\.hpp$|\\\\.cpp$|\\\\.c$|\\\\.ui$|\\\\.qrc$" "Source Files" ${sources})
function (source_group_by_path PARENT_PATH REGEX GROUP)

    foreach (FILENAME ${ARGN})
        
        get_filename_component(FILEPATH "${FILENAME}" REALPATH)
        file(RELATIVE_PATH FILEPATH ${PARENT_PATH} ${FILEPATH})
        get_filename_component(FILEPATH "${FILEPATH}" DIRECTORY)

        string(REPLACE "/" "\\" FILEPATH "${FILEPATH}")

    source_group("${GROUP}\\${FILEPATH}" REGULAR_EXPRESSION "${REGEX}" FILES ${FILENAME})

    endforeach()

endfunction (source_group_by_path)




# Function that extract entries matching a given regex from a list. ${OUTPUT} will store
# the list of matching filenames.
function (list_extract OUTPUT REGEX)

    foreach(FILENAME ${ARGN})
        if(${FILENAME} MATCHES "${REGEX}")
            list(APPEND ${OUTPUT} ${FILENAME})
        endif()
    endforeach()

endfunction (list_extract)


macro(install_qt COMP DEST)

    set(args ${ARGV})
    list(REMOVE_AT args 0)
    list(REMOVE_AT args 0)


    foreach(target ${args})
        get_target_property(qtrelease Qt5::${target} LOCATION_RELEASE)
        install(FILES ${qtrelease} DESTINATION ${DEST} COMPONENT ${COMP})
    endforeach() 

    get_target_property(qtrelease Qt5::Core LOCATION_RELEASE)
    get_filename_component(qtdir ${qtrelease} DIRECTORY)

    install(FILES 
        ${qtdir}/icudt52.dll
        ${qtdir}/icuin52.dll
        ${qtdir}/icuuc52.dll
        DESTINATION ${DEST}
        COMPONENT ${COMP})

endmacro()


macro(install_qt_platforms COMP DEST)

    set(args ${ARGV})
    list(REMOVE_AT args 0)
    list(REMOVE_AT args 0)

    get_target_property(qtrelease Qt5::Core LOCATION_RELEASE)
    get_filename_component(qtdir ${qtrelease} DIRECTORY)

    foreach(target ${args})
        install(FILES "${qtdir}/../plugins/platforms/${target}.dll" DESTINATION ${DEST}/plugins/platforms COMPONENT ${COMP})
    endforeach()    

    install(FILES ${CMAKE_SOURCE_DIR}/packages/qt.conf DESTINATION ${DEST} COMPONENT ${COMP})

endmacro()


macro(install_qt_imageformats COMP DEST)

    set(args ${ARGV})
    list(REMOVE_AT args 0)
    list(REMOVE_AT args 0)

    get_target_property(qtrelease Qt5::Core LOCATION_RELEASE)
    get_filename_component(qtdir ${qtrelease} DIRECTORY)

    foreach(target ${args})
        install(FILES "${qtdir}/../plugins/imageformats/${target}.dll" DESTINATION ${DEST}/plugins/imageformats COMPONENT ${COMP})
    endforeach()    

    install(FILES ${CMAKE_SOURCE_DIR}/packages/qt.conf DESTINATION ${DEST} COMPONENT ${COMP})

endmacro()
