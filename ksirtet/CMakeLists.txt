include_directories (
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(Ksirtet_SRCS
    gtetris.cpp tpiece.cpp spiece.cpp gtboard.cpp board.cpp
    net.cpp dialogs.cpp field.cpp main.cpp dbutton.cpp
)

set(Ksirtet_MOC
    spiece.moc board.moc dialogs.moc field.moc main.moc
)

QT1_WRAP_MOC(MOC_FILES ${Ksirtet_MOC})

add_executable(ksirtet ${Ksirtet_SRCS} ${MOC_FILES} ${QwSpriteField_SOURCES})
target_link_libraries(ksirtet
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS ksirtet RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES data/ksirtet.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(data/ksirtet.xpm)

install(FILES
    data/index.html
    data/index-1.html
    data/index-2.html
    data/index-3.html
    data/index-4.html
    data/logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/ksirtet )
