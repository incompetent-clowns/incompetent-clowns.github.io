#include <WiFi.h>
#include <ArduinoWebsockets.h>

#include "Config.h"

using namespace websockets;

WebsocketsClient client;
WiFiClientSecure tcp;

// WebsocketsClient client;



void connectWiFi()
{
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());


    IPAddress ip;

    if (WiFi.hostByName("mac.taild17908.ts.net", ip))
    {
        Serial.print("Resolved to: ");
        Serial.println(ip);
        Serial.println("DNS works");
    }
    else
    {
        Serial.println("DNS failed");
    }


    // WiFiClientSecure Wclient;

    tcp.setInsecure();

    if (tcp.connect("mac.taild17908.ts.net", 443))
    {
        Serial.println("TLS works");
    }
    else
    {
        Serial.println("TLS failed");
    }

    // tcp.setInsecure();
    // client.setClient(&tcp);

}

void connectWebSocket()
{
        
    client.onMessage([](WebsocketsMessage message)
    {
        Serial.print("Received: ");
        Serial.println(message.data());
    });

    client.onEvent([](WebsocketsEvent event, String data)
    {
        switch(event)
        {
            case WebsocketsEvent::ConnectionOpened:
                Serial.println("WebSocket connected");
                client.send("Hello from ESP32");
                break;

            case WebsocketsEvent::ConnectionClosed:
                Serial.println("WebSocket disconnected");
                break;

            case WebsocketsEvent::GotPing:
                Serial.println("Ping");
                break;

            case WebsocketsEvent::GotPong:
                Serial.println("Pong");
                break;
        }
    });

    String url =
        "wss://" +
        String(WS_HOST) +
        ":443/";
        //":443";
        
        // "ws://" +
        // String(WS_PATH);
        // ":" +
        // String(WS_PORT) +

    Serial.print("Connecting to ");
    Serial.println(url);

    // client.setInsecure();
    bool ok = client.connect(url); //,443);

    if(!ok)
    {
        Serial.println("Connection failed.");
    }
}

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println("Gaba! Can you read me?");

    connectWiFi();

    connectWebSocket();
}

void loop()
{
    // Serial.println("Glaba loop!");
    client.poll();

    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi lost.");

        WiFi.disconnect();
        connectWiFi();
    }

    if(!client.available())
    {
        Serial.println("Reconnecting WebSocket...");

        delay(5000);

        connectWebSocket();
    }

    delay(10);
}