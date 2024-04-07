
import serial
from sys import argv

if __name__ == "__main__":
    args = argv[1:]
    notes = " ".join(args) + "\n"
    board = serial.Serial(port="COM4", baudrate=115200, timeout=.1)
    print(notes)

    board.write(bytes(notes, "ascii"))

