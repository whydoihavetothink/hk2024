import axios from "axios";
import React, { useState } from "react";


function Morse () {
    const [ music, setMusic ] = useState<string>("440");
    const [ nice, setNice ] = useState<string>("A");
    const [ msg, setMsg ] = useState<string>("hello world");
    
    const [speed, setSpeed] = useState<number>(200);

    function note(n: string, p: string) {
        setMusic(n)
        setNice(p)
        }

    function speeder(n: number) {
        setSpeed(n);
        }

    function submit() {
        const _ = axios.post("http://localhost:8080/morse", { frq: music, l: String(speed), msg: msg})
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
        </div>
        <div>
            { nice }
        </div> 
        <div>
        <div>
        Speed
        </div>
        <button onClick={() => speeder(200)}>Fast</button>
        <button onClick={() => speeder(400)}>Medium</button>
        <button onClick={() => speeder(600)}>Slow</button>
        <div>
        { speed }
        </div>
        </div>
        <input value={msg} onChange={e => setMsg(e.target.value)}></input>
        <button onClick={submit}>submit morse</button>
    </>
}

export default Morse;
