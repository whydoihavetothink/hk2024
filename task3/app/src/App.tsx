import React, { useState } from "react";
import axios from "axios";


function App () {
	const [file, setFile] = useState<File | null>(null);

	function handleFileChange (e: React.ChangeEvent<HTMLInputElement>) {
		if (e.target.files) {
			setFile(e.target.files[0]);
		}
	};

	async function handleUpload () {
		if (file) {
            console.log("Uploading file...");

            const formData = new FormData();
            formData.append("file", file);

            axios.post("/api", formData);

/*
        const data = await fetch("https://localhost:3000/api/local", {
                method: "POST",
                body: formData,
            }).then((result) => {
                return result.json()
            }).catch((_) => {
                console.log("Err")
                return ""
            });
        console.log(data)
*/
        };
	};

	return (
		<>
			<div>
				<label htmlFor="file" className="sr-only">
					Choose a file
				</label>
				<input id="file" type="file" onChange={handleFileChange} />
			</div>
			{file && (
				<section>
					File details:
					<ul>
						<li>Name: {file.name}</li>
						<li>Type: {file.type}</li>
						<li>Size: {file.size} bytes</li>
					</ul>
				</section>
			)}

			{file && <button onClick={handleUpload}>Upload a file</button>}
		</>
	);
};

export default App;
