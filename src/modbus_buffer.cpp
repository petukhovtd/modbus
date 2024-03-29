#include <modbus/modbus_buffer.h>
#include <modbus/modbus_framing.h>

#include <sstream>

namespace modbus {

namespace {

/// @brief Общая функция получения значения в буфере
/// @param[in] frameType тип фрейма
/// @param[in] field тип поля
/// @param[in] begin начало буфера
/// @return
uint8_t CommonGetter(FrameType frameType, CommonField field, AduBuffer::const_iterator begin) {
    auto it = begin + GetPosForType(frameType, field);
    switch (frameType) {
        case RTU:
        case TCP:
            return *it;
        case ASCII:
            return modbus::ascii::ToU8(*(it), *(it + 1));
        default:
            return 0;
    }
}

/// @brief Общая функция устанлвки значения в буфере
/// @param[in] frameType тип фрейма
/// @param[in] field тип поля
/// @param[in] begin начало буфера
/// @param[in] value значение
/// @return
void CommonSetter(FrameType frameType, CommonField field, AduBuffer::iterator begin, uint8_t value) {
    auto it = begin + GetPosForType(frameType, field);
    switch (frameType) {
        case RTU:
        case TCP:
            *it = value;
            break;
        case ASCII: {
            auto res = modbus::ascii::FromU8(value);
            *it = res.first;
            ++it;
            *it = res.second;
        }
            break;
        default:
            break;
    }
}

}

ModbusBuffer::ModbusBuffer(FrameType type)
        : type_(type), buffer_(MakeAduBuffer()), aduSize_(GetAduMaxSize(type)) {}

ModbusBuffer::ModbusBuffer(ModbusBuffer &&other) noexcept
        : type_(other.type_), buffer_(std::move(other.buffer_)), aduSize_(other.aduSize_) {}

FrameType ModbusBuffer::GetType() const {
    return type_;
}

AduBuffer::iterator ModbusBuffer::begin() {
    return buffer_.begin() + GetAduStart(type_);
}

AduBuffer::const_iterator ModbusBuffer::begin() const {
    return buffer_.begin() + GetAduStart(type_);
}

AduBuffer::iterator ModbusBuffer::end() {
    return ModbusBuffer::begin() + aduSize_;
}

AduBuffer::const_iterator ModbusBuffer::end() const {
    return ModbusBuffer::begin() + aduSize_;
}

AduBuffer::const_iterator ModbusBuffer::cbegin() const {
    return buffer_.begin() + GetAduStart(type_);
}

AduBuffer::const_iterator ModbusBuffer::cend() const {
    return ModbusBuffer::begin() + aduSize_;
}

bool ModbusBuffer::SetAduSize(size_t aduSize) {
    if (aduSize < GetAduMinSize(type_) || GetAduMaxSize(type_) < aduSize) {
        return false;
    }
    aduSize_ = aduSize;
    return true;
}

size_t ModbusBuffer::GetAduSize() const {
    return aduSize_;
}

uint8_t ModbusBuffer::GetUnitId() const {
    return CommonGetter(type_, CommonField::UnitId, buffer_.begin());
}

void ModbusBuffer::SetUnitId(uint8_t id) {
    return CommonSetter(type_, CommonField::UnitId, buffer_.begin(), id);
}

uint8_t ModbusBuffer::GetFunctionCode() const {
    return CommonGetter(type_, CommonField::FunctionCode, buffer_.begin());
}

void ModbusBuffer::SetFunctionCode(uint8_t fc) {
    return CommonSetter(type_, CommonField::FunctionCode, buffer_.begin(), fc);
}

void ModbusBuffer::ConvertTo(FrameType toType) {
    if (type_ == toType) {
        return;
    }

    size_t pduSize = CalculatePduSize(type_, aduSize_);

    if (pduSize >= aduSize_) {
        throw std::logic_error("pdu size more then adu size");
    }

    // TCP -> ASCII
    // RTU -> ASCII
    if (toType == FrameType::ASCII) {
        ascii::ToAscii(buffer_.begin() + unitIdGeneralPos,
                       buffer_.begin() + unitIdGeneralPos,
                       unitIdSize + pduSize);
        pduSize *= 2;
    }

    // ASCII -> RTU
    // ASCII -> TCP
    if (type_ == FrameType::ASCII) {
        ascii::FromAscii(buffer_.begin() + unitIdGeneralPos,
                         buffer_.begin() + unitIdGeneralPos,
                         unitIdSize * 2 + pduSize);
        pduSize /= 2;
    }

    type_ = toType;
    aduSize_ = CalculateAduSize(type_, pduSize);
}

}
