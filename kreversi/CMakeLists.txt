include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(Kreversi_SRCS
    main.cpp misc.cpp app.cpp board.cpp Game.cpp Score.cpp SuperEngine.cpp
    Engine.cpp Position.cpp Move.cpp Random.cpp about.cpp playsound.cpp
)

set(Kreversi_MOC
app.moc board.moc about.moc
    )

QT1_WRAP_MOC(MOC_FILES ${Kreversi_MOC})

add_executable(kreversi1 ${Kreversi_SRCS} ${MOC_FILES} ${QwSpriteField_SOURCES})
target_link_libraries(kreversi1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_MEDIATOOL}
)

add_definitions(-DHAVE_MEDIATOOL)

install(TARGETS kreversi1 RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kreversi.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(icons/kreversi.xpm)


install(FILES
    pics/hint.xpm
    pics/help.xpm
    pics/undo.xpm
    pics/zoomout.xpm
    pics/zoomin.xpm
    pics/stop.xpm
    pics/chips.xpm
    pics/logo.xpm
    pics/chips_mono.xpm
    DESTINATION "${KDE1_DATADIR}/kreversi/pics")

install(FILES
    pics/background/Dark_Wood.xpm
    pics/background/Earth.xpm
    pics/background/Granite.xpm
    pics/background/Hexagon.xpm
    pics/background/Light_Wood.xpm
    pics/background/Mystique.xpm
    pics/background/Ocean.xpm
    pics/background/Pipes.xpm
    pics/background/Puzzle.xpm
    pics/background/Stones.xpm
    DESTINATION "${KDE1_DATADIR}/kreversi/pics/background")

install(FILES
    sounds/click.wav
    sounds/lost.wav
    sounds/won.wav
    DESTINATION "${KDE1_DATADIR}/kreversi/sounds")

install(FILES
    html/index.html
    html/index-1.html
    html/index-2.html
    html/index-3.html
    html/index-4.html
    html/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kreversi )
