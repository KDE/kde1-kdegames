add_subdirectory(docs)

include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(Ksame_SRCS main.cpp StoneWidget.cpp KSameWidget.cpp HighScore.cpp)

set(Ksame_MOC StoneWidget.h KSameWidget.h HighScore.h)

QT1_WRAP_MOC(MOC_FILES ${Ksame_MOC})

add_executable(ksame ${Ksame_SRCS} ${MOC_FILES})
target_link_libraries(ksame
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS ksame RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES ksame.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(ksame.xpm)

set(PICS stones.gif stones_mask.gif)
install(FILES ${PICS} DESTINATION "${KDE1_DATADIR}/ksame/pics")
