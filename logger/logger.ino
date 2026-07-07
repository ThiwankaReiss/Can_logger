#include <SPI.h>
#include <mcp_can.h>
#include <SD.h>

// Pin definitions
#define CAN_CS_PIN  10
#define CAN_INT_PIN 2
#define SD_CS_PIN   4

const int LED_PIN = 7;   // Logging indicator LED

MCP_CAN CAN(CAN_CS_PIN);
File logFile;

bool canReady  = false;
bool fileReady = false;

unsigned long lastFilePosition = 0;
unsigned long lastRetryMillis  = 0;
#define RETRY_INTERVAL_MS 2000   // retry CAN / SD every 2 s when not ready

// ---------- Try to initialise CAN ----------
bool initCAN() {
  if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("CAN initialization failed.");
    return false;
  }
  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN initialized.");
  return true;
}

// ---------- Try to initialise SD and open a new log file ----------
bool initSDAndFile() {
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD initialization failed!");
    return false;
  }

  // Close any previously open handle
  if (logFile) logFile.close();

  char filename[20];
  strcpy(filename, "canlog.csv");

  if (SD.exists(filename)) {
    int fileIndex = 1;
    do {
      snprintf(filename, sizeof(filename), "canlog%d.csv", fileIndex);
      fileIndex++;
    } while (SD.exists(filename));
  }

  logFile = SD.open(filename, FILE_WRITE);
  if (!logFile) {
    Serial.println("Cannot open log file!");
    return false;
  }

  logFile.println("Millis,CAN_ID,DLC,Data0,Data1,Data2,Data3,Data4,Data5,Data6,Data7");
  logFile.flush();

  Serial.print("=== Logging Started === File: ");
  Serial.println(filename);
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Configure chip select pins
  pinMode(CAN_CS_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(CAN_CS_PIN, HIGH);
  digitalWrite(SD_CS_PIN, HIGH);

  // Hardware SPI requires SS pin to be output on Arduino Nano/Uno
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  // LED output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Attempt init at boot — failures are NOT fatal; loop() will retry
  canReady  = initCAN();
  fileReady = initSDAndFile();
}

void loop() {
  // ---------- Retry failed initialisations on a timed interval ----------
  if (!canReady || !fileReady) {
    unsigned long now = millis();
    if (now - lastRetryMillis >= RETRY_INTERVAL_MS) {
      lastRetryMillis = now;
      if (!canReady)  canReady  = initCAN();
      if (!fileReady) fileReady = initSDAndFile();
    }
    digitalWrite(LED_PIN, LOW);
    return;   // nothing to log until both are ready
  }

  // ---------- Check if MCP2515 indicates a received CAN frame ----------
  if (digitalRead(CAN_INT_PIN) == LOW) {
    unsigned long canId;
    byte len;
    byte buf[8];

    if (CAN.readMsgBuf(&canId, &len, buf) == CAN_OK) {
      unsigned long ts = millis();

      // Confirm SD is still accessible
      if (SD.begin(SD_CS_PIN)) {

        lastFilePosition = logFile.position();

        // ---------- Write CSV Record ----------
        logFile.print(ts);
        logFile.print(",");
        logFile.print(canId, HEX);
        logFile.print(",");
        logFile.print(len);
        logFile.print(",");

        for (byte i = 0; i < 8; i++) {
          if (i > 0) logFile.print(",");
          if (i < len) {
            if (buf[i] < 0x10) logFile.print("0");
            logFile.print(buf[i], HEX);
          }
        }

        logFile.println();
        logFile.flush();

        if (logFile.position() > lastFilePosition) {
          digitalWrite(LED_PIN, HIGH);   // Logging successful
        } else {
          // Write failed — SD likely removed; reopen file on next retry
          Serial.println("Write failed — SD removed?");
          logFile.close();
          fileReady = false;
          digitalWrite(LED_PIN, LOW);
        }

      } else {
        // SD card missing or inaccessible
        Serial.println("SD not accessible — will retry.");
        logFile.close();
        fileReady = false;
        digitalWrite(LED_PIN, LOW);
      }

    } else {
      // CAN read error — single failure; do not reset canReady
      digitalWrite(LED_PIN, LOW);
    }
  }
}