// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <ArduinoWebsockets.h>

// // #include <esp_websocket_client.h>

// #include "Config.h"

// using namespace websockets;

// // WebsocketsClient client;
// // WiFiClient tcp;
// WiFiClientSecure tcp;

// WebsocketsClient client;



// void connectWiFi()
// {
//     Serial.print("Connecting to WiFi");

//     WiFi.mode(WIFI_STA);
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println();
//     Serial.println("WiFi connected");

//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());


//     IPAddress ip;

//     if (WiFi.hostByName("mac.taild17908.ts.net", ip))
//     {
//         Serial.print("Resolved to: ");
//         Serial.println(ip);
//         Serial.println("DNS works");
//     }
//     else
//     {
//         Serial.println("DNS failed");
//     }


//     // WiFiClientSecure Wclient;

//     tcp.setInsecure();

//     if (tcp.connect("mac.taild17908.ts.net", 443))
//     {
//         Serial.println("TLS works");
//     }
//     else
//     {
//         Serial.println("TLS failed");
//     }

//     Serial.println("Trying to upgrade manually");
//     tcp.print(
//     "GET / HTTP/1.1\r\n"
//     "Host: mac.taild17908.ts.net\r\n"
//     "Upgrade: websocket\r\n"
//     "Connection: Upgrade\r\n"
//     "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
//     "Sec-WebSocket-Version: 13\r\n"
//     "\r\n");



//     // Serial.println("Testing HTTPS GET");
//     // tcp.print(
//     //     "GET / HTTP/1.1\r\n"
//     //     "Host: mac.taild17908.ts.net\r\n"
//     //     "User-Agent: ESP32\r\n"
//     //     "Connection: close\r\n"
//     //     "\r\n");




//     // delay(5000);

//     // tcp.print(
//     //     "GET / HTTP/1.1\r\n"
//     //     "Host: mac.taild17908.ts.net\r\n"
//     //     "Connection: close\r\n"
//     //     "\r\n");    

//     // tcp.setInsecure();
//     // client.setClient(&tcp);

// }

// void connectWebSocket()
// {
        
//     client.onMessage([](WebsocketsMessage message)
//     {
//         Serial.print("Received: ");
//         Serial.println(message.data());
//     });

//     client.onEvent([](WebsocketsEvent event, String data)
//     {
//         switch(event)
//         {
//             case WebsocketsEvent::ConnectionOpened:
//                 Serial.println("WebSocket connected");
//                 client.send("Hello from ESP32");
//                 break;

//             case WebsocketsEvent::ConnectionClosed:
//                 Serial.println("WebSocket disconnected");
//                 break;

//             case WebsocketsEvent::GotPing:
//                 Serial.println("Ping");
//                 break;

//             case WebsocketsEvent::GotPong:
//                 Serial.println("Pong");
//                 break;
//         }
//     });

//     String url =
//         "wss://" +
//         String(WS_HOST) + //"/";
//         ":443/";
//         // ":443/";
//         //":443";
        
//         // "ws://" +
//         // String(WS_PATH);
//         // ":" +
//         // String(WS_PORT) +

//     Serial.print("Connecting to ");
//     Serial.println(url);

//     client.setInsecure();
//     bool ok = client.connect(url); //,443);

//     if(!ok)
//     {
//         Serial.println("Connection failed.");
//     }
// }

// void setup()
// {
//     Serial.begin(115200);

//     delay(1000);

//     Serial.println("Gaba! Can you read me?");

//     connectWiFi();

//     connectWebSocket();
// }

// void loop()
// {
//     // Serial.println("Glaba loop!");
//     // client.poll();

//     if(WiFi.status() != WL_CONNECTED)
//     {
//         Serial.println("WiFi lost.");

//         WiFi.disconnect();
//         connectWiFi();
//     }

//     unsigned long timeout = millis();

//     while (tcp.connected() && millis() - timeout < 10000)
//     {
//         while (tcp.available())
//         {
//             char c = tcp.read();
//             Serial.write(c);
//             timeout = millis();      // reset timeout whenever data arrives
//         }

//         delay(10);
//     }

//     // while (tcp.available())
//     // {
//     //     // Serial.write(tcp.read());
//     //     Serial.write(tcp.read());
//     // }

//     if (!tcp.connected()) {
//         Serial.println("TCP Connection closed.");
//         delay(9000);
//         // break;
//     }

//     if(!client.available())
//     {
//         Serial.println("Reconnecting WebSocket...");

//         delay(5000);

//         connectWebSocket();
//     }

//     delay(1000);
// }






// ////////////////////////////////////////////////////////////////////////////////
// THIS WORKED!!
// #include <WiFi.h>
// #include <WebSocketsClient.h>

// const char *ssid = "Airtel_7000000000";
// const char *password = "air81588";

// WebSocketsClient ws;

// void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
// {
//     switch (type)
//     {
//         case WStype_CONNECTED:
//             Serial.println("Connected!");
//             ws.sendTXT("Hello from ESP32");
//             break;

//         case WStype_TEXT:
//             Serial.print("RX: ");
//             Serial.println((char *)payload);
//             break;

//         case WStype_DISCONNECTED:
//             Serial.println("Disconnected");
//             break;

