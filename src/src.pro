QT       += core gui
QT       += charts
QT       += sql
QT       += network

TRANSLATIONS = Translate_EN.ts \
                Translate_CN.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS += $$PWD/libs/libcomponents.a
#LIBS +=
INCLUDEPATH += $$PWD/libs/include
#INCLUDEPATH +=

SOURCES += \
    Scroll.cpp \
    categorizedialog.cpp \
    domainsql.cpp \
    establishdialog.cpp \
    insertdialog.cpp \
    login.cpp \
    main.cpp \
    IDNIMS.cpp \
    modifydialog.cpp \
    removedialog.cpp

HEADERS += \
    IDNIMS.h \
    Scroll.h \
    categorizedialog.h \
    domainsql.h \
    establishdialog.h \
    insertdialog.h \
    login.h \
    modifydialog.h \
    removedialog.h \
    scrolltext.h

FORMS += \
    IDNIMS.ui \
    categorizedialog.ui \
    establishdialog.ui \
    insertdialog.ui \
    login.ui \
    modifydialog.ui \
    removedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
