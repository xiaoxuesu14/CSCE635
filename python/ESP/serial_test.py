import esp
import time
import serial
import struct

SERIAL_RATE_HZ = 0.1
SERIAL_PERIOD = 1.0/SERIAL_RATE_HZ

ser = serial.Serial('/dev/ttyACM0',9600,timeout=0.01)

ser.open()

counter = 0
tnext = time.clock() + SERIAL_PERIOD
ch = ''
while counter < 5:
    if time.clock() >= tnext:
        print(ch)
        ch = ''
        tnext = tnext + 1.0
        counter = counter+1
        msg = esp.esp_pack_gps(3200000000,-960000000,float(counter))
        print("Checksum:",esp.checksum_valid(msg,16))
        '''for k in range(len(msg)):
            print(k,struct.unpack('B',msg[k]))'''
        ser.write(msg)
    # echo anything we receive
    while ser.inWaiting() > 0:
        ch += ser.read()

ser.close()
