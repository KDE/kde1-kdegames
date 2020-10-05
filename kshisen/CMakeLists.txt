include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(Kshisen_SRCS app.cpp board.cpp main.cpp)

set(Kshisen_MOC app.moc board.moc)

QT1_WRAP_MOC(MOC_FILES ${Kshisen_MOC})

add_executable(kshisen1 ${Kshisen_SRCS} ${MOC_FILES})
target_link_libraries(kshisen1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kshisen1 RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kshisen.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(kshisen.xpm)

install(FILES
    kshisen.xpm
    kshisen_bgnd.xpm
    DESTINATION "${KDE1_DATADIR}/kshisen/pics"
)

install(FILES
    html/index.html html/index-1.html html/index-2.html html/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kshisen )
