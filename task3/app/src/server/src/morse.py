import serial
import sys

MORSE_CODE_DICT = { 'A':'.-', 'B':'-...',
                    'C':'-.-.', 'D':'-..', 'E':'.',
                    'F':'..-.', 'G':'--.', 'H':'....',
                    'I':'..', 'J':'.---', 'K':'-.-',
                    'L':'.-..', 'M':'--', 'N':'-.',
                    'O':'---', 'P':'.--.', 'Q':'--.-',
                    'R':'.-.', 'S':'...', 'T':'-',
                    'U':'..-', 'V':'...-', 'W':'.--',
                    'X':'-..-', 'Y':'-.--', 'Z':'--..',
                    '1':'.----', '2':'..---', '3':'...--',
                    '4':'....-', '5':'.....', '6':'-....',
                    '7':'--...', '8':'---..', '9':'----.',
                    '0':'-----', ', ':'--..--', '.':'.-.-.-',
                    '?':'..--..', '/':'-..-.', '-':'-....-',
                    '(':'-.--.', ')':'-.--.-'}

if __name__ == '__main__':
    board = serial.Serial(port="COM4", baudrate=115200, timeout=.1)
    text = sys.argv[3:]
    freq = int(sys.argv[1])
    dur = int(sys.argv[2])
    data = []
    for word in text:
        for c in word:
            for n in MORSE_CODE_DICT[c.upper()]:
                if n == '.':
                    data.extend([freq, dur, 0, dur//2])
                else:
                    data.extend([freq, dur * 2, 0, dur//2])
            data.extend([0, dur])
    notes_str = ' '.join(map(str,data)) + '\n'
    print(notes_str)
    board.write(bytes(notes_str, "ascii"))
