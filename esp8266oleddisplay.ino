#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <U8g2lib.h>

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// WiFi settings
const char* ssid = "ESP32_Snake_Game";
const char* password = "snakegame123";

WebServer server(80);

// Game settings
#define GRID_WIDTH 16
#define GRID_HEIGHT 8
#define PIXEL_SIZE 4

// Snake game variables
struct Point {
  int x, y;
};

Point snake[GRID_WIDTH * GRID_HEIGHT];
int snakeLength = 3;
Point food;
int direction = 0; // 0=right, 1=down, 2=left, 3=up
bool gameOver = false;
int score = 0;
unsigned long lastMove = 0;
int gameSpeed = 300; // milliseconds

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 15, "Starting Snake Game...");
  display.sendBuffer();
  delay(2000);
  
  // Setup WiFi hotspot
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Initialize game
  initGame();
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/reset", handleReset);
  server.begin();
  
  Serial.println("HTTP server started");
  Serial.println("Connect to WiFi: ESP32_Snake_Game");
  Serial.println("Password: snakegame123");
  Serial.println("Then go to: 192.168.4.1");
}

void loop() {
  server.handleClient();
  
  if (!gameOver && millis() - lastMove > gameSpeed) {
    moveSnake();
    lastMove = millis();
  }
  
  drawGame();
  delay(50);
}

void initGame() {
  // Initialize snake in center
  snake[0] = {GRID_WIDTH/2, GRID_HEIGHT/2};
  snake[1] = {GRID_WIDTH/2-1, GRID_HEIGHT/2};
  snake[2] = {GRID_WIDTH/2-2, GRID_HEIGHT/2};
  snakeLength = 3;
  direction = 0;
  gameOver = false;
  score = 0;
  generateFood();
}

void generateFood() {
  bool validPosition = false;
  while (!validPosition) {
    food.x = random(0, GRID_WIDTH);
    food.y = random(0, GRID_HEIGHT);
    validPosition = true;
    
    // Check if food spawns on snake
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        validPosition = false;
        break;
      }
    }
  }
}

void moveSnake() {
  if (gameOver) return;
  
  Point newHead = snake[0];
  
  // Move head based on direction
  switch (direction) {
    case 0: newHead.x++; break; // right
    case 1: newHead.y++; break; // down
    case 2: newHead.x--; break; // left
    case 3: newHead.y--; break; // up
  }
  
  // Check wall collision
  if (newHead.x < 0 || newHead.x >= GRID_WIDTH || 
      newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
    gameOver = true;
    return;
  }
  
  // Check self collision
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
      gameOver = true;
      return;
    }
  }
  
  // Move snake body
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i-1];
  }
  snake[0] = newHead;
  
  // Check food collision
  if (newHead.x == food.x && newHead.y == food.y) {
    snakeLength++;
    score += 10;
    generateFood();
    
    // Increase speed slightly
    if (gameSpeed > 100) {
      gameSpeed -= 5;
    }
  }
}

