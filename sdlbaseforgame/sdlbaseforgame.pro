TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off
CONFIG += c++11

SOURCES += ../src/main.cpp \
    ../src/cGame.cpp \
    ../src/cInput.cpp \
    ../src/cTexture.cpp \
    ../src/cAnimation2D.cpp \
    ../src/cText.cpp \
    ../src/cSound.cpp \
    ../src/cMusic.cpp \
    ../src/cTimer.cpp \
    ../src/cWindow.cpp \
    ../src/cRenderer.cpp \
    ../src/cGLShader.cpp

HEADERS += \
    ../src/vec3.hpp \
    ../src/cGame.hpp \
    ../src/cInput.hpp \
    ../src/cTexture.hpp \
    ../src/TypesDefined.hpp \
    ../src/cAnimation2D.hpp \
    ../src/cText.hpp \
    ../src/cSound.hpp \
    ../src/cMusic.hpp \
    ../src/cTimer.hpp \
    ../src/cWindow.hpp \
    ../src/cRenderer.hpp \
    ../src/cGLShader.hpp

#INCLUDEPATH += ../libs/SDL2/include/
#INCLUDEPATH += ../libs/SMPEG/include/
#INCLUDEPATH += ../libs/SDL2/libs/
#INCLUDEPATH += ../libs/SMPEG/libs/
#DEPENDPATH += ../libs/SDL2/libs/
#DEPENDPATH += ../libs/SMPEG/libs/

#LIBS += -L../libs/SDL2/libs/ -lSDL2
#LIBS += -L../libs/SDL2/libs/ -lSDL2_image
#LIBS += -L../libs/SDL2/libs/ -lSDL2_ttf
#LIBS += -L../libs/SDL2/libs/ -lSDL2_mixer
#LIBS += -L../libs/SMPEG/libs/ -lsmpeg2
#LIBS += -lGLEW -lGL -lGLU



INCLUDEPATH += ../libs/windows/SDL2/libs/x64
INCLUDEPATH += ../libs/windows/SDL2/include/
INCLUDEPATH += ../libs/windows/glew-1.10.0/include/
#DEPENDPATH += C:/Users/w7home/VJ/3d_vj2014/libs/windows/glew-1.10.0/include/
DEPENDPATH += ../libs/windows/SDL2/libs/x64
DEPENDPATH += ../libs/windows/glew-1.10.0/lib/Release/x64
DEPENDPATH += ../libs/windows/glew-1.10.0/bin/Release/x64
LIBS += -L../libs/windows/SDL2/libs/x64/ -lSDL2
LIBS += -L../libs/windows/SDL2/libs/x64/ -lSDL2_image
LIBS += -L../libs/windows/SDL2/libs/x64/ -lSDL2_ttf
LIBS += -L../libs/windows/SDL2/libs/x64/ -lSDL2_mixer
#LIBS += -L../libs/windows/SDL2/libs/x64/ -lsmpeg2
LIBS += -L../libs/windows/glew-1.10.0/lib/Release/x64/ -lglew32
#LIBS +=  -lGL -lGLU
