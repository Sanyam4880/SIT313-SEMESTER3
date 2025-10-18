#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 🛜 Wi-Fi Credentials
const char* ssid = "SANYAMRANA";
const char* password = "test8950";

LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F
WebServer server(80);


#define MAX_MESSAGES 4
String messages[MAX_MESSAGES];
int msgCount = 0;
int displayIndex = 0;  // Which top message is currently displayed

unsigned long lastRotateTime = 0;
const unsigned long rotateInterval = 10000; // 10 seconds per message pair

unsigned long lastScrollTime = 0;
const unsigned long scrollInterval = 300;   // Speed of horizontal scroll


int scrollIndexLine1 = 0;
int scrollIndexLine2 = 0;

void handleRoot() {
  String html = "<h2>ESP32 LCD Message</h2>"
                "<form action=\"/set\" method=\"GET\">"
                "<input name=\"msg\" type=\"text\" style=\"width:200px\">"
                "<input type=\"submit\" value=\"Send\">"
                "</form>";
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("msg")) {
    String newMsg = server.arg("msg");
    Serial.println("New msg: " + newMsg);

    if (msgCount < MAX_MESSAGES) {
      messages[msgCount++] = newMsg;
    } else {
      for (int i = 1; i < MAX_MESSAGES; i++) {
        messages[i - 1] = messages[i];
      }
      messages[MAX_MESSAGES - 1] = newMsg;
    }

    displayIndex = (msgCount > 2) ? msgCount - 2 : 0;
    scrollIndexLine1 = 0;
    scrollIndexLine2 = 0;
    updateLCD();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void updateLCD() {
  lcd.clear();
  displayLine(0, displayIndex);       // top line
  displayLine(1, displayIndex + 1);   // bottom line
}


void displayLine(int line, int msgIndex) {
  if (msgIndex >= msgCount) return;

  String msg = messages[msgIndex];
  int scrollIndex = (line == 0) ? scrollIndexLine1 : scrollIndexLine2;

  if (msg.length() <= 16) {
    lcd.setCursor(0, line);
    lcd.print(msg);
  } else {
    String segment = msg.substring(scrollIndex, scrollIndex + 16);
    lcd.setCursor(0, line);
    lcd.print(segment);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");
  Serial.print("🌐 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("📡 Web Server started!");
}

void loop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - lastScrollTime >= scrollInterval) {
    lastScrollTime = now;

    if (displayIndex < msgCount && messages[displayIndex].length() > 16) {
      scrollIndexLine1++;
      if (scrollIndexLine1 > messages[displayIndex].length() - 16) {
        scrollIndexLine1 = 0;  // loop back
      }
    }

    if (displayIndex + 1 < msgCount && messages[displayIndex + 1].length() > 16) {
      scrollIndexLine2++;
      if (scrollIndexLine2 > messages[displayIndex + 1].length() - 16) {
        scrollIndexLine2 = 0;
      }
    }

    updateLCD();
  }

  if (now - lastRotateTime >= rotateInterval && msgCount > 2) {
    lastRotateTime = now;
    displayIndex++;
    if (displayIndex > msgCount - 2) {
      displayIndex = 0;
    }
    scrollIndexLine1 = 0;
    scrollIndexLine2 = 0;
    updateLCD();
  }
}
