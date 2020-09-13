const ws = new WebSocket('ws://192.168.0.200:81/');
ws.onopen = function() {
    console.log('WebSocket Client Connected');
};
ws.onerror = function(event) {
    window.alert("Error connecting WebSocket client");
    console.log(event);
}
ws.onmessage = function(e) {
  console.log("Received: '" + e.data + "'");
  var response = JSON.parse(e.data);
  document.getElementById("pump").checked = response.pump.state;
  document.getElementById("light").checked = response.light.state;
  if (response.pump.state) {
    var stopAtDate = new Date(response.pump.stopPumpAt * 1000);
    var message = "Turns off at " + stopAtDate.toLocaleTimeString('en-US');
    document.getElementById("pumpOffAtText").style.display = "initial";
    document.getElementById("pumpOffAtMessage").textContent = message;
   } else {
        document.getElementById("pumpOffAtText").style.display = "none";

   }
};

function flipPump() {
    console.log("Flipping pump");
    ws.send("pump");
};

function flipLight() {
    console.log("Flipping LIGHT");
    ws.send("light");
};