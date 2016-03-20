
#ifndef RPC_TYPES_H
#define RPC_TYPES_H


#include <vector>
#include <string>

#include <rpc/defines.h>


namespace rpc
{


using byte = unsigned char;

using buffer_bytes = std::vector<byte>;

enum class result : unsigned short
{
    success = 0,                //!< Функция выполнена успешно.
    very_long_name_resource,    //!< Очень длинное имя ресурса.
    invalid_handle,             //!< Идентификатор pipe не валиден.
    broken,                     //!< Соединение через pipe испорчено.
    closed,                     //!< Соединение через pipe закрыто.

    other,                      //!< Другая ошибка.
};


} // nmaespace rpc


#endif // RPC_TYPES_H
