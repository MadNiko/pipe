
RPC_TARGET_NAME = rpc

RPC_VERSION_MAJOR = 0
RPC_VERSION_MINOR = 1
#RPC_VERSION = $${RPC_VERSION_MAJOR}.$${RPC_VERSION_MINOR}

DEFINES += RPC_VERSION_MAJOR=$${RPC_VERSION_MAJOR}
DEFINES += RPC_VERSION_MINOR=$${RPC_VERSION_MINOR}
#DEFINES += RPC_VERSION=$${RPC_VERSION}

CONFIG *= c++11

RPC_PROJECT_DIR = $${PWD}/../../../

RPC_OUTPUT_DIR = $${RPC_PROJECT_DIR}/output
