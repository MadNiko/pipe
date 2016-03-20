
#ifndef RPC_MACRO_H
#define RPC_MACRO_H


#include <rpc/traits.h>
#include <rpc/server.h>
#include <rpc/client.h>


namespace rpc
{
    //

    namespace traits
    {
        //

        namespace types
        {
            //
        }
    }

    namespace processing
    {
        //
    }

    namespace invoke
    {
        //
    }

} // namespace rpc


#define RPC_TYPE_DECLARE(zip_type, func_zip, func_unzip) \
    \
    namespace rpc \
    { \
        namespace traits \
        { \
            namespace types \
            { \
                template<class value_t> \
                static \
                typename std::enable_if<std::is_same<value_t, zip_type>::value, void>::type \
                zip(buffer_bytes& buffer, zip_type&& value) \
                { \
                    func_zip(buffer, std::forward<zip_type>(value)); \
                } \
                \
                template<class value_t> \
                static \
                typename std::enable_if<std::is_same<value_t, typename std::decay<zip_type>::type>::value, const rpc::byte*>::type \
                unzip(zip_type& value, const rpc::byte* in, const rpc::byte* in_end) \
                { \
                    return func_unzip(value, in, in_end); \
                } \
            } \
        } \
    }


#define RPC_REG_FUNCTION_IN_SERVER(server, func_t) \
    rpc::processing::_register_##func_t<std::is_void<rpc::traits::func_result_t<decltype(func_t)>>::value> \
        (server, #func_t, rpc::processing::void_##func_t, rpc::processing::result_##func_t)



#define RPC_PROCESSING_DECLARE(func_t) \
    \
    namespace rpc \
    { \
        namespace processing \
        { \
            template<class result_t> \
            rpc::buffer_bytes func_t(const rpc::byte* in, const rpc::byte* in_end); \
            \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, \
                     class tuple_args_t = typename rpc::traits::func_args_t<decltype(::func_t)>> \
            typename std::enable_if<std::is_void<result_t>::value, rpc::buffer_bytes>::type \
            func_t(const rpc::byte* in, const rpc::byte* in_end) \
            { \
                tuple_args_t tuple_args(rpc::traits::unzip_args<tuple_args_t>(in, in_end)); \
                rpc::traits::call(std::move(tuple_args), ::func_t); \
                return rpc::buffer_bytes(); \
            } \
            \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, \
                     class tuple_args_t = typename rpc::traits::func_args_t<decltype(::func_t)>> \
            typename std::enable_if<!std::is_void<result_t>::value, rpc::buffer_bytes>::type \
            func_t(const rpc::byte* in, const rpc::byte* in_end) \
            { \
                tuple_args_t tuple_args(rpc::traits::unzip_args<tuple_args_t>(in, in_end)); \
                result_t result(rpc::traits::call(std::move(tuple_args), ::func_t)); \
                return rpc::traits::zip_result<result_t>(std::move(result)); \
            } \
        } \
    }


#define RPC_INVOKE_DECLARE(func_t) \
    \
    namespace rpc \
    { \
        namespace invoke \
        { \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, class... args_t> \
            inline result_t \
            func_t(const std::wstring& pipe_channel_name, args_t&&... args) \
            { \
                rpc::client client(pipe_channel_name); \
                return rpc::invoke::func_t(client, std::forward<args_t>(args)...); \
            } \
            \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, class... args_t> \
            inline result_t \
            func_t(rpc::client& client, args_t&&... args) \
            { \
                const std::string call_id(#func_t); \
                return rpc::traits::unzip_result<result_t>(client.invoke(call_id, rpc::traits::zip_args(std::forward<args_t>(args)...))); \
            } \
        } \
    }

#define RPC_LOCAL_INVOKE_DECLARE(func_t) \
    \
    namespace rpc \
    { \
        namespace invoke \
        { \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, class... args_t> \
            inline result_t \
            func_t(const std::wstring& pipe_channel_name, args_t&&... args) \
            { \
                rpc::client client(pipe_channel_name); \
                return rpc::invoke::func_t(client, std::forward<args_t>(args)...); \
            } \
            \
            template<class result_t = typename rpc::traits::func_result_t<decltype(::func_t)>, class... args_t> \
            inline result_t \
            func_t(rpc::client& client, args_t&&... args) \
            { \
                const rpc::buffer_bytes in_buffer(rpc::traits::zip_args(std::forward<args_t>(args)...)); \
                (void)client; /*return rpc::traits::unzip_result<result_t>(client.invoke(#func_t, in_buffer));*/ \
                return rpc::traits::unzip_result<result_t>(rpc::processing::func_t(in_buffer.data(), in_buffer.data() + in_buffer.size())); \
            } \
        } \
    }


#define RPC_FUNCTION_DECLARE(func_t) \
    RPC_PROCESSING_DECLARE(func_t) \
    RPC_LOCAL_INVOKE_DECLARE(func_t)


#endif // RPC_MACRO_H
