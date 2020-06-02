add_subdirectory(docs)

include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(Kpoker_SRCS kpoker.cpp kpaint.cpp top.cpp main.cpp)

set(Kpoker_MOC kpoker.moc kpaint.moc top.moc)

QT1_WRAP_MOC(MOC_FILES ${Kpoker_MOC})

add_executable(kpoker ${Kpoker_SRCS} ${MOC_FILES} ${QwSpriteField_SOURCES})
target_link_libraries(kpoker
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_MEDIATOOL}
)

install(TARGETS kpoker RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kpoker.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(kpoker.xpm)

file(GLOB PIXMAPS bitmaps/*.bmp)
install(FILES ${PIXMAPS}
    DESTINATION "${KDE1_DATADIR}/kpoker/pics")

file(GLOB SOUNDS sounds/*.wav)
install(FILES ${SOUNDS}
    DESTINATION "${KDE1_DATADIR}/kpoker/sounds")
