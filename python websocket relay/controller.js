'use strict'
var ws;

var pan = 0;
var tilt = 0;
var drive = 0;
var turn = 0;

function init() {

  // Connect to Web Socket
  ws = new WebSocket("ws://localhost:9001/");

  // Set event handlers.
  ws.onopen = function() {
    output("onopen");
  };

  ws.onmessage = function(e) {
    // e.data contains received string.
    output("onmessage: " + e.data);
  };

  ws.onclose = function() {
    output("onclose");
  };

  ws.onerror = function(e) {
    output("onerror");
    console.log(e)
  };

}

function onSubmit() {
  var input = document.getElementById("input");
  // You can send message to the Web Socket using ws.send.
  ws.send(input.value);
  output("send: " + input.value);
  input.value = "";
  input.focus();
}

function onCloseClick() {
  ws.close();
}

function output(str) {
  var log = document.getElementById("log");
  var escaped = str.replace(/&/, "&amp;").replace(/</, "&lt;").
    replace(/>/, "&gt;").replace(/"/, "&quot;"); // "
  //log.innerHTML = escaped + "<br>" + log.innerHTML;
  log.innerHTML = escaped;
}

function sendMessage(forward, turn, pan, tilt) {
  var message = [forward, turn, pan, tilt].join(',')

  ws.send(message);
  output("sent: " + message);
}

function sendDeltaMove(dpan, dtilt) {
  pan += dpan;
  tilt += dtilt;
}

function sendCommand() {
  sendMessage(drive, turn, pan, tilt);
}

// window.setInterval(sendCommand, .1);

document.onkeyup = function() {
  drive = turn = 0;
  sendCommand();
}

document.onkeydown = function(evt) {
    evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    var charStr = String.fromCharCode(charCode);

  console.log('pressed', charStr)
    if (charStr == 'W') {
      drive = drivePower;
    } else if (charStr == 'S') {
      drive = -drivePower;
    } else if (charStr == 'A') {
      turn = -drivePower;
    } else if (charStr == 'D') {
      turn = drivePower;
    } else if (charStr == 'I') {
      tilt += increment;
    } else if (charStr == 'K') {
      tilt -= increment;
    } else if (charStr == 'J') {
      pan -= increment;
    } else if (charStr == 'L') {
      pan += increment;
    }

    sendCommand();
};


var increment = 10;

var drivePower = 1.0;
