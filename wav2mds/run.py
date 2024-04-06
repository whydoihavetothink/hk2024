from main import Main
import concurrent.futures
import serial
from sys import argv
from pydub import AudioSegment

file = "id_b.wav" 


def toMDS(file, sampleEnd = 100, threshhold = 0.37, numberOfNotes = 100):
    with concurrent.futures.ThreadPoolExecutor() as executor:
        m = Main(file)

        splitLengthinSeconds = 0.2
        # multithreading
        song = m.split(splitLengthinSeconds, sampleBeginning = 0, sampleEnd = sampleEnd)

        notes = list(executor.map(lambda data: m.thread(data, threshhold, numberOfNotes), song))

        # every list in the list is a triad or more notes played at the same time [[0,12]] for example is an " a' " and an " a'' "
        # notes = m.noteNames(notes)
        # notes = m.removeRepetitions(notes)
        final_notes = []
        winner = 0
        for note_list in notes:
            count_dict = {i: note_list.count(i) for i in note_list}
            if len(count_dict) == 0:
                final_notes.append(0)
                continue
            max_value = max(count_dict.values())
            for k, v in count_dict.items():
                if v == max_value:
                    winner = k
                    break
            final_notes.append(round(440 * 2 ** (winner / 12), 2))

        final_notes_compiled = ""

        counter = 0
        previous = True
        new = False
        for note in final_notes:
            if note == previous or previous == True:
                new = False
                counter += 1
            else:
                final_notes_compiled += f"{ previous } { counter } ";
                counter = 1
                new = True
            previous = note
        if new == True:
            final_notes_compiled += f"{ previous } 1 "
        
        return final_notes_compiled


if __name__ == "__main__":
    args = argv[1:]
    file = AudioSegment.from_mp3(args[0])
    wav = "tmp.wav"
    file.export(wav, fromat="wav")
    board = serial.Serial(port="COM4", baudrate=115200, timeout=.1)
    notes = toMDS(wav, sampleEnd = int(args[1]))

    board.write(bytes(notes, "utf8"))

