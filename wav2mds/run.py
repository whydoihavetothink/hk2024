from main import Main
import concurrent.futures 

file = "id_b.wav" #  input(r"file: ")
threshhold = 0.37 # float(input(r"threshhold (0.37 works for me, depending on the instrument): "))        #change the threshhold
numberOfNotes = 100 #  int(input(r"maximum number of notes to detect: "))     #change here to change the number of recorded notes


if __name__ == '__main__':
    with concurrent.futures.ThreadPoolExecutor() as executor:
        m = Main(file)

        splitLengthinSeconds = 0.2
        # multithreading
        song = m.split(splitLengthinSeconds, sampleBeginning = 0, sampleEnd = 100)

        notes = list(executor.map(lambda data: m.thread(data, threshhold, numberOfNotes), song))

        # every list in the list is a triad or more notes played at the same time [[0,12]] for example is an " a' " and an " a'' "
        # notes = m.noteNames(notes)
        # notes = m.removeRepetitions(notes)
        print(len(notes))
        print(notes)
