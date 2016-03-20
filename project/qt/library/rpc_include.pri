
RPC_INCLUDE_DIR = $${RPC_PROJECT_DIR}/include

INCLUDEPATH += $${RPC_INCLUDE_DIR}

HEADERS +=  \
            $${RPC_INCLUDE_DIR}/rpc.h \
            $${RPC_INCLUDE_DIR}/rpc/defines.h \
            $${RPC_INCLUDE_DIR}/rpc/traits.h \
            $${RPC_INCLUDE_DIR}/rpc/types.h \
            $${RPC_INCLUDE_DIR}/rpc/client.h \
            $${RPC_INCLUDE_DIR}/rpc/server.h \
            $${RPC_INCLUDE_DIR}/rpc/macro.h \
