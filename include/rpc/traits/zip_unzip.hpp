
#ifndef RPC_TRAITS_ZIP_UNZIP_H
#define RPC_TRAITS_ZIP_UNZIP_H


#include <tuple>
#include <cassert>
#include <algorithm>
#include <type_traits>

#include <rpc/types.h>

#include <rpc/traits/built_in_types.hpp>
#include <rpc/traits/call_with_tuple_args.hpp>


namespace rpc { namespace traits
{


namespace impl
{

    inline void zip(buffer_bytes&)
    { }

    template<typename arg_t>
    inline void zip(buffer_bytes& buffer, arg_t&& arg)
    {
        rpc::traits::types::zip(buffer, std::forward<arg_t>(arg));
    }

    template<class first_arg_t, class... other_args_t>
    inline void zip(buffer_bytes& buffer, first_arg_t&& first_arg, other_args_t&&... other_args)
    {
        zip<first_arg_t>(buffer, std::forward<first_arg_t>(first_arg));
        zip<other_args_t...>(buffer, std::forward<other_args_t>(other_args)...);
    }


    inline const rpc::byte* unzip(const rpc::byte* in, const rpc::byte*)
    {
        return in;
    }

    template<class arg_t>
    inline const rpc::byte* unzip(const rpc::byte* in, const rpc::byte* in_end, arg_t& arg)
    {
        return rpc::traits::types::unzip(arg, in, in_end);
    }

    template<class next_arg_t, class... other_args_t>
    inline const rpc::byte* unzip(const rpc::byte* in, const rpc::byte* in_end, next_arg_t& next_arg, other_args_t&... other_args)
    {
        return rpc::traits::impl::unzip<other_args_t...>(rpc::traits::impl::unzip<next_arg_t>(in, in_end, next_arg), in_end, other_args...);
    }

    template<class args_t, unsigned... indexes>
    inline const rpc::byte* unzip(const rpc::byte* in, const rpc::byte* in_end, args_t& args, const sequence<indexes...>&)
    {
        (void)args;
        return rpc::traits::impl::unzip(in, in_end, std::get<indexes>(args)...);
    }

} // namespace impl


template<class... args_t>
inline buffer_bytes zip_args(args_t&&... args)
{
    buffer_bytes buffer;
    rpc::traits::impl::zip(buffer, std::forward<args_t>(args)...);
    return buffer;
}

template<class args_t>
inline args_t unzip_args(const rpc::buffer_bytes& buffer_in)
{
    return unzip_args<args_t>(std::begin(buffer_in), std::end(buffer_in));
}

template<class args_t>
inline args_t unzip_args(const rpc::byte* in, const rpc::byte* in_end)
{
    args_t args;

    const rpc::byte* in_finish(rpc::traits::impl::unzip(in, in_end, args, rpc::traits::impl::make_sequence<std::tuple_size<args_t>::value>()));

    if (in_finish == nullptr)
        return args_t(); //throw std::runtime_error("unzip args error");

    if (in_finish != in_end)
        return args_t(); //throw std::runtime_error("unzip args error");

    return args;
}


template<class result_t>
inline buffer_bytes zip_result(result_t&& result)
{
    buffer_bytes buffer;
    rpc::traits::impl::zip(buffer, std::forward<result_t>(result));
    return buffer;
}

template<class result_t>
inline result_t unzip_result(buffer_bytes&& buffer_result)
{
    result_t result;

    const rpc::byte* in(buffer_result.data());
    const rpc::byte* in_end(in + buffer_result.size());

    const rpc::byte* in_finish(rpc::traits::types::unzip(result, in, in_end));

    if (in_finish == nullptr)
        return result_t(); //throw std::runtime_error("unzip result error");

    if (in_finish != in_end)
        return result_t(); //throw std::runtime_error("unzip result error");

    return result;
}

template<>
inline void unzip_result(buffer_bytes&& /*buffer_result*/)
{ }


}} // nmaespace rpc::traits


#endif // RPC_TRAITS_ZIP_UNZIP_H






