#include <modbus/modbus_buffer.h>
#include <modbus/modbus_buffer_rtu_wrapper.h>
#include <modbus/modbus_buffer_tcp_wrapper.h>

int main()
{
     // RTU to TCP
     {
          static const modbus::AduBuffer rtuFrame = { 0x1, 0x6, 0xDF, 0x62, 0x38 };

          //  Create max buffer for rtu frame type
          // |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | ... |256 |
          //                                 ^- begin                       ^- end
          modbus::ModbusBuffer modbusBuffer( modbus::FrameType::RTU );

          //  Fill modbus message, set adu size
          // |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |
          //                               | ------- ADU -----------|
          //                                    |-- PDU --|
          //                               |0x01|0x06|0xDF|0x62|0x38|
          //                                 ^- begin            ^- end
          std::copy( rtuFrame.begin(), rtuFrame.end(), modbusBuffer.begin() );
          modbusBuffer.SetAduSize( rtuFrame.size() );

          // Check crc
          modbus::ModbusBufferRtuWrapper rtuWrapper( modbusBuffer );
          auto checkResult = rtuWrapper.Check();
          if( checkResult != modbus::CheckFrameResult::NoError )
          {
               return -1;
          }

          //  Transform to tcp frame
          // |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |
          // | ----------------- ADU ---------------------|
          //                                    |-- PDU --|
          // |    |    |    |    |    |    |0x01|0x06|0xDF|0x62|0x38|
          //   ^- begin                                ^- end
          modbusBuffer.ConvertTo( modbus::FrameType::TCP );
          modbus::ModbusBufferTcpWrapper tcpWrapper( modbusBuffer );

          // Update tcp header
          // |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |
          // |         |  0x00   |  0x03   |0x01|0x06|0xDF|
          tcpWrapper.Update();

          // Update transaction id
          // |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |
          // |  0x01   |  0x00   |  0x03   |0x01|0x06|0xDF|
          tcpWrapper.SetTransactionId( 1 );
     }
     return 0;
}
