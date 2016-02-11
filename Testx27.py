#!/usr/bin/python

import serial

ser = serial.Serial('/dev/ttyACM0')
ser.baudrate = 115200

ser.write('1') ##start comm
