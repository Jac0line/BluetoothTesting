TEMPLATE = app
TARGET = btchat

QT = core bluetooth widgets multimedia
requires(qtConfig(listwidget))
android: QT += androidextras

SOURCES = \
    AudioFileStream.cpp \
    audiooutput.cpp \
    main.cpp \
    chat.cpp \
    remoteselector.cpp \
    chatclient.cpp
    audiooutput.cpp
    AudioFileStream.cpp

HEADERS = \
    AudioFileStream.h \
    audiooutput.h \
    chat.h \
    remoteselector.h \
    chatclient.h
    audiooutput.h
    AudioFileStream.h

FORMS = \
    chat.ui \
    remoteselector.ui

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/btchat
INSTALLS += target
