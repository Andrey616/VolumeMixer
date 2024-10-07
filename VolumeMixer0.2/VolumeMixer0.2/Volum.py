# -*- coding: utf-8 -*-
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume, IAudioEndpointVolume
from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from comtypes import CLSCTX_ALL
import sys

keyEnc5 = 0
keyEnc6 = 0
keyEnc7 = 0
media = [" ", "", "Telegram.exe", "firefox.exe", "Discord.exe", "", "", ""]

def main(data):
    data = data.strip().split('|')
    data = data[1:-1:]
    global keyEnc5, keyEnc6, keyEnc7
    sessions = AudioUtilities.GetAllSessions()
    for session in sessions:
        volume = session._ctl.QueryInterface(ISimpleAudioVolume)

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

    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume_master = interface.QueryInterface(IAudioEndpointVolume)
    volume_master.SetMasterVolumeLevelScalar(float(data[0])/100, None)


if __name__ == "__main__":
    for line in sys.stdin:
        if line.strip() == "":
            continue
        try:
            main(line)
            print("OK")  # ���������� �������������
        except Exception as e:
            print(f"ERROR: {str(e)}")