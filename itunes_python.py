import serial
import appscript
import os

from pykakasi import kakasi
kakasi = kakasi()

itunes = appscript.app('iTunes')

from time import sleep

def main():

    kakasi.setMode('H', 'a')
    kakasi.setMode('K', 'a')
    kakasi.setMode('J', 'a')
    conv = kakasi.getConverter()
    
    with serial.Serial(port="/dev/tty.usbmodem141141", baudrate=9600, timeout=1) as device:

        while True:
            title = itunes.current_track.name.get()
            artist = itunes.current_track.artist.get()
            time = int(itunes.player_position.get())

            minute = 0;

            if time >= 60:
                while time >= 60:
                    time = time - 60
                    minute = minute + 1

                if minute < 10:
                    time_str = str(0)
                elif minute == 0:
                    time_str = str(0) + str(0)

                time_str = time_str + str(minute)
                if time < 10:
                    time_str = time_str + str(0) + str(time)
                else:
                    time_str = time_str + str(time)
            else:
                time_str = str(0) + str(0)
                if time < 10:
                    time_str = time_str + str(0) + str(time)
                else:
                    time_str = time_str + str(time)

            sleep(0.5)
            device.write(conv.do(title).encode(errors="ignore") + '\n')
            device.write(conv.do(artist).encode(errors="ignore") + '\n')
            device.write(time_str + '\n')

            os.system('clear')
            print("TITLE = " + title)
            print("ARTIST = " + artist)
            print("TIME = " + time_str)

        device.close()

if __name__ == '__main__':
    main()
