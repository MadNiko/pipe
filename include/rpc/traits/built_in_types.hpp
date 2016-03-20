
#ifndef RPC_TRAITS_BUILT_IN_TYPES_H
#define RPC_TRAITS_BUILT_IN_TYPES_H


#include <tuple>
#include <cassert>
#include <algorithm>
#include <type_traits>

#include <rpc/types.h>


namespace rpc { namespace traits
{


namespace types
{


template<class value_t>
using func_zip_t = void(buffer_bytes& buffer, value_t&& value);

template<class value_t>
using func_unzip_t = const rpc::byte*(value_t& value, const rpc::byte* in, const rpc::byte* in_end);


//template<class value_t> void zip(rpc::buffer_bytes& buffer, value_t&& value);
//template<class value_t> const rpc::byte* unzip(value_t& value, const rpc::byte* in, const rpc::byte* in_end);


template<class type>
static
inline void zip_binary_data(buffer_bytes& buffer, const type* value)
{
    assert(value);

    const std::size_t size_type(sizeof(type));

    const rpc::byte* byte_in(reinterpret_cast<const rpc::byte*>(value));

    const buffer_bytes::size_type size_before(buffer.size());

    buffer.resize(size_before + size_type, 0x00);

    std::memcpy((buffer.data() + size_before), byte_in, size_type);
}

template<class type>
static
inline const rpc::byte* unzip_binary_data(type* value, const rpc::byte* in, const rpc::byte* in_end)
{
    assert(value);
    assert(in && in_end);

    const std::size_t size_type(sizeof(type));

    const rpc::byte* const in_next_value(in + size_type);

    if (in_end < in_next_value)
        return nullptr;

    rpc::byte* const bytes(reinterpret_cast<rpc::byte*>(value));

    std::memcpy(bytes, in, size_type);

    return in_next_value;
}


//======================================================================================================================
// (sign/unsign) char,  (sign/unsign) char16_t, (sign/unsign) char32_t, (sign/unsign) wchar_t,
// (sign/unsign) short, (sign/unsign) int,      (sign/unsign) long,     (sign/unsign) long long
//----------------------------------------------------------------------------------------------------------------------
template<class value_t>
static
typename std::enable_if<(std::is_integral<typename std::decay<value_t>::type>::value && !std::is_same<typename std::decay<value_t>::type, bool>::value), void>::type
zip(buffer_bytes& buffer, value_t&& value)
{
    zip_binary_data(buffer, &value);
}
template<class value_t>
static
typename std::enable_if<(std::is_integral<typename std::decay<value_t>::type>::value && !std::is_same<typename std::decay<value_t>::type, bool>::value), const rpc::byte*>::type
unzip(value_t& value, const rpc::byte* in, const rpc::byte* in_end)
{
    return unzip_binary_data(&value, in, in_end);
}


//======================================================================================================================
// enum's
//----------------------------------------------------------------------------------------------------------------------
template<class value_t>
static
typename std::enable_if<std::is_enum<value_t>::value, void>::type
zip(buffer_bytes& buffer, value_t&& value)
{
    using underlying_type = typename std::underlying_type<value_t>::type;
    const underlying_type tmp_value(value);
    zip_binary_data(buffer, &tmp_value);
}
template<class value_t>
static
typename std::enable_if<std::is_enum<value_t>::value, const rpc::byte*>::type
unzip(value_t& value, const rpc::byte* in, const rpc::byte* in_end)
{
    typename std::underlying_type<value_t>::type tmp_value(0);
    const rpc::byte* in_next(unzip_binary_data(&tmp_value, in, in_end));
    if (in_next != nullptr)
        value = (0 != tmp_value);
    return in_next;
}


//======================================================================================================================
// bool
//----------------------------------------------------------------------------------------------------------------------
template<class value_t>
static
typename std::enable_if<std::is_same<typename std::decay<value_t>::type, bool>::value, void>::type
zip(buffer_bytes& buffer, value_t&& value)
{
    const rpc::byte tmp_value(value ? -1 : 0);
    zip_binary_data(buffer, &tmp_value);
}
template<class value_t>
static
//typename std::enable_if<std::is_same<typename std::decay<value_t>::type, bool>::value, const rpc::byte*>::type
typename std::enable_if<std::is_same<value_t, bool>::value, const rpc::byte*>::type
unzip(value_t& value, const rpc::byte* in, const rpc::byte* in_end)
{
    rpc::byte tmp_value(0);
    const rpc::byte* in_next(unzip_binary_data(&tmp_value, in, in_end));
    if (in_next)
        value = (0 != tmp_value);
    return in_next;
}


} // namespace types


}} // nmaespace rpc::traits


#endif // RPC_TRAITS_BUILT_IN_TYPES_H