void drawGame() {
  display.clearBuffer();
  
  if (gameOver) {
    display.setFont(u8g2_font_10x20_tf);
    display.drawStr(15, 25, "GAME");
    display.drawStr(25, 45, "OVER");
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(0, 60, ("Score: " + String(score)).c_str());
  } else {
    // Draw snake
    for (int i = 0; i < snakeLength; i++) {
      int x = snake[i].x * PIXEL_SIZE * 2;
      int y = snake[i].y * PIXEL_SIZE * 2;
      if (i == 0) {
        // Draw head as filled box
        display.drawBox(x, y, PIXEL_SIZE * 2, PIXEL_SIZE * 2);
      } else {
        // Draw body as outline
        display.drawFrame(x, y, PIXEL_SIZE * 2, PIXEL_SIZE * 2);
      }
    }
    
    // Draw food as filled circle
    int fx = food.x * PIXEL_SIZE * 2 + PIXEL_SIZE;
    int fy = food.y * PIXEL_SIZE * 2 + PIXEL_SIZE;
    display.drawDisc(fx, fy, PIXEL_SIZE, U8G2_DRAW_ALL);
    
    // Draw score
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(0, 62, ("Score: " + String(score)).c_str());
  }
  
  display.sendBuffer();
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Snake Game</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { 
            font-family: Arial; 
            text-align: center; 
            background: #2c3e50;
            color: white;
            margin: 0;
            padding: 20px;
        }
        .container {
            max-width: 400px;
            margin: 0 auto;
        }
        h1 { 
            color: #3498db; 
            margin-bottom: 30px;
        }
        .controls {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            grid-template-rows: 1fr 1fr 1fr;
            gap: 10px;
            max-width: 200px;
            margin: 20px auto;
        }
        button {
            background: #3498db;
            color: white;
            border: none;
            padding: 20px;
            font-size: 20px;
            border-radius: 10px;
            cursor: pointer;
            transition: background 0.3s;
        }
        button:hover {
            background: #2980b9;
        }
        button:active {
            background: #1f6390;
        }
        .up { grid-column: 2; grid-row: 1; }
        .left { grid-column: 1; grid-row: 2; }
        .right { grid-column: 3; grid-row: 2; }
        .down { grid-column: 2; grid-row: 3; }
        .reset {
            background: #e74c3c;
            margin-top: 20px;
            padding: 15px 30px;
            font-size: 16px;
        }
        .reset:hover {
            background: #c0392b;
        }
        .info {
            margin-top: 30px;
            padding: 15px;
            background: #34495e;
            border-radius: 10px;
            font-size: 14px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🐍 Snake Game Controller</h1>
        <p>Use the buttons below to control the snake on the OLED display!</p>
        
        <div class="controls">
            <button class="up" onclick="move('up')">↑</button>
            <button class="left" onclick="move('left')">←</button>
            <button class="right" onclick="move('right')">→</button>
            <button class="down" onclick="move('down')">↓</button>
        </div>
        
        <button class="reset" onclick="resetGame()">🔄 Reset Game</button>
        
        <div class="info">
            <p><strong>How to Play:</strong></p>
            <p>• Use arrow buttons to change snake direction<br>
            • Eat the food circles to grow and score points<br>
            • Don't hit walls or yourself!<br>
            • Game speeds up as you score more points</p>
        </div>
    </div>

    <script>
        function move(dir) {
            fetch('/move?dir=' + dir)
                .then(response => response.text())
                .then(data => console.log(data))
                .catch(error => console.error('Error:', error));
        }
        
        function resetGame() {
            fetch('/reset')
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                    alert('Game Reset!');
                })
                .catch(error => console.error('Error:', error));
        }
        
        // Keyboard controls
        document.addEventListener('keydown', function(event) {
            switch(event.key) {
                case 'ArrowUp':
                    event.preventDefault();
                    move('up');
                    break;
                case 'ArrowDown':
                    event.preventDefault();
                    move('down');
                    break;
                case 'ArrowLeft':
                    event.preventDefault();
                    move('left');
                    break;
                case 'ArrowRight':
                    event.preventDefault();
                    move('right');
                    break;
                case ' ':
                    event.preventDefault();
                    resetGame();
                    break;
            }
        });
    </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

void handleMove() {
  String dir = server.arg("dir");
  int newDirection = direction;
  
  if (dir == "up" && direction != 1) newDirection = 3;
  else if (dir == "down" && direction != 3) newDirection = 1;
  else if (dir == "left" && direction != 0) newDirection = 2;
  else if (dir == "right" && direction != 2) newDirection = 0;
  
  direction = newDirection;
  server.send(200, "text/plain", "Direction changed to: " + dir);
}

void handleReset() {
  initGame();
  server.send(200, "text/plain", "Game reset!");
}