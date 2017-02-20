
#include <cassert>

#include <rpc/defines.h>
#if defined(RPC_CC_MSVC)
#   include <windows.h>
#elif defined(RPC_CC_MINGW)
#   include <windef.h>
#   include <winbase.h>
#   include <fileapi.h>
#   include <handleapi.h>
#endif


#include "system/pipe.h"



namespace
{


inline std::wstring make_pipe_name(const std::wstring& server_name, const std::wstring& pipe_name)
{
    return std::wstring(LR"(\\)").append(server_name).append(LR"(\pipe\)").append(pipe_name);
}


} // namespace



namespace rpc { namespace system
{


struct pipe::data
{
    std::wstring name;

    HANDLE handle = INVALID_HANDLE_VALUE;
};


pipe::pipe()
    : m_data(nullptr, &pipe::erase)
{ }

pipe::pipe(pipe&& other)
    : m_data(nullptr, &pipe::erase)
{
    std::swap(m_data, other.m_data);
}

pipe::~pipe()
{ }

void pipe::erase(pipe::data* data)
{
    if (!data)
        return;

    if (data->handle != INVALID_HANDLE_VALUE)
        ::CloseHandle(data->handle);

    delete data;
}

pipe& pipe::operator=(pipe&& other)
{
    data_ptr data(nullptr, &pipe::erase);
    std::swap(data, other.m_data);
    std::swap(data, m_data);

    return *this;
}

bool pipe::write(buffer_bytes&& buffer)
{
    if (!m_data)
        return false;

    (void)buffer;

    return true;
}

bool pipe::read(buffer_bytes& buffer)
{
    if (!m_data)
        return false;

    (void)buffer;

    return true;
}

pipe::error pipe::last_error()
{
    if (!m_data)
        return error::bad_pipe;


    return error::bad_pipe;
}






pipe_client pipe_client::connect(const std::wstring& server_name, const std::wstring& pipe_name)
{
    system::pipe_client pipe_client;
    pipe_client.m_data.reset(new system::pipe::data());
    pipe_client.m_data->name = make_pipe_name(server_name, pipe_name);
    pipe_client.m_data->handle =
            ::CreateFileW(
                reinterpret_cast<LPCWSTR>(pipe_client.m_data->name.c_str()),
                (GENERIC_READ | GENERIC_WRITE),
                0,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);

    return pipe_client;
}

pipe_client::pipe_client()
    : pipe()
{ }

pipe_client::pipe_client(pipe_client&& other)
    : pipe(std::move(other))
{ }

pipe_client::~pipe_client()
{ }

pipe_client& pipe_client::operator=(pipe_client&& other)
{
    pipe::operator=(std::move(other));
    return *this;
}

void pipe_client::disconnect()
{
    if (!m_data)
        return;

    //if (m_data->handle != PlatformAPI::InvalidHandleValue)
    //{
    //    PlatformAPI::Handle handle(PlatformAPI::InvalidHandleValue);
    //    std::swap(handle, m_data.handle);
    //    PlatformAPI::Pipe::disconnect(handle);
    //    PlatformAPI::closeHandle(handle);
    //}
}









pipe_server pipe_server::create(const std::wstring& server_name, const std::wstring& pipe_name)
{
    system::pipe_server pipe_server;
    pipe_server.m_data.reset(new system::pipe::data());
    pipe_server.m_data->name = make_pipe_name(server_name, pipe_name);
    //pipe_server.m_data->handle =
    //        ::CreateNamedPipeW(
    //            reinterpret_cast<LPCWSTR>(pipe_server.m_data->name.c_str()),
    //            openMode,
    //            pipeMode,
    //            maxInstances,
    //            outBufferSize,
    //            inBufferSize,
    //            defaultTimeOut,
    //            nullptr);

    return pipe_server;
}

pipe_server::pipe_server()
    : pipe()
{ }

pipe_server::pipe_server(pipe_server&& other)
    : pipe(std::move(other))
{ }

pipe_server::~pipe_server()
{ }

pipe_server& pipe_server::operator=(pipe_server&& other)
{
    pipe::operator=(std::move(other));
    return *this;
}

bool pipe_server::wait_connect()
{
    if (!m_data)
        return false;

    return (FALSE != ::ConnectNamedPipe(m_data->handle, nullptr));
}


}} // namespace rpc::system
