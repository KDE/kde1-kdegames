include_directories (
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(Kshisen_SRCS app.cpp board.cpp main.cpp)

set(Kshisen_MOC app.moc board.moc)

QT1_WRAP_MOC(MOC_FILES ${Kshisen_MOC})

add_executable(kshisen ${Kshisen_SRCS} ${MOC_FILES})
target_link_libraries(kshisen
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kshisen RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kshisen.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(kshisen-large.xpm)

install_icon(pics/kshisen.bmp "${KDE1_DATADIR}/kshisen/pics")
install_icon(pics/kshisen_bgnd.bmp "${KDE1_DATADIR}/kshisen/pics")
install_icon(pics/kshisen.xpm "${KDE1_DATADIR}/kshisen/pics")
install_icon(pics/kshisen_bgnd.xpm "${KDE1_DATADIR}/kshisen/pics")

install(FILES
    html/index.html html/index-1.html html/index-2.html html/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kshisen )
