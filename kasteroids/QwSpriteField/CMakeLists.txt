include_directories (
    ${KDE1_INCLUDE_DIR}
)

set (QwSpriteField_SOURCES
    QwSpriteField.cpp
    QwSpriteField-double.cpp
	QwSpriteField-int.cpp
    QwSpriteField-mobdouble.cpp
	QwSpriteField-mobint.cpp
    QwCluster.cpp
)
set (QwSpriteField_MOC
    QwSpriteField.h
    )

QT1_WRAP_MOC(MOC_FILES ${QwSpriteField_MOC})

add_library(QwSpriteField STATIC ${QwSpriteField_SOURCES} ${MOC_FILES})
target_link_libraries(QwSpriteField Qt::Qt1)

