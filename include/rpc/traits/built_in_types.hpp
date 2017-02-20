
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
typename std::enable_if<(std::is_integral<value_t>::value && !std::is_same<value_t, bool>::value), void>::type
zip(buffer_bytes& buffer, value_t&& value)
{
    zip_binary_data(buffer, &value);
}
template<class value_t>
static
typename std::enable_if<(std::is_integral<value_t>::value && !std::is_same<value_t, bool>::value), const rpc::byte*>::type
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
    const underlying_type tmp_value(static_cast<underlying_type>(value));
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
        value = static_cast<value_t>(tmp_value);
    return in_next;
}



//======================================================================================================================
// vector
//----------------------------------------------------------------------------------------------------------------------

template <
    class item,
    class allocator = std::allocator<item>,
    class vector = std::vector<item, allocator>
>
static void zip(buffer_bytes& buffer, vector&& value)
{
    const rpc::buffer_bytes::size_type size_size_vec(sizeof(typename vector::size_type));
    typename vector::size_type size_vec(value.size());

    buffer.reserve(buffer.size() + size_size_vec + (size_vec * sizeof(typename vector::value_type)));

    rpc::traits::types::zip(buffer, std::move(size_vec));
    for (typename vector::value_type& item : value)
    {
        rpc::traits::types::zip(buffer, std::move(item));
    }
}
template <
    class item,
    class allocator = std::allocator<item>,
    class vector = std::vector<item, allocator>
>
static const rpc::byte* unzip(vector& value, const rpc::byte* in, const rpc::byte* in_end)
{
    typename vector::size_type size_vec(0);
    in = rpc::traits::types::unzip(size_vec, in, in_end);
    if (in == nullptr)
        return nullptr;

    value.resize(size_vec);

    for (typename vector::value_type& item : value)
    {
        in = rpc::traits::types::unzip(item, in, in_end);
        if (in == nullptr)
            return nullptr;
    }

    return in;
}



//======================================================================================================================
// bool
//----------------------------------------------------------------------------------------------------------------------
template<class value_t>
static
typename std::enable_if<std::is_same<value_t, bool>::value, void>::type
zip(buffer_bytes& buffer, value_t&& value)
{
    const rpc::byte tmp_value(value ? -1 : 0);
    zip_binary_data(buffer, &tmp_value);
}
template<class value_t>
static
typename std::enable_if<std::is_same<value_t, bool>::value, const rpc::byte*>::type
unzip(value_t& value, const rpc::byte* in, const rpc::byte* in_end)
{
    rpc::byte tmp_value(0);
    const rpc::byte* in_next(unzip_binary_data(&tmp_value, in, in_end));
    if (in_next)
        value = (0 != tmp_value);
    return in_next;
}



//======================================================================================================================
// string's
//----------------------------------------------------------------------------------------------------------------------
namespace impl
{

template<
    class elem,
    class traits = std::char_traits<elem>,
    class alloc = std::allocator<elem>,
    class string = std::basic_string<elem, traits, alloc>
>
inline void zip(rpc::buffer_bytes& buffer, string&& str)
{
    const rpc::buffer_bytes::size_type size_size_str(sizeof(typename string::size_type));
    typename string::size_type size_str(str.size());

    buffer.reserve(buffer.size() + size_size_str + (size_str * sizeof(typename string::value_type)));

    rpc::traits::types::zip(buffer, std::move(size_str));
    for (typename string::value_type& value : str)
    {
        rpc::traits::types::zip(buffer, std::move(value));
    }
}

template<
    class elem,
    class traits = std::char_traits<elem>,
    class alloc = std::allocator<elem>,
    class string = std::basic_string<elem, traits, alloc>
>
inline const rpc::byte* unzip(string& str, const rpc::byte* in, const rpc::byte* in_end)
{
    typename string::size_type size_str(0);
    in = rpc::traits::types::unzip(size_str, in, in_end);
    if (in == nullptr)
        return nullptr;

    str.resize(size_str);

    for (typename string::value_type& value : str)
    {
        in = rpc::traits::types::unzip(value, in, in_end);
        if (in == nullptr)
            return nullptr;
    }

    return in;
}

} // namespace impl

static void zip(rpc::buffer_bytes& buffer, std::string&& str)
{
    rpc::traits::types::impl::zip<char>(buffer, std::move(str));
}
static void zip(rpc::buffer_bytes& buffer, std::wstring&& str)
{
    rpc::traits::types::impl::zip<wchar_t>(buffer, std::move(str));
}

static const rpc::byte* unzip(std::string& str, const rpc::byte* in, const rpc::byte* in_end)
{
    return rpc::traits::types::impl::unzip<char>(str, in, in_end);
}
static const rpc::byte* unzip(std::wstring& str, const rpc::byte* in, const rpc::byte* in_end)
{
    return rpc::traits::types::impl::unzip<wchar_t>(str, in, in_end);
}



//======================================================================================================================
// tuple
//----------------------------------------------------------------------------------------------------------------------







} // namespace types


}} // nmaespace rpc::traits


#endif // RPC_TRAITS_BUILT_IN_TYPES_H






