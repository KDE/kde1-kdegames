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

add_executable(kreversi ${Kreversi_SRCS} ${MOC_FILES} ${QwSpriteField_SOURCES})
target_link_libraries(kreversi
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_MEDIATOOL}
)

add_definitions(-DHAVE_MEDIATOOL)

install(TARGETS kreversi RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kreversi.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(kreversi.xpm)


file(GLOB PIXMAPS pics/*.xpm)
install(FILES ${PIXMAPS}
    DESTINATION "${KDE1_DATADIR}/kreversi/pics")

file(GLOB BACKGROUNDS pics/background/*.xpm)
install(FILES ${BACKGROUNDS}
    DESTINATION "${KDE1_DATADIR}/kreversi/background")

file(GLOB SOUNDS sounds/*.wav)
install(FILES ${SOUNDS}
    DESTINATION "${KDE1_DATADIR}/kreversi/sounds")

install(FILES
    html/index.html
    html/index-1.html
    html/index-2.html
    html/index-3.html
    html/index-4.html
    html/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kreversi )
