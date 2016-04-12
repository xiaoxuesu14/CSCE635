import serial
import time
import struct

class parserObject:
    def __init__(self):
        self.parsing = False
        self.msgBuffer = bytes()
        self.len = -1
        self.pos = -1
    def handle(self,ch):
        self.msgBuffer+=struct.pack('B',ord(ch))
        if not self.parsing:
            if len(self.msgBuffer) > 1:
                byteLast = struct.unpack("B",self.msgBuffer[-2])[0]
                byteNow = struct.unpack("B",self.msgBuffer[-1])[0]
                if byteLast == 160 and byteNow == 161:
                    print("Parsed message")
                    print(byteNow,byteLast)
                    #for k in range(len(self.msgBuffer)):
                    #    print( struct.unpack("B",self.msgBuffer[k])[0] )
                self.parsing = True
                self.pos = -1
        else:
            self.pos = self.pos+1
            '''
            if self.pos == 1:
                # read low and high bytes for length
                #bytesR = struct.unpack("BBBB",self.msgBuffer[-4:0])[0]
                for k in range(len(self.msgBuffer)):
                    print( struct.unpack("B",self.msgBuffer[k])[0] )
                bytesR = 0
                lenH = struct.unpack("B",self.msgBuffer[-2])[0]
                lenL = struct.unpack("B",self.msgBuffer[-1])[0]
                self.len = (lenH >> 4) + lenL
                print(bytesR,self.len,self.msgBuffer[-2],self.msgBuffer[-1],lenH,lenL)
                '''

def packTest():
    buf = bytes()
    buf+=struct.pack('%sB' % 9,160,161,0,2,2,0,2,13,10)
    return buf

PORT = '/dev/ttyUSB0'
BAUDRATE = 9600
TRYINTERVAL = 1.0

ser = serial.Serial(PORT,BAUDRATE,timeout=0.01)

print("Opening port" + PORT)

ser.open()

time.sleep(1.0)

print("writing to port")

trying = True
tryCounter = 0
tryTime = time.time() + TRYINTERVAL

po = parserObject()
stri = ''
while True:
    try:
        # read from serial
        # echo anything we receive
        printed = False
        while ser.inWaiting() > 0:
            ch = ser.read()
            stri += ch
            po.handle( ch )
            printed = True
        if printed:
            #print(stri.encode('hex'))
            stri = ''
        if trying:
            if time.time() >= tryTime:
                tryTime += TRYINTERVAL
                tryCounter = tryCounter + 1
                if tryCounter >= 10:
                    trying = False
                buf = packTest()
                for k in range(len(buf)):
                    print(buf[k].encode('hex'))
                    ser.write(buf[k])
                    time.sleep(0.05)
    except KeyboardInterrupt:
        ser.close();
        print("Exiting")
        break;
