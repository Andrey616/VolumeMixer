from __future__ import print_function
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume
from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
import serial
import time
import serial.tools.list_ports
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
from comtypes import CLSCTX_ALL
import os

keyEnc5 = 0
keyEnc6 = 0
keyEnc7 = 0

def main(data):
    global keyEnc5, keyEnc6, keyEnc7
    sessions = AudioUtilities.GetAllSessions()
    for session in sessions:
        #print(session.Process and session.Process.name()) #выводит всё что использует звук
        volume = session._ctl.QueryInterface(ISimpleAudioVolume)

        """if session.Process and session.Process.name() == media[0]:
            volume.SetMasterVolume(float(data[0])/100, None)"""
        
        if session.Process and session.Process.name() == media[1]:
            volume.SetMasterVolume(float(data[1])/100, None)

        if session.Process and session.Process.name() == media[2]:
            volume.SetMasterVolume(float(data[2])/100, None)

        if session.Process and session.Process.name() == media[3]:
            volume.SetMasterVolume(float(data[3])/100, None)

        if session.Process and session.Process.name() == media[4]:
            volume.SetMasterVolume(float(data[4])/100, None)

        if session.Process and session.Process.name() == media[5]:
            volume.SetMasterVolume(float(data[5])/100, None)
            if data[8] == "111" and keyEnc5 == 0:
                volume.SetMute(1, None)
                keyEnc5 = 1
            elif data[8] == "111" and keyEnc5 == 1:
                volume.SetMute(0, None)
                keyEnc5 = 0

        if session.Process and session.Process.name() == media[6]:
            volume.SetMasterVolume(float(data[6])/100, None)
            if data[8] == "222" and keyEnc6 == 0:
                volume.SetMute(1, None)
                keyEnc6 = 1
            elif data[8] == "222" and keyEnc6 == 1:
                volume.SetMute(0, None)
                keyEnc6 = 0

        if session.Process and session.Process.name() == media[7]:
            volume.SetMasterVolume(float(data[7])/100, None)
            if data[8] == "333" and keyEnc7 == 0:
                volume.SetMute(1, None)
                keyEnc7 = 1
            elif data[8] == "333" and keyEnc7 == 1:
                volume.SetMute(0, None)
                keyEnc7 = 0
        
        #общая громкость
        devices = AudioUtilities.GetSpeakers()
        interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
        volume = interface.QueryInterface(IAudioEndpointVolume)
        volume.SetMasterVolumeLevelScalar(float(data[0])/100, None)
            

"""MixerValue = [0,0,0]     
MixerValue1 = '' 

def encod():  # функция считывает громкость приложений для энкодеров но не работает отправка на ардуино 
    global MixerValue1
    global MixerValue

    ser = serial.Serial(
    port='COM6',  # Замените на нужный порт
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )

    sessions = AudioUtilities.GetAllSessions()
    for session in sessions:
        if session.Process and session.Process.name() == media[5]:
            volume = session._ctl.QueryInterface(ISimpleAudioVolume)
            MixerValue[0] = int(round(volume.GetMasterVolume(), 2) * 100)

        if session.Process and session.Process.name() == media[6]:
            volume = session._ctl.QueryInterface(ISimpleAudioVolume)
            MixerValue[1] = int(round(volume.GetMasterVolume(), 2) * 100)

        if session.Process and session.Process.name() == media[7]:
            volume = session._ctl.QueryInterface(ISimpleAudioVolume)
            MixerValue[2] = int(round(volume.GetMasterVolume(), 2) * 100)

    MixerValue = [str(i) for i in MixerValue]
    MixerValue1 = '|'.join(MixerValue)
    MixerValue1 = b'MixerValue1'

    ser.write(MixerValue1)
    
    ser.close()

def encod1():
    ser = serial.Serial(
    port='COM6',  # Замените на нужный порт
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )
    ser.write('100100100'.encode())
    
    ser.close()"""

"""
Сделать:

"""

media = ["не используется", "", "Telegram.exe", "firefox.exe", "Discord.exe", "", "", ""]
port = ""
baudrate = 9600

for i in list(serial.tools.list_ports.comports()):
    if 'USB-SERIAL' in i.description:
        port = i.device

while True:
    try:
        ser = serial.Serial(port, baudrate)
        #print(f"Успешно подключено к порту: {port}",end="")
        data = ser.readline()
        data = str(data, 'CP866')
        data = data.split('|')
        data = data[1:-1:]
        if len(data) == 9:
            os.system('cls||clear')
            print(f"\033[92mУспешно подключено к порту: {port}\nПринятые данные: {data}", end="\r")
            main(data)
        ser.close() 
        
    except (OSError, serial.SerialException):
        os.system('cls||clear')
        print("\033[91mОшибка подключения к порту. Попытка переподулючения..", end="\r")
        for i in list(serial.tools.list_ports.comports()):
            if 'USB-SERIAL' in i.description:
                port = i.device
        continue
    