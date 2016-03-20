
#ifndef RPC_TRAITS_CALL_WITH_TUPLE_ARGS_H
#define RPC_TRAITS_CALL_WITH_TUPLE_ARGS_H


#include <tuple>
#include <cassert>
#include <algorithm>
#include <type_traits>

#include <rpc/types.h>


namespace rpc { namespace traits
{


namespace impl
{

    /** @brief Вспомагательная структура хранящая последовательность чисел */
    template <unsigned... indexes>
    struct sequence { typedef sequence<indexes...> type; };


    /** @brief Вспомагательная структура для создания последовательности чисел начиная с нуля
     * @tparam count - Колличество чисел в последовательности
     */
    template <unsigned count, unsigned index = 0, unsigned... indexes>
    struct make_sequence : std::conditional<(index < count), make_sequence<count, index + 1, count - index - 1, indexes...>, sequence<indexes...>>::type{};


    template<class args_t, class func_t, unsigned... indexes>
    inline auto call(args_t&& args, const func_t& func, const sequence<indexes...>&)
    -> decltype(func(std::forward<typename std::tuple_element<indexes, args_t>::type>(std::get<indexes>(args))...))
    {
        (void)args;
        return func(std::forward<typename std::tuple_element<indexes, args_t>::type>(std::get<indexes>(args))...);
    }

} // namespace impl


template<class args_t, class func_t>
inline auto call(args_t&& args, const func_t& func)
-> decltype(rpc::traits::impl::call(std::forward<args_t>(args), func, rpc::traits::impl::make_sequence<std::tuple_size<args_t>::value>()))
{
    return rpc::traits::impl::call(std::forward<args_t>(args), func, rpc::traits::impl::make_sequence<std::tuple_size<args_t>::value>());
}


}} // nmaespace rpc::traits


#endif // RPC_TRAITS_CALL_WITH_TUPLE_ARGS_H






