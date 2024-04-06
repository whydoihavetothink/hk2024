import { JSDOM } from "jsdom";
import { readFileSync } from "fs"; 
import path from "path";


interface IarduinoSong {
    tempo: number,
    length: number,
    note: string[],
    duration: number[],
}

function translate(filename: string): IarduinoSong {
    const out: IarduinoSong = { note: new Array<string>, duration: new Array<number> } as IarduinoSong;
    const file: string = readFileSync(path.resolve(__dirname, filename), "utf8");
    const doc = new JSDOM(file).window.document;

    const tempo = doc.querySelector("tempo");
    if ( tempo ) {
        out.tempo = +tempo;
    } else {
        out.tempo = 120;
    }
    const divisions: number = +(doc.querySelector("divisions")?.textContent || "0");
    const quarter: number = 60 / out.tempo / divisions * 1000;

    doc.querySelectorAll("measure").forEach((measure) => {
        measure.querySelectorAll("note").forEach((note) => {
            let note_name = note.querySelector("step")?.textContent || "";
            note_name += note.querySelector("alter")?.textContent === "1" ? "S" : "";
            note_name += note.querySelector("octave")?.textContent || "";
            note_name = note_name.length < 1 ? "0" : note_name.toUpperCase();
            out.note.push(note_name);

            let duration = +(note.querySelector("duration")?.textContent || "0" );
            if ( duration > 0 ) {
                duration = duration * quarter;
            } else {
                const type = note.querySelector("type")?.textContent;
                switch (type) {
                    case "half":
                        duration = quarter * 2;
                        break;
                    case "quarter":
                        duration = quarter;
                        break;
                    case "eighth":
                        duration = quarter / 2;
                        break;
                    default:
                    case "whole":
                        duration = quarter * 4;
                        break;
                }
            }
            out.duration.push(duration);
        })
    })
    out.length = out.note.length;

    return out
}


console.log(translate("./music.xml"))


