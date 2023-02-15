QT       += core gui widgets openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

CONFIG += c++17 console


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glviewportwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp

HEADERS += \
    glviewportwidget.h \
    mainwindow.h \
    mesh.h

FORMS += \
    mainwindow.ui

unix:!macx {
LIBS += -L/usr/local/lib/ -lqtadvanceddocking -lassimp

INCLUDEPATH += /usr/local/include
}

windows {
LIBS += -L'C:\\Program Files (x86)\\Assimp\bin'
LIBS += -L'C:\\Program Files (x86)\\Assimp\lib'
LIBS += -L'C:\Users\talis.kapfhammer\OneDrive - Accenture\Documents\git\build-ads-Desktop_Qt_6_5_0_MSVC2019_64bit-Debug\lib'

LIBS += -lassimp-vc143-mtd -lqtadvanceddockingd

INCLUDEPATH += 'C:\\Program Files (x86)\\Assimp\include'
INCLUDEPATH += 'C:\Users\talis.kapfhammer\OneDrive - Accenture\Documents\git\Qt-Advanced-Docking-System\src'

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/pass_through.vert \
    shaders/simple.frag \
    theme/darkstyle.qss

RESOURCES += \
    resources.qrc \
    theme/darkstyle.qrc
