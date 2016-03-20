
#ifndef RPC_SERVER_H
#define RPC_SERVER_H


#include <functional>

#include <rpc/defines.h>
#include <rpc/types.h>


namespace rpc
{


class RPC_EXPORT server
{

public:

    using call_id = std::string;
    using call_func = buffer_bytes(*)(const byte* in, const byte* in_end);
    using call_lambda = std::function<buffer_bytes(const byte* in, const byte* in_end)>;

public:

    server();
    server(server&& other);
    server(const std::wstring& local_pipe_name);
    server(const std::wstring& server_name, const std::wstring& pipe_name);
    ~server();

    server& operator=(server&& other);

    server(const server& other) = delete;
    server& operator=(const server& other) = delete;

public:

    void register_func(const call_id& call_id, call_func call_func);
    void register_lambda(const call_id& call_id, call_lambda&& call_lambda);

    void start_in_this_thread(bool only_one_connection);

    void stop();

public: // In progress

    struct guad_thread
    {
        void wait_finished() const;

        void stop();
    };

    guad_thread start_in_separate_thread(std::size_t max_number_sub_threads = 0);

private:

    struct data;
    static void erase(data*);
    data* m_data;
};


} // nmaespace rpc


#endif // RPC_SERVER_H


