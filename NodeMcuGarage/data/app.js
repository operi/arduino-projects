const ws = new WebSocket('ws://192.168.0.200:81/');
ws.onopen = function() {
    console.log('WebSocket Client Connected');
    ws.send("status");
};
ws.onmessage = function(e) {
  console.log("Received: '" + e.data + "'");
};

function flipPump() {
    console.log("Flipping pump");
    ws.send("pump");
}

function flipLight() {
    console.log("Flipping LIGHT");
    ws.send("light");
}