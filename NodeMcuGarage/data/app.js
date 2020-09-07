const ws = new WebSocket('ws://192.168.0.200:81/');
ws.onopen = function() {
    console.log('WebSocket Client Connected');
};
ws.onmessage = function(e) {
  console.log("Received: '" + e.data + "'");
  var response = JSON.parse(e.data);
  document.getElementById("pump").checked = response.pump.state;
  document.getElementById("light").checked = response.light.state;
};

function flipPump() {
    console.log("Flipping pump");
    ws.send("pump");
}

function flipLight() {
    console.log("Flipping LIGHT");
    ws.send("light");
}