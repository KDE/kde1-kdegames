#add_subdirectory(icons)
#add_subdirectory(pics)

include_directories (
    ${KDE1_INCLUDE_DIR}
)

set(KPAT_SRCS main.cpp patience.cpp rnd.cpp cardmaps.cpp
    computation.cpp klondike.cpp grandf.cpp ten.cpp
    basiccard.cpp card.cpp napoleon.cpp idiot.cpp
    microsol.cpp pwidget.cpp freecell.cpp mod3.cpp)

set(KPAT_MOC patience.moc computation.moc klondike.moc
    grandf.moc basiccard.moc card.moc mod3.moc
    ten.moc napoleon.moc idiot.moc freecell.moc
    microsol.moc pwidget.moc cardmaps.moc)

QT1_WRAP_MOC(MOC_FILES ${KPAT_MOC})

add_executable(kpat1 ${KPAT_SRCS} ${MOC_FILES})
target_link_libraries(kpat1
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

# If you want to list 52 cards individually, go ahead
# Why thank you.
install(FILES
    pics/1.bmp pics/2.bmp pics/3.bmp pics/4.bmp pics/5.bmp pics/6.bmp
    pics/7.bmp pics/8.bmp pics/9.bmp pics/10.bmp pics/11.bmp pics/12.bmp
    pics/13.bmp pics/14.bmp pics/15.bmp pics/16.bmp pics/17.bmp pics/18.bmp
    pics/19.bmp pics/20.bmp pics/21.bmp pics/22.bmp pics/23.bmp pics/24.bmp
    pics/25.bmp pics/26.bmp pics/27.bmp pics/28.bmp pics/29.bmp pics/30.bmp
    pics/31.bmp pics/32.bmp pics/33.bmp pics/34.bmp pics/35.bmp pics/36.bmp
    pics/37.bmp pics/38.bmp pics/39.bmp pics/40.bmp pics/41.bmp pics/42.bmp
    pics/43.bmp pics/44.bmp pics/45.bmp pics/46.bmp pics/47.bmp pics/48.bmp
    pics/49.bmp pics/50.bmp pics/51.bmp pics/52.bmp
    pics/back1.bmp
    pics/back2.bmp
    pics/back3.bmp
        DESTINATION "${KDE1_DATADIR}/kpat/pics")

install(FILES
    index.html index-1.html index-2.html logotp3.gif
    DESTINATION ${KDE1_HTMLDIR}/en/kpat
)

install(TARGETS kpat1 RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kpat.kdelnk DESTINATION ${KDE1_APPSDIR}/Games)
install_icon(icons/kpat.xpm)

install(FILES icons/kpat-lq.xpm DESTINATION ${KDE1_ICONDIR})
