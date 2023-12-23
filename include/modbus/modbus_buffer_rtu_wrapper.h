#pragma once

#include <modbus/modbus_buffer_wrapper.h>

namespace modbus {

/// @brief Обертка RTU фрейма
class ModbusBufferRtuWrapper : public IModbusBufferWrapper {
public:
    /// @brief Конструктор класса
    /// @param[in,out] modbusBuffer
    /// @throw std::logic_error если фрейм не RTU типа
    explicit ModbusBufferRtuWrapper(ModbusBuffer &modbusBuffer);

    ~ModbusBufferRtuWrapper() override = default;

    /// @brief Проверяет crc
    /// @return
    CheckFrameResult Check() const override;

    /// @brief Обновляет crc
    void Update() override;

private:
    ModbusBuffer &modbusBuffer_;
};

}
