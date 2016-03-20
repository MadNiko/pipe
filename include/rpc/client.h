
#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H


#include <rpc/defines.h>
#include <rpc/traits.h>
#include <rpc/types.h>


namespace rpc
{


class RPC_EXPORT client
{

public:

    client();
    client(client&& other);
    client(const std::wstring& local_pipe_name);
    client(const std::wstring& server_name, const std::wstring& pipe_name);
    ~client();

    client& operator=(client&& other);

    client(const client& other) = delete;
    client& operator=(const client& other) = delete;

public:

    buffer_bytes invoke(const std::string& call_id, const buffer_bytes& buffer_in);

private:

    struct data;
    static void erase(data*);
    data* m_data;
};


} // nmaespace rpc


#endif // RPC_CLIENT_H
