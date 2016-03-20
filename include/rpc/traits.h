
#ifndef RPC_TRAITS_H
#define RPC_TRAITS_H


#include <rpc/traits/zip_unzip.hpp>
#include <rpc/traits/built_in_types.hpp>
#include <rpc/traits/call_with_tuple_args.hpp>


namespace rpc { namespace traits
{


template<class f>
struct func;

template<class result_t, class... args_t>
struct func<result_t(*)(args_t...)> : public func<result_t(args_t...)>
{ };

template<typename result_t, typename... args_t>
struct func<result_t(args_t...)>
{
    using result = result_t;
    using args = std::tuple<args_t...>;
};

template<class... params>
using func_result_t = typename func<params...>::result;

template<class... params>
using func_args_t = typename func<params...>::args;


}} // nmaespace rpc::traits


#endif // RPC_TRAITS_H






