add_subdirectory(html)
#add_subdirectory(sprites)
add_subdirectory(QwSpriteField)

include_directories (
    ${KDE1_INCLUDE_DIR}
    QwSpriteField
)

set(KASTEROIDS_SRCS  kfixedtopwidget.cpp main.cpp toplevel.cpp view.cpp)

set(KASTEROIDS_MOC
    kfixedtopwidget.moc
    toplevel.moc
    view.moc
    )

QT1_WRAP_MOC(MOC_FILES ${KASTEROIDS_MOC})

add_executable(kasteroids ${KASTEROIDS_SRCS} ${MOC_FILES} ${QwSpriteField_SOURCES})
target_link_libraries(kasteroids
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_MEDIATOOL}
    QwSpriteField
)

install(TARGETS kasteroids RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kasteroids.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(kasteroids.xpm)

file(GLOB ROCK1_SPRITES sprites/rock1/*.ppm)
file(GLOB ROCK1_MASKS   sprites/rock1/*.pbm)

file(GLOB ROCK2_SPRITES sprites/rock2/*.ppm)
file(GLOB ROCK2_MASKS   sprites/rock2/*.pbm)

file(GLOB ROCK3_SPRITES sprites/rock3/*.ppm)
file(GLOB ROCK3_MASKS   sprites/rock3/*.pbm)

file(GLOB MISSILE_SPRITES sprites/missile/*.ppm)
file(GLOB MISSILE_MASKS   sprites/missile/*.pbm)

file(GLOB SHIP_SPRITES sprites/ship/*.ppm)
file(GLOB SHIP_MASKS   sprites/ship/*.pbm)

file(GLOB BITS_SPRITES sprites/bits/*.ppm)
file(GLOB BITS_MASKS   sprites/bits/*.pbm)


install(FILES ${ROCK1_SPRITES} ${ROCK1_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/rock1")

install(FILES ${ROCK2_SPRITES} ${ROCK2_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/rock2")
install(FILES ${ROCK3_SPRITES} ${ROCK3_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/rock3")
install(FILES ${MISSILE_SPRITES} ${MISSILE_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/missile")
install(FILES ${SHIP_SPRITES} ${SHIP_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/ship")
install(FILES ${BITS_SPRITES} ${BITS_MASKS}
    DESTINATION "${KDE1_DATADIR}/kasteroids/sprites/bits")

install(FILES bg.ppm DESTINATION ${KDE1_DATADIR}/kasteroids/sprites)
