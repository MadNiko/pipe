
#include <cassert>
#include <unordered_map>

#include <rpc/server.h>

#include "consts.h"
#include "system/pipe.h"


namespace rpc
{


namespace
{


struct call_meta
{
    server::call_func func = nullptr;
    server::call_lambda lambda;
};

using calls_map = std::unordered_map<server::call_id, call_meta>;


} // namespace


struct server::data
{
    system::pipe_server pipe;

    calls_map calls;
};


server::server()
    : m_data(nullptr)
{ }

server::server(server&& other)
    : m_data(nullptr)
{
    std::swap(m_data, other.m_data);
}

server::server(const std::wstring& local_pipe_name)
    : server(L".", local_pipe_name)
{ }

server::server(const std::wstring& server_name, const std::wstring& pipe_name)
    : m_data(new data())
{
    try
    {
        m_data->pipe = system::pipe_server::create(server_name, pipe_name);
    }
    catch(...)
    {
        erase(m_data);
        throw;
    }
}

server::~server()
{
    erase(m_data);
}

server& server::operator=(server&& other)
{
    server::data* data(nullptr);
    std::swap(data, other.m_data);
    std::swap(data, m_data);
    erase(data);

    return *this;
}

void server::register_func(const server::call_id& call_id, server::call_func call_func)
{
    if (!m_data)
        throw std::runtime_error("bad object");

    if (!call_func)
        throw std::runtime_error("pointer on function an empty");

    call_meta& meta_ref(m_data->calls[call_id]);
    if (!meta_ref.func && !meta_ref.lambda)
        throw std::runtime_error("id calling already used");

    meta_ref.func = call_func;
}

void server::register_lambda(const server::call_id& call_id, server::call_lambda&& call_lambda)
{
    if (!m_data)
        throw std::runtime_error("bad object");

    if (!call_lambda)
        throw std::runtime_error("object lambda an empty");

    call_meta& meta_ref(m_data->calls[call_id]);
    if (!meta_ref.func && !meta_ref.lambda)
        throw std::runtime_error("id calling already used");

    meta_ref.lambda = std::move(call_lambda);
}

void server::start_in_this_thread(bool only_one_connection)
{
    (void)only_one_connection;

    if (!m_data)
        throw std::runtime_error("bad object");

    m_data->pipe.wait_connect();

    buffer_bytes buffer_in;

    if (!m_data->pipe.read(buffer_in))
        throw std::runtime_error("error read data");

    if (buffer_in.empty())
        throw std::runtime_error("bad data");

    const byte* in(buffer_in.data());
    const byte* in_end(in + buffer_in.size());

    const call_id id(reinterpret_cast<const char*>(in));

    in += id.size();

    assert(in <= in_end);

    const calls_map::const_iterator it(m_data->calls.find(id));
    if (it == std::end(m_data->calls))
        throw std::runtime_error("id calling already used");

    const call_meta& meta(it->second);

    buffer_bytes buffer_out;
    try
    {
        if (meta.func)
        {
            buffer_out = meta.func(in, in_end);
        }
        else
        {
            assert(meta.lambda);
            buffer_out = meta.lambda(in, in_end);
        }
        buffer_out.insert(std::begin(buffer_out), response_first_byte::success);
    }
    catch(const std::exception& except)
    {
        (void)except;

        buffer_out.push_back(response_first_byte::exception);
        //buffer_out.push_back(std::string(except.what()));
    }

    if (!m_data->pipe.write(std::move(buffer_out)))
        throw std::runtime_error("error response");
}

void server::stop()
{
    //TODO
}


void server::erase(server::data* data)
{
    if (!data)
        return;

    //TODO

    delete data;
}


} // nmaespace rpc
