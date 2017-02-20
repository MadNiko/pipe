

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include <rpc.h>


enum enum_1
{
    enum_1_value_1,
    enum_1_value_2,
    enum_1_value_3,
};
const char* enum_to_str(enum_1 e)
{
    return e == enum_1_value_1 ? "enum_1::value_1" :
           e == enum_1_value_2 ? "enum_1::value_2" :
           e == enum_1_value_3 ? "enum_1::value_3" : "<unknown>";
}

enum class enum_2
{
    value_1,
    value_2,
    value_3,
};
const char* enum_to_str(enum_2 e)
{
    return e == enum_2::value_1 ? "enum_2::value_1" :
           e == enum_2::value_2 ? "enum_2::value_2" :
           e == enum_2::value_3 ? "enum_2::value_3" : "<unknown>";
}

enum enum_3 : int
{
    enum_3_value_1,
    enum_3_value_2,
    enum_3_value_3,
};
const char* enum_to_str(enum_3 e)
{
    return e == enum_3_value_1 ? "enum_3::value_1" :
           e == enum_3_value_2 ? "enum_3::value_2" :
           e == enum_3_value_3 ? "enum_3::value_3" : "<unknown>";
}


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


int int__int_bool_char(int v1, bool v2, char v3)
{
    std::cout << "int__int_bool_char(" << v1 << ", " << v2 << ", '" << v3 << "') -> ";

    return 3;
}
RPC_FUNCTION_DECLARE(int__int_bool_char)


bool bool__char_e3(char v1, enum_3 v2)
{
    std::cout << "bool__char_e3('" << v1 << "', " << enum_to_str(v2) << ") -> ";

    return false;
}
RPC_FUNCTION_DECLARE(bool__char_e3)


enum_1 e1__e2_e3(enum_2 e2, enum_3 e3)
{
    std::cout << "e1__e2_e3(";
    std::cout << enum_to_str(e2);
    std::cout << ", ";
    std::cout << enum_to_str(e3);
    std::cout << ") -> ";

    return enum_1_value_2;
}
RPC_FUNCTION_DECLARE(e1__e2_e3)


void void__e1_e2_e3(enum_1 e1, enum_2 e2, enum_3 e3)
{
    std::cout << "void__e1_e2_e3(";
    std::cout << enum_to_str(e1);
    std::cout << ", ";
    std::cout << enum_to_str(e2);
    std::cout << ", ";
    std::cout << enum_to_str(e3);
    std::cout << ")";
    std::cout << std::endl;
}
RPC_FUNCTION_DECLARE(void__e1_e2_e3)


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

    std::cout << rpc::invoke::int__int_bool_char(client, 33, true, 'X') << std::endl;

    std::cout << rpc::invoke::bool__char_e3(client, enum_3_value_2) << std::endl;

    std::cout << enum_to_str(rpc::invoke::e1__e2_e3(client, enum_2::value_3, enum_3_value_1)) << std::endl;

    rpc::invoke::void__e1_e2_e3(client, enum_1_value_3, enum_2::value_1, enum_3_value_2);
}











void void__string(std::string v1)
{
    std::cout << "void__string(\"" << v1 << "\")";
}
RPC_FUNCTION_DECLARE(void__string)

std::wstring wstring__string(std::string v1)
{
    std::cout << "wstring__string(\"" << v1 << "\") -> ";
    return std::wstring(L"123dsdf");
}
RPC_FUNCTION_DECLARE(wstring__string)

void test_custom_type()
{
    rpc::invoke::void__string(L"pipe_channel_name", std::string("abcdef"));
    std::cout << std::endl;

    std::wcout << rpc::invoke::wstring__string(L"pipe_channel_name", std::string("45fghgf")) << std::endl;

}



struct data
{
    int v;
};
static void zip_data(rpc::buffer_bytes& buffer, data&& value)
{
    rpc::traits::types::zip(buffer, std::move(value.v));
}
static const rpc::byte* unzip_data(data& value, const rpc::byte* in, const rpc::byte* in_end)
{
    return rpc::traits::types::unzip(value.v, in, in_end);
}
RPC_TYPE_DECLARE(data, zip_data, unzip_data)




void void__data(data d)
{
    std::cout << "void_data(data{" << d.v << "})" << std::endl;
}
RPC_FUNCTION_DECLARE(void__data)


void test_custom_type__data()
{
    data d{13};
    rpc::invoke::void__data(L"pipe_channel_name", std::move(d));
}


template<class item_t, template<class T, class Allocator = std::allocator<T>> class container_t>
inline std::string print(const container_t<item_t>& container)
{
    std::stringstream ss;
    std::size_t count(0);
    for (const item_t& item : container)
    {
        if (++count > 1) ss << ", ";
        ss << item;
    }
    return ss.str();
}

void void__vector(std::vector<int> vi)
{
    std::cout << "void__vector(std::vector{" << print(vi) << "})" << std::endl;
}
//RPC_FUNCTION_DECLARE(void__vector)

void test_std_containers()
{
    std::vector<int> vi{1,9,4};
    void__vector(std::move(vi));
    //rpc::invoke::void__vector(L"pipe_channel_name", std::move(vi));
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

    test_custom_type();

    test_custom_type__data();

    test_std_containers();

    return 0;
}





