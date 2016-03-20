

#include <string>
#include <iostream>

#include <rpc.h>


//using std_string = std::string;
//static void zip_std_string(rpc::buffer_bytes& buffer, std_string&& str)
//{
//    const rpc::buffer_bytes::size_type size_size(sizeof(std_string::size_type));
//    const std_string::size_type size_str(str.size());

//    buffer.reserve(buffer.size() + size_size + (size_str * sizeof(std_string::value_type)));

//    rpc::traits::types::zip(buffer, size_size);
//    for (const std_string::value_type& value : str)
//    {
//        rpc::traits::types::zip(buffer, value);
//    }
//}
//static const rpc::byte* unzip_std_string(std_string& str, const rpc::byte* in, const rpc::byte* in_end)
//{
//    const rpc::buffer_bytes::size_type size_size(0);
//    in = rpc::traits::types::unzip(size_size, in, in_end);
//    if (in == nullptr)
//        return nullptr;

//    str.resize(size_size);

//    for (std_string::value_type& value : str)
//    {
//        in = rpc::traits::types::unzip(value, in, in_end);
//        if (in == nullptr)
//            return nullptr;
//    }

//    return in;
//}
////RPC_TYPE_DECLARE(std_string, zip_std_string, unzip_std_string)


void void__void()
{
    std::cout << "void__void()" << std::endl;
}
RPC_FUNCTION_DECLARE(void__void)


void void__int(int v)
{
    std::cout << "void__int(" << v << ")" << std::endl;
}
RPC_FUNCTION_DECLARE(void__int)


int int__void()
{
    std::cout << "int__void() -> ";

    return 1;
}
RPC_FUNCTION_DECLARE(int__void)


int int__string_int(std::string v1, int v2)
{
    std::cout << "int__string_int(\"" << v1 << "\", " << v2 << ") -> ";
    return 2;
}
RPC_FUNCTION_DECLARE(int__string_int)


int int__int_bool_char(int v1, bool v2, char v3)
{
    std::cout << "int__int_bool_char(" << v1 << ", " << v2 << ", " << v3 << ") -> ";

    return 3;
}
RPC_FUNCTION_DECLARE(int__int_bool_char)


void main_server()
{
    rpc::server rpc_server(L"pipe_channel_name");

    rpc_server.register_lambda("no_declared_before_rpc_call", [](const rpc::byte*, const rpc::byte*)
    {
        return rpc::buffer_bytes();
    });

    rpc_server.start_in_this_thread(true);
}

void main_client()
{
    rpc::client client(std::wstring(L"pipe_channel_name"));

    rpc::client(L"pipe_channel_name").invoke("no_declared_before_rpc_call", rpc::buffer_bytes());

    rpc::invoke::void__void(client);

    rpc::invoke::void__int(client, 11);

    std::cout << rpc::invoke::int__void(client) << std::endl;

    //std::cout << rpc::invoke::int__string_int(client, std::string("abcdef"), 22) << std::endl;

    std::cout << rpc::invoke::int__int_bool_char(client, 33, true, 'X') << std::endl;
}


int main()
{
    //char ch;
    //std::cout << "Enter: ";
    //std::cin  >> ch;
    //std::cout << " . . . continue";
    //std::cout << std::endl;

    std::cout << std::boolalpha;

    //main_server();

    main_client();

    return 0;
}
