#pragma once

#include <modbus/modbus_buffer.h>

namespace modbus {

/// @brief Результат проверки фрейма
enum class CheckFrameResult {
    NoError = 0,
    TcpInvalidProtocolId,
    TcpInvalidLength,
    RtuInvalidCrc,
    AsciiInvalidStartTag,
    AsciiInvalidLrc,
    AsciiInvalidEndTag,
};

/// @brief Вспомогательная обертка со специфичными для каждого фрейма
class IModbusBufferWrapper {
public:
    virtual ~IModbusBufferWrapper() = default;

    /// @brief Проверка фрейма
    /// @return
    virtual CheckFrameResult Check() const = 0;

    /// @brief Обновление специальный полей фрейма
    virtual void Update() = 0;
};

/// @brief Создать обертку из буфера
/// @param[in,out] modbusBuffer
/// @return
std::shared_ptr<IModbusBufferWrapper> MakeModbusBufferWrapper(ModbusBuffer &modbusBuffer);

}
