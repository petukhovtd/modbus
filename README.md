# modbus
Modbus buffer implementation for convert from other frame type

Main idea is use one memory for other modbus frame type.

```
                                   | ----------- PDU ---------|
|  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |      |
| tr id   | pr id   | len     | id | fc | data                |
| ----------------------- TCP ADU ----------------------------|
                              | id | fc | data                | CRC (2) |
                              | -------------- RTU ADU -----------------|
                         |star| id      | fc      | data      | LRC (2) | end (2) |
                         | ---------------- ASCII ADU ----------------------------|
                                        | ----- ASCII PDU ----|          
```

## Example
```
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
```
