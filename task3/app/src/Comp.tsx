import axios from "axios";
import React, { useState } from "react";


function Comp () {
    const [ music, setMusic ] = useState<string>("");
    const [ nice, setNice ] = useState<string>("");
    const [ msg, setMsg ] = useState<number>(1000);

    function note(n: string, p: string) {
        setMusic(music + n + ` ${msg} `)
        setNice(nice + p + ` (${msg}) `)
        }

    function submit() {
        const _ = axios.post("http://localhost:8080/notes", { notes: music })
            .then((_) => {
                return "All Good";
            })
            .catch((_) => {
                 return "something f*"
             }).finally(() => {
                setMusic("");
                setNice("");
            });
    }

    return <>
        <div>
        <button onClick={() => note("440", "A")}>A</button>
        <button onClick={() => note("466", "A#")}>A#</button>
        <button onClick={() => note("494", "B")}>B</button>
        <button onClick={() => note("523", "C")}>C</button>
        <button onClick={() => note("554", "C#")}>C#</button>
        <button onClick={() => note("587", "D")}>D</button>
        <button onClick={() => note("622", "D#")}>D#</button>
        <button onClick={() => note("659", "E")}>E</button>
        <button onClick={() => note("698", "F")}>F</button>
        <button onClick={() => note("740", "F#")}>F#</button>
        <button onClick={() => note("784", "G")}>G</button>
        <button onClick={() => note("831", "G#")}>G#</button>
        <button onClick={() => note("0", "-")}>Pause</button>
        </div> 
        <input value={msg} onChange={e => setMsg(+e.target.value)} type="number"></input>
        ms
        <div>
            { nice }
        </div> 
        <button onClick={submit}>submit</button>
        <button onClick={() => { setMusic(""); setNice("") }}>clear</button>
    </>
}

export default Comp;
