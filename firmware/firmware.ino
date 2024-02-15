#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include <Stepper.h>

const char *ssid = "ESP32-AP";
const char *password = "password";

Servo servo;
const int servoPin = 13; 

Stepper stepper(200, 12, 14, 27, 26); 

const char *html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }

    #slider {
      width: 80%;
      margin: 20px auto;
    }

    #buttons {
      display: flex;
      justify-content: center;
    }

    button {
      margin: 5px;
      padding: 10px;
      font-size: 16px;
    }
  </style>
</head>
<body>

<h2>Servo and Stepper Control</h2>

<label for="slider">Servo Control:</label>
<input type="range" id="slider" min="0" max="180" value="90">

<div id="buttons">
<button onclick="stepperControl(-25)\">-25 Steps</button>  
<button onclick="stepperControl(-5)\">-5 Steps</button>
<button onclick="stepperControl(-1)\">-1 Step</button>
  
  <button onclick="stepperControl(1)\">+1 Step</button>
  <button onclick="stepperControl(5)\">+5 Steps</button>
  <button onclick="stepperControl(25)\">+25 Steps</button>
</div>

<script>
  var slider = document.getElementById("slider");

  slider.addEventListener("input", function() {
    fetch('/servo?angle=' + slider.value);
  });

  function stepperControl(steps) {
    fetch('/stepper?steps=' + steps);
  }
</script>

</body>
</html>
)";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP.toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", html);
  });

  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request) {
    int angle = request->getParam("angle")->value().toInt();
    servo.write(angle);
    request->send(200, "text/plain", "Servo angle set to: " + String(angle));
  });

  server.on("/stepper", HTTP_GET, [](AsyncWebServerRequest *request) {
    int steps = request->getParam("steps")->value().toInt();
    stepper.step(steps);
    request->send(200, "text/plain", "Stepper steps moved: " + String(steps));
  });

  servo.attach(servoPin);
  stepper.setSpeed(200); 

  server.begin();
}

void loop() {
  
}
