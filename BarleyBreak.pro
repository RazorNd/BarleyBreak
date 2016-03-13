TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tagboard.cpp \
    findertaggame.cpp

HEADERS += \
    tagboard.h \
    findertaggame.h

OTHER_FILES += \
    input.txt \
    output.txt

