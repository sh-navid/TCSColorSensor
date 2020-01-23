'use strict';
const express = require('express');
const app = express();
const SerialPort = require("serialport");

//------------------------------------------------------------------------
app.listen(9001, function () {
    console.log('REST Server is Listening ...\n');
});

// noinspection JSUnresolvedFunction
app.get('/:action', function (req, res) {
    let action = req.params.action || req.param('action');

    if (action === 'w') {
        socket.write("w");
        return res.send('White calibration ...');
    }
    if (action === 'b') {
        socket.write("b");
        return res.send("Black calibration ...");
    }
    if (action === 'n') {
        socket.write("n");
        return res.send("Do sampling ...");
    }

    return res.send('Action: ' + action);
});

app.get('/get/:action', function (req, res) {
    let action = req.params.action || req.param('action');

    if (action === 'w') {
        return res.send({result: whiteJSONData});
    }
    if (action === 'b') {
        return res.send({result: blackJSONData});
    }
    if (action === 'n') {
        return res.send({result: sampleJSONData});
    }
});

//------------------------------------------------------------------------
let socket = new SerialPort("COM4", {
    baudRate: 9600
});

socket.on('open', function () {
    console.log('Serial Server is Listening ...\n');
});

let sampleJSONData = {};
let blackJSONData = {};
let whiteJSONData = {};

//FIXME: need Semaphore
let d = "";
socket.on('data', function (data) {
    data = data.toString();
    d += data;
    if (d.indexOf("$") > -1) {
        d = d.replaceAll('\r\n', '');
        d = d.replaceAll('\r', '');
        d = d.replaceAll('\n', '');
        d = d.replaceAll(' ', '');
        d = d.replaceAll("$", "\n");
        console.log(d);


        let k = d.split('~')[0];
        let j = d.split('~')[1].split('$')[0];
        j=j.replaceAll("\'","\"");
        j=JSON.parse(j);
        if (k === 'SAMPLE') {
            sampleJSONData = j;
        } else if (k === 'BLACK') {
            blackJSONData = j;
        } else if (k === 'WHITE') {
            whiteJSONData = j;
        }


        d = "";

        console.log(j);
    }
});

//------------------------------------------------------------------------
String.prototype.replaceAll = function (search, replacement) {
    let target = this;
    return target.replace(new RegExp(search, 'g'), replacement);
};
