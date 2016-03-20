

#include <rpc/client.h>

#include "system/pipe.h"


namespace rpc
{


struct client::data
{
    system::pipe_client pipe;
};


client::client()
    : m_data(nullptr)
{ }

client::client(client&& other)
    : m_data(nullptr)
{
    std::swap(m_data, other.m_data);
}

client::client(const std::wstring& local_pipe_name)
    : client(L".", local_pipe_name)
{ }

client::client(const std::wstring& server_name, const std::wstring& pipe_name)
    : m_data(new data())
{
    try
    {
        m_data->pipe = system::pipe_client::connect(server_name, pipe_name);
    }
    catch(...)
    {
        erase(m_data);
        throw;
    }
}

client::~client()
{
    erase(m_data);
}

client& client::operator=(client&& other)
{
    client::data* data(nullptr);
    std::swap(data, other.m_data);
    std::swap(data, m_data);
    erase(data);

    return *this;
}

buffer_bytes client::invoke(const std::string& call_id, const buffer_bytes& buffer_in)
{
    (void)call_id;
    (void)buffer_in;

    //TODO

    return buffer_bytes();
}

void client::erase(client::data* data)
{
    if (!data)
        return;

    data->pipe.disconnect();

    delete data;
}


} // nmaespace rpc
