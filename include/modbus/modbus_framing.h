#pragma once

#include <modbus/modbus_types.h>

#include <cinttypes>
#include <utility>

namespace modbus {

/// @brief Преобразование старшего и младшего байтов в u16
/// @param[in] first старший байт
/// @param[in] second младший байт
/// @return
uint16_t U16FromBuffer(uint8_t first, uint8_t second);

/// @brief Разбирение u16 на байты
/// @param[in] value
/// @return { first, second }
std::pair<uint8_t, uint8_t> U16ToBuffer(uint16_t value);

/// @brief Расчитать CRC последовательности байт
/// @param[in] begin
/// @param[in] end
/// @return
uint16_t CalculateCrc(modbus::AduBuffer::const_iterator begin, modbus::AduBuffer::const_iterator end);

namespace ascii {

/// @brief Преобразование символа 0-9A-F в числоа 0-0xF
/// @param[in] c
/// @return
uint8_t ToU4(unsigned char c);

/// @brief Преобразование числа в символ 0-9A-Z
/// @param[in] u
/// @return
unsigned char FromU4(uint8_t u);

/// @brief Преобразование пары символов 0-9A-Z в u8 число
/// @param[in] first
/// @param[in] second
/// @return
uint8_t ToU8(unsigned char first, unsigned char second);

/// @brief Преобразование числа в пару символов 0-9A-Z
/// @param[in] u
/// @return
std::pair<unsigned char, unsigned char> FromU8(uint8_t u);

/// @brief Конвертирование байт в ascii
/// По используемой схеме размещения pdu в памяти, ascii данные смещены вперед
/// в результате при прямом преобразовании будут затираться не преобразованные бинарные данные
/// поэтому бинарные данные копируются в временный буфер, размера size
/// @param[in] from начало бинарных данных
/// @param[out] to начало записи ascii данных
/// @param[in] size размер бинарных данных
void ToAscii(modbus::AduBuffer::iterator from, modbus::AduBuffer::iterator to, size_t size);

/// @brief Конвертирование ascii в байты
/// Бинарные данные находятся позади ascii в процессе преобразования
/// поэтому преобразование не выделяет дополнительной памяти
/// !! Ожидается что итераторы from и to указывают на один контейнер
/// @param[in] from начало ascii данных
/// @param[out] to начало записи бинарных данных
/// @param[in] size размер ascii данных
/// @throw std::logic_error from < to более чем на 1 байт, что приведет к повреждению данных
void FromAscii(modbus::AduBuffer::iterator from, modbus::AduBuffer::iterator to, size_t size);

/// @brief Расчитать LRC последовательности байт
/// @param[in] begin
/// @param[in] end
/// @return
uint8_t CalculateLrc(modbus::AduBuffer::const_iterator begin, modbus::AduBuffer::const_iterator end);

}

}
