add_subdirectory(br)
add_subdirectory(cs)
add_subdirectory(da)
add_subdirectory(de)
add_subdirectory(el)
add_subdirectory(eo)
add_subdirectory(es)
add_subdirectory(et)
add_subdirectory(fi)
add_subdirectory(fr)
add_subdirectory(he)
add_subdirectory(hr)
add_subdirectory(hs)
add_subdirectory(hu)
add_subdirectory(is)
add_subdirectory(it)
add_subdirectory(ja)
add_subdirectory(ko)
add_subdirectory(mk)
add_subdirectory(nl)
add_subdirectory(no)
add_subdirectory(pl)
add_subdirectory(pt)
add_subdirectory(pt_BR)
add_subdirectory(ro)
add_subdirectory(sk)
add_subdirectory(sl)
add_subdirectory(sv)
add_subdirectory(zh_CN.GB2312)
add_subdirectory(zh_TW.Big5)

install(FILES
    kabalone.pot
    kasteroids.pot
    kblackbox.pot
    kmahjongg.pot
    kmines.pot
    konquest.pot
    kpat.pot
    kpoker.pot
    kreversi.pot
    ksame.pot
    kshisen.pot
    ksirtet.pot
    ksmiletris.pot
    ksnake.pot
    ksokoban.pot
DESTINATION ${KDE1_LOCALE})
