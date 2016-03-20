
include($${PWD}/rpc_config.pri)

TEMPLATE = lib

TARGET = $${RPC_TARGET_NAME}

DESTDIR = $${RPC_OUTPUT_DIR}

CONFIG -= qt
#CONFIG += staticlib


DEFINES += RPC_BUILD
CONFIG(staticlib): DEFINES += RPC_LIB_STATIC


include($${PWD}/rpc_include.pri)
include($${PWD}/rpc_src.pri)
