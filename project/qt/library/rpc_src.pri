
RPC_SRC_DIR = $${RPC_PROJECT_DIR}/src

INCLUDEPATH += $${RPC_SRC_DIR}

HEADERS +=  \
            $${RPC_INCLUDE_DIR}/rpc/traits/zip_unzip.hpp \
            $${RPC_INCLUDE_DIR}/rpc/traits/built_in_types.hpp \
            $${RPC_INCLUDE_DIR}/rpc/traits/call_with_tuple_args.hpp \
            \
            $${RPC_SRC_DIR}/consts.h \
            \
            $${RPC_SRC_DIR}/system/pipe.h \

SOURCES +=  \
            $${RPC_SRC_DIR}/client.cpp \
            $${RPC_SRC_DIR}/server.cpp \
            \
            $${RPC_SRC_DIR}/consts.cpp \

win32: SOURCES += $${RPC_SRC_DIR}/system/pipe_win.cpp
unix:  SOURCES += $${RPC_SRC_DIR}/system/pipe_unix.cpp

