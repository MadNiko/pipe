
#ifndef RPC_SRC_SYSTEM_PIPE_H
#define RPC_SRC_SYSTEM_PIPE_H


#include <memory>

#include <rpc/types.h>


namespace rpc { namespace system
{


class pipe
{
public:

    enum class error : unsigned long
    {
        broken                           =  109,     //!< The pipe has been ended.
        pipe_local                       =  229,     //!< The pipe is local.
        bad_pipe                         =  230,     //!< The pipe state is invalid.
        pipe_busy                        =  231,     //!< All pipe instances are busy.
        no_data                          =  232,     //!< The pipe is being closed.
        pipe_not_connected               =  233,     //!< No process is on the other end of the pipe.
        pipe_connected                   =  535,     //!< There is a process on other end of the pipe.
        pipe_listening                   =  536,     //!< Waiting for a process to open the other end of the pipe.
        cannot_impersonate               = 1368,     //!< Unable to impersonate using a named pipe until data has been read from that pipe.
        rpc_x_invalid_pipe_object        = 1830,     //!< The RPC pipe object is invalid or corrupted.
        rpc_x_wrong_pipe_order           = 1831,     //!< An invalid operation was attempted on an RPC pipe object.
        rpc_x_wring_pipe_version         = 1832,     //!< Unsupported RPC pipe version.
        rpc_x_pipe_closed                = 1916,     //!< The RPC pipe object has already been closed.
        rpc_x_discipline_error           = 1917,     //!< The RPC call completed before all pipes were processed.
        rpc_x_pipe_empty                 = 1918,     //!< No more data is available from the RPC pipe.
    };

public:

    pipe();
    pipe(pipe&& other);
    ~pipe();

    pipe& operator=(pipe&& other);

    pipe(const pipe&& other) = delete;
    pipe& operator=(const pipe&& other) = delete;

public:

    bool write(buffer_bytes&& buffer);

    bool read(buffer_bytes& buffer);

    error last_error();

protected:

    struct data;
    using data_ptr = std::unique_ptr<data, void(*)(data*)>;
    static void erase(data* data);

    data_ptr m_data;
};


class pipe_client final : public pipe
{
public:

    static pipe_client connect(const std::wstring& server_name, const std::wstring& pipe_name);

public:

    pipe_client();
    pipe_client(pipe_client&& other);
    ~pipe_client();

    pipe_client& operator=(pipe_client&& other);

public:

    void disconnect();
};


class pipe_server final : public pipe
{
public:

    static pipe_server create(const std::wstring& server_name, const std::wstring& pipe_name);

public:

    pipe_server();
    pipe_server(pipe_server&& other);
    ~pipe_server();

    pipe_server& operator=(pipe_server&& other);

public:

    bool wait_connect();
};


} } // namespace rpc::system


#endif // RPC_SRC_SYSTEM_PIPE_H
