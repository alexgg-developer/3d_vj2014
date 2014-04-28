TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off

SOURCES += ../src/main.cpp \
    ../src/cGame.cpp \
    ../src/cInput.cpp \
    ../src/cTexture.cpp \
    ../src/cAnimation2D.cpp \
    ../src/cText.cpp \
    ../src/cSound.cpp \
    ../src/cMusic.cpp

HEADERS += \
    ../src/vec3.hpp \
    ../src/cGame.hpp \
    ../src/cInput.hpp \
    ../src/cTexture.hpp \
    ../src/TypesDefined.hpp \
    ../src/cAnimation2D.hpp \
    ../src/cText.hpp \
    ../src/cSound.hpp \
    ../src/cMusic.hpp

INCLUDEPATH += ../libs/SDL2/include/
INCLUDEPATH += ../libs/SDL2/libs/
DEPENDPATH += ../libs/SDL2/libs/

LIBS += -L../libs/SDL2/libs/ -lSDL2
LIBS += -L../libs/SDL2/libs/ -lSDL2_image
LIBS += -L../libs/SDL2/libs/ -lSDL2_ttf
