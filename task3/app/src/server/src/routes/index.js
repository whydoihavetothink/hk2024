const express = require("express");
const router = express.Router();
const controller = require("../controller/file.controller");

const child_process = require("child_process");
async function sh(cmd) {
  return new Promise(function (resolve, reject) {
    child_process.exec(cmd, (err, stdout, stderr) => {
      if (err) {
        reject(err);
      } else {
        resolve({ stdout, stderr });
      }
    });
  });
}

function play (req, res) {
    const notes = req.body.notes;
    sh(`python src\\server\\src\\upload.py ${notes}`)
    res.status(200).json({body: "valid"})
}

function morse(req, res) {
    const body = req.body;

    const frq = body.frq;
    const l = body.l;
    const msg = body.msg;

    sh(`python src\\server\\src\\morse.py ${frq} ${l} ${msg}`)
    res.status(200).json({body: "valid"})

}



let routes = (app) => {
  router.post("/upload", controller.upload);
  router.get("/files", controller.getListFiles);
  router.get("/files/:name", controller.download);
  router.delete("/files/:name", controller.remove);
  router.post("/notes", play);
  router.post("/morse", morse);

  app.use(express.json());
  app.use(router);
};

module.exports = routes;
