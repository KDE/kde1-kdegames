add_subdirectory(data)
add_subdirectory(wav)

include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KSMILETRIS_SRCS
    main.cpp gamewindow.cpp gamewidget.cpp screenwidget.cpp mirrorwidget.cpp
    npiecewidget.cpp scoredialog.cpp newscoredialog.cpp
)

set(KSMILETRIS_MOC
    gamewindow.moc gamewidget.moc screenwidget.moc mirrorwidget.moc
    npiecewidget.moc scoredialog.moc newscoredialog.moc
)

QT1_WRAP_MOC(MOC_FILES ${KSMILETRIS_MOC})

add_executable(ksmiletris ${KSMILETRIS_SRCS} ${MOC_FILES})
target_link_libraries(ksmiletris
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_MEDIATOOL}
)

install(TARGETS ksmiletris RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES ksmiletris.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)

install_icon(ksmiletris.xpm)

install(FILES
    doc/en/index.html
    doc/en/ksmiletris.gif
DESTINATION ${KDE1_HTMLDIR}/en/ksmiletris)
