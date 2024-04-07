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
    sh(`python upload.py ${notes}`)
    res.status(200).json({body: "valid"})
}



let routes = (app) => {
  router.post("/upload", controller.upload);
  router.get("/files", controller.getListFiles);
  router.get("/files/:name", controller.download);
  router.delete("/files/:name", controller.remove);
  router.post("/notes", play);

  app.use(express.json());
  app.use(router);
};

module.exports = routes;
