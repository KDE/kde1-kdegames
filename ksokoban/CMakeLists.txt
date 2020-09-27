add_executable(bin2c images/bin2c.c)

function(add_resource generated_source filedir)
    set(resource_file "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}")
    set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${ARGV1}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${filedir}")
    add_custom_command(
        OUTPUT "${filedir}/data.c"
        COMMAND bin2c ${ARGN}
        DEPENDS bin2c ${ARGN}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${filedir}"
        COMMENT "Generating code for resource ${resource_file}"
        VERBATIM
        )
    set(${generated_source} "${filedir}/data.c" PARENT_SCOPE)
endfunction()

add_resource(IMG_RES images "${CMAKE_CURRENT_SOURCE_DIR}/images/background.bmp")

add_resource(IMG_20_RES
    images/20
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/eastwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/floor.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/goal.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/horizwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/man.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/object.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/saveman.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/treasure.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/vertiwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/20/westwall.ppm
)
add_resource(IMG_32_RES
    images/32
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/eastwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/floor.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/goal.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/horizwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/man.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/object.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/saveman.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/treasure.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/vertiwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/32/westwall.ppm
)
add_resource(IMG_40_RES
    images/40
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/eastwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/floor.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/goal.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/horizwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/man.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/object.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/saveman.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/treasure.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/vertiwall.ppm
    ${CMAKE_CURRENT_SOURCE_DIR}/images/40/westwall.ppm
)
add_resource(LEVELS_RES
    levels
    ${CMAKE_CURRENT_SOURCE_DIR}/levels/level.data
    )

include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(ksokoban_SRCS
    # shouldn't really build it, but I can't be arsed to fuck more around with cmake
    ${IMG_RES}
    ${IMG_40_RES}
    ${IMG_32_RES}
    ${IMG_20_RES}
    ${LEVELS_RES}
    Bookmark.C
    HiRes.C
    History.C
    HtmlPrinter.C
    LevelMap.C
    LowRes.C
    main.C
    MainWindow.C
    Map.C
    MapDelta.C
    MedRes.C
    ModalLabel.C
    Move.C
    MoveSequence.C
    PathFinder.C
    PlayField.C
    StaticImage.C
)

set(ksokoban_MOC
    MainWindow.H
    ModalLabel.H
    PlayField.H
)

QT1_WRAP_MOC(MOC_FILES ${ksokoban_MOC})

add_executable(ksokoban ${ksokoban_SRCS} ${MOC_FILES})
target_link_libraries(ksokoban
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS ksokoban RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES ksokoban.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(ksokoban.xpm)

file(GLOB BACKGROUNDS data/backgrounds/*.xpm)
install(FILES ${BACKGROUNDS}
    DESTINATION "${KDE1_DATADIR}/ksokoban/backgrounds")

file(GLOB PIXMAPS data/pixmaps/*.xpm)
install(FILES ${PIXMAPS}
    DESTINATION "${KDE1_DATADIR}/ksokoban/pics")

install(FILES data/highScores
    DESTINATION "${KDE1_DATADIR}/ksokoban/")

file(GLOB LEVELS data/levels/room*)
install(FILES ${LEVELS}
    DESTINATION "${KDE1_DATADIR}/ksokoban/levels")
