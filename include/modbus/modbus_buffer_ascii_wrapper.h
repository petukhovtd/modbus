#pragma once

#include <modbus/modbus_buffer_wrapper.h>

namespace modbus {

/// @brief Обертка ASCII фрейма
class ModbusBufferAsciiWrapper : public IModbusBufferWrapper {
public:
    /// @brief Конструктор класса
    /// @param[in,out] modbusBuffer
    /// @throw std::logic_error если фрейм не ASCII типа
    explicit ModbusBufferAsciiWrapper(ModbusBuffer &modbusBuffer);

    ~ModbusBufferAsciiWrapper() override = default;

    /// @brief Обновляет метки начала и конца фрейма, обновляет lrc
    /// @return
    CheckFrameResult Check() const override;

    /// @brief Проверяет метки начала и конца фрейма, проверяет lrc
    void Update() override;

private:
    ModbusBuffer &modbusBuffer_;
};

}
