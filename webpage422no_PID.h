const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
    <style>
      body {
        margin: 0;
        padding: 0;
        background: linear-gradient(135deg, #1a4025 0%, #0f2b20 100%);
      }
      .container {
        height: 100vh;
        width: 100vw;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        backdrop-filter: blur(15px);
      }
      .row {
        display: flex;
        gap: 0.5rem;
        margin: 0.5rem;
      }
      button {
        width: 100px;
        height: 100px;
        border: none;
        border-radius: 12px;
        font-size: 20px;
        color: white;
        cursor: pointer;
        transition: all 0.2s;
      }
      button:not(.stop) {
        background: linear-gradient(145deg, #2d5a40, #1b4332);
      }
      .stop {
        background: linear-gradient(145deg, #8B4513, #654321);
      }
      button:active {
        transform: scale(0.95);
        filter: brightness(0.9);
      }
      button:hover {
        box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="row">
        <button type="button" onmousedown="forwards()" onmouseup="stop()">⬆<br>Forward</button>
      </div>
      <div class="row">
        <button type="button" onmousedown="left()" onmouseup="stop()">⬅<br>Left</button>
        <button class="stop" type="button" onmousedown="stop()" onmouseup="stop()">■<br>Stop</button>
        <button type="button" onmousedown="right()" onmouseup="stop()">➡<br>Right</button>
      </div>
      <div class="row">
        <button type="button" onmousedown="backwards()" onmouseup="stop()">⬇<br>Back</button>
      </div>
    </div>

    <script>
      function move(string) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", string, true);
        xhttp.send();
      }
      function forwards() {
        move("forwards");
      }
      function stop() {
        move("stop");
      }
      function left() {
        move("left");
      }
      function backwards() {
        move("backwards");
      }
      function right() {
        move("right");
      }
        
      document.addEventListener("keypress", function (event) {
        if (event.key == "w") {
          forwards();
        } else if (event.key == "s") {
          backwards();
        } else if (event.key == "a") {
          left();
        } else if (event.key == "d") {
          right();
        } else if (event.key == " ") {
          stop();
        }
      });
      document.addEventListener('keyup', stop);
    </script>
  </body>
</html>
)===";