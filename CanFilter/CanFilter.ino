#include <SPI.h>
#include <mcp_can.h>

//---------------------- Pin Definitions ----------------------//
#define CAN1_CS_PIN 10    // MCP2515 connected to CAN Bus 1
#define CAN2_CS_PIN 4     // MCP2515 connected to CAN Bus 2

#define LED_PIN     7

//---------------------- CAN Objects --------------------------//
MCP_CAN CAN1(CAN1_CS_PIN);
MCP_CAN CAN2(CAN2_CS_PIN);

//-------------------------------------------------------------//
bool initCAN1()
{
    if (CAN1.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) != CAN_OK)
    {
        Serial.println("CAN1 Initialization Failed");
        return false;
    }

    CAN1.setMode(MCP_NORMAL);
    Serial.println("CAN1 Initialized");
    return true;
}

//-------------------------------------------------------------//
bool initCAN2()
{
    if (CAN2.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) != CAN_OK)
    {
        Serial.println("CAN2 Initialization Failed");
        return false;
    }

    CAN2.setMode(MCP_NORMAL);
    Serial.println("CAN2 Initialized");
    return true;
}

//-------------------------------------------------------------//
void setup()
{
    Serial.begin(115200);

    pinMode(CAN1_CS_PIN, OUTPUT);
    pinMode(CAN2_CS_PIN, OUTPUT);

    digitalWrite(CAN1_CS_PIN, HIGH);
    digitalWrite(CAN2_CS_PIN, HIGH);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    while (!initCAN1());
    while (!initCAN2());

    Serial.println("One-Way CAN Gateway Started");
}

//-------------------------------------------------------------//
void loop()
{
    // Poll CAN Bus 1 for new messages
    if (CAN1.checkReceive() == CAN_MSGAVAIL)
    {
        unsigned long canID;
        byte len;
        byte data[8];

        // Read message from CAN Bus 1
        if (CAN1.readMsgBuf(&canID, &len, data) == CAN_OK)
        {
            // Forward message to CAN Bus 2
            if (CAN2.sendMsgBuf(canID, 0, len, data) == CAN_OK)
            {
                digitalWrite(LED_PIN, HIGH);

                Serial.print("Forwarded ID: 0x");
                Serial.print(canID, HEX);
                Serial.print("  Data: ");

                for (byte i = 0; i < len; i++)
                {
                    if (data[i] < 0x10) Serial.print("0");
                    Serial.print(data[i], HEX);
                    Serial.print(" ");
                }

                Serial.println();
            }
            else
            {
                Serial.println("CAN2 Transmit Failed");
                digitalWrite(LED_PIN, LOW);
            }
        }
    }
}