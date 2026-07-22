#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoWebsockets.h>

// #include <esp_websocket_client.h>

#include "Config.h"

using namespace websockets;

// WebsocketsClient client;
// WiFiClient tcp;
WiFiClientSecure tcp;

WebsocketsClient client;



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

    Serial.println("Trying to upgrade manually");
    tcp.print(
    "GET / HTTP/1.1\r\n"
    "Host: mac.taild17908.ts.net\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
    "Sec-WebSocket-Version: 13\r\n"
    "\r\n");



    // Serial.println("Testing HTTPS GET");
    // tcp.print(
    //     "GET / HTTP/1.1\r\n"
    //     "Host: mac.taild17908.ts.net\r\n"
    //     "User-Agent: ESP32\r\n"
    //     "Connection: close\r\n"
    //     "\r\n");




    // delay(5000);

    // tcp.print(
    //     "GET / HTTP/1.1\r\n"
    //     "Host: mac.taild17908.ts.net\r\n"
    //     "Connection: close\r\n"
    //     "\r\n");    

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
        String(WS_HOST) + //"/";
        ":443";
        // ":443/";
        //":443";
        
        // "ws://" +
        // String(WS_PATH);
        // ":" +
        // String(WS_PORT) +

    Serial.print("Connecting to ");
    Serial.println(url);

    client.setInsecure();
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
    // client.poll();

    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi lost.");

        WiFi.disconnect();
        connectWiFi();
    }

    unsigned long timeout = millis();

    while (tcp.connected() && millis() - timeout < 10000)
    {
        while (tcp.available())
        {
            char c = tcp.read();
            Serial.write(c);
            timeout = millis();      // reset timeout whenever data arrives
        }

        delay(10);
    }

    // while (tcp.available())
    // {
    //     // Serial.write(tcp.read());
    //     Serial.write(tcp.read());
    // }

    if (!tcp.connected()) {
        Serial.println("TCP Connection closed.");
        delay(9000);
        // break;
    }

    if(!client.available())
    {
        Serial.println("Reconnecting WebSocket...");

        delay(5000);

        connectWebSocket();
    }

    delay(1000);
}






////////////////////////////////////////////////////////////////////////////////


