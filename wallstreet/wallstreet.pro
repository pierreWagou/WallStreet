QT += widgets charts
QT += xml

ICON = image/icon.icns

RC_ICONS = image/icon.ico

QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    bougie.cpp \
    configuration.cpp \
    coursOHLC.cpp \
    devise.cpp \
    deviseManager.cpp \
    editeurTexte.cpp \
    evolutionCours.cpp \
    evolutionViewer.cpp \
    strategieFactory.cpp \
    home.cpp \
    main.cpp \
    paireDevise.cpp \
    strategie.cpp \
    strategieSelection.cpp \
    tradingException.cpp \
    transactionManager.cpp \
    transaction.cpp \
    transactionViewer.cpp

HEADERS += \
    bougie.h \
    configuration.h \
    coursOHLC.h \
    devise.h \
    deviseManager.h \
    editeurTexte.h \
    evolutionCours.h \
    evolutionViewer.h \
    strategieFactory.h \
    home.h \
    paireDevise.h \
    strategie.h \
    strategieSelection.h \
    tradingException.h \
    transactionManager.h \
    transaction.h \
    transactionViewer.h

RESOURCES += \
    resource.qrc
