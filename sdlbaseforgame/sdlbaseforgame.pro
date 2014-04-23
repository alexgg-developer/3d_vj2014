TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../src/main.cpp \
    ../src/cGame.cpp \
    ../src/cInput.cpp \
    ../src/cTexture.cpp

HEADERS += \
    ../src/vec3.hpp \
    ../src/cGame.hpp \
    ../src/cInput.hpp \
    ../src/cTexture.hpp

INCLUDEPATH += ../libs/SDL2/include/
INCLUDEPATH += ../libs/SDL2/libs
DEPENDPATH += ../libs/SDL2/libs

LIBS += -L../libs/SDL2/libs/ -lSDL2