//         case WStype_ERROR:
//             Serial.println("WebSocket error");
//             break;

//         default:
//             break;
//     }
// }

// void setup()
// {
//     Serial.begin(115200);

//     WiFi.begin(ssid, password);

//     Serial.print("Connecting to WiFi");
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println("\nWiFi connected.");

//     //
//     // host, port, path, protocol
//     //
//     ws.beginSSL(
//         "mac.taild17908.ts.net",
//         443,
//         "/",
//         ""          // <-- disable "arduino" subprotocol
//     );

//     ws.onEvent(webSocketEvent);

//     ws.enableHeartbeat(15000, 3000, 2);

//     ws.setReconnectInterval(5000);
// }

// void loop()
// {
//     ws.loop();
// }



////////////////////////// TRYING TO ADD THE CRYPTO PART

#include <LittleFS.h>

#include "mbedtls/pk.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/base64.h"
#include "mbedtls/sha256.h"

////////////////////////
#include <WiFi.h>
#include <WebSocketsClient.h>
const char *ssid = "Airtel_7000000000";
const char *password = "air81588";

WebSocketsClient ws;
///////////////////////////


//----------------------------
mbedtls_pk_context pk;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;

const char *KEY_FILE = "/ecdsa.pem";

bool initRandom()
{
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    const char *pers = "esp32-ecdsa";

    int ret = mbedtls_ctr_drbg_seed(
        &ctr_drbg,
        mbedtls_entropy_func,
        &entropy,
        (const unsigned char *)pers,
        strlen(pers));

    return ret == 0;
}


bool generateKey()
{
    mbedtls_pk_init(&pk);

    int ret = mbedtls_pk_setup(
        &pk,
        mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));

    if (ret != 0)
        return false;

    ret = mbedtls_ecp_gen_key(
        MBEDTLS_ECP_DP_SECP256R1,
        mbedtls_pk_ec(pk),
        mbedtls_ctr_drbg_random,
        &ctr_drbg);

    return ret == 0;
}


bool saveKey()
{
    unsigned char buffer[1600];

    int len = mbedtls_pk_write_key_pem(
        &pk,
        buffer,
        sizeof(buffer));

    if (len != 0)
        return false;

    File f = LittleFS.open(KEY_FILE, "w");
    if (!f)
        return false;

    f.print((char *)buffer);
    f.close();

    return true;
}

bool loadKey()
{
    if (!LittleFS.exists(KEY_FILE))
        return false;

    File f = LittleFS.open(KEY_FILE, "r");
    if (!f)
        return false;

    String pem = f.readString();

    f.close();

    mbedtls_pk_init(&pk);

    int ret = mbedtls_pk_parse_key(
        &pk,
        (const unsigned char *)pem.c_str(),
        pem.length() + 1,
        NULL,
        0,
        mbedtls_ctr_drbg_random,
        &ctr_drbg);

    return ret == 0;
}


void printPublicKey()
{
    unsigned char buffer[800];

    int ret = mbedtls_pk_write_pubkey_pem(
        &pk,
        buffer,
        sizeof(buffer));

    if (ret == 0)
    {
        Serial.println("Public key:");
        Serial.println((char *)buffer);
    }
}



void signMessage(const String &message)
{
    unsigned char hash[32];

    mbedtls_sha256(
        (const unsigned char *)message.c_str(),
        message.length(),
        hash,
        0);

    unsigned char signature[100];
    size_t sigLen = 0;

    int ret = mbedtls_pk_sign(
        &pk,
        MBEDTLS_MD_SHA256,
        hash,
        0,
        signature,
        &sigLen,
        mbedtls_ctr_drbg_random,
        &ctr_drbg);

    if (ret != 0)
    {
        Serial.println("Signing failed");
        return;
    }

    unsigned char base64[300];
    size_t outLen;

    mbedtls_base64_encode(
        base64,
        sizeof(base64),
        &outLen,
        signature,
        sigLen);

    Serial.println("Signature:");
    Serial.println((char *)base64);
}
//---------------------------------


///////////////////////
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
        case WStype_CONNECTED:
            Serial.println("Connected!");
            ws.sendTXT("Hello from ESP32");
            break;

        case WStype_TEXT:
            Serial.print("RX: ");
            Serial.println((char *)payload);
            break;

        case WStype_DISCONNECTED:
            Serial.println("Disconnected");
            break;

        case WStype_ERROR:
            Serial.println("WebSocket error");
            break;

        default:
            break;
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected.");

//////-------------------v
    LittleFS.begin();
    initRandom();

    if (!loadKey())
    {
        Serial.println("Generating new key...");
        generateKey();
        saveKey();
    }
    else
    {
        Serial.println("Loaded existing key.");
    }    

    printPublicKey();

//////------------------------^


    //
    // host, port, path, protocol
    //
    ws.beginSSL(
        "mac.taild17908.ts.net",
        443,
        "/",
        ""          // <-- disable "arduino" subprotocol
    );

    ws.onEvent(webSocketEvent);

    ws.enableHeartbeat(15000, 3000, 2);

    ws.setReconnectInterval(5000);
}

void loop()
{
    ws.loop();
}

