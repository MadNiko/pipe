
include($${PWD}/../library/rpc_config.pri)
include($${PWD}/../library/rpc_include.pri)

RPC_TESTS_DIR = $${PWD}/../../../tests


TEMPLATE = app

TARGET = $${RPC_TARGET_NAME}_tests

DESTDIR = $${RPC_OUTPUT_DIR}

QMAKE_LIBDIR += $${RPC_OUTPUT_DIR}

LIBS += -l$${RPC_TARGET_NAME}

INCLUDEPATH += $${RPC_TESTS_DIR}

SOURCES += $${RPC_TESTS_DIR}/main.cpp
