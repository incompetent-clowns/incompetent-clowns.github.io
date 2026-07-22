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

// #include <LittleFS.h>

// #include "mbedtls/pk.h"
// #include "mbedtls/ecp.h"
// #include "mbedtls/ecdsa.h"
// #include "mbedtls/ctr_drbg.h"
// #include "mbedtls/entropy.h"
// #include "mbedtls/base64.h"
// #include "mbedtls/sha256.h"

#include <Preferences.h>

#include "mbedtls/ecdsa.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/sha256.h"
#include "mbedtls/base64.h"
static const uint8_t SPKI_PREFIX[] = {
    0x30, 0x59,             // SEQUENCE (89 bytes)
    0x30, 0x13,             // SEQUENCE (19 bytes)
    0x06, 0x07,             // OID (7 bytes)
    0x2A, 0x86, 0x48, 0xCE,
    0x3D, 0x02, 0x01,       // id-ecPublicKey
    0x06, 0x08,             // OID (8 bytes)
    0x2A, 0x86, 0x48, 0xCE,
    0x3D, 0x03, 0x01, 0x07, // prime256v1 (P-256)
    0x03, 0x42,             // BIT STRING (66 bytes)
    0x00                    // 0 unused bits
};
////////////////////////
#include <WiFi.h>
#include <WebSocketsClient.h>
const char *ssid = "Airtel_7000000000";
const char *password = "air81588";

WebSocketsClient ws;
///////////////////////////


//################
#include <ArduinoJson.h>
//################


//--------------------
Preferences prefs;

mbedtls_ecdsa_context key;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr;


void printHex(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (data[i] < 16)
            Serial.print("0");
        Serial.print(data[i], HEX);
    }
    Serial.println();
}

void initRandom()
{
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr);

    const char *pers = "ecdsa";

    int ret = mbedtls_ctr_drbg_seed(
        &ctr,
        mbedtls_entropy_func,
        &entropy,
        (const unsigned char *)pers,
        strlen(pers));

    if (ret != 0)
    {
        Serial.printf("RNG init failed: %d\n", ret);
        while (true)
            delay(1000);
    }
}

void loadOrCreateKey()
{
    prefs.begin("crypto", false);

    mbedtls_ecdsa_init(&key);

    mbedtls_ecp_group_load(
        &key.MBEDTLS_PRIVATE(grp),
        MBEDTLS_ECP_DP_SECP256R1);

    if (prefs.isKey("private"))
    {
        Serial.println("Loading existing key");

        uint8_t d[32];

        prefs.getBytes("private", d, 32);

        mbedtls_mpi_read_binary(
            &key.MBEDTLS_PRIVATE(d),
            d,
            32);

        int ret = mbedtls_ecp_mul(
            &key.MBEDTLS_PRIVATE(grp),
            &key.MBEDTLS_PRIVATE(Q),
            &key.MBEDTLS_PRIVATE(d),
            &key.MBEDTLS_PRIVATE(grp).G,
            mbedtls_ctr_drbg_random,
            &ctr);

        if (ret != 0)
        {
            Serial.printf("Public key rebuild failed %d\n", ret);
            while (true)
                delay(1000);
        }
    }
    else
    {
        Serial.println("Generating new key");

        int ret = mbedtls_ecdsa_genkey(
            &key,
            MBEDTLS_ECP_DP_SECP256R1,
            mbedtls_ctr_drbg_random,
            &ctr);

        if (ret != 0)
        {
            Serial.printf("Key generation failed %d\n", ret);
            while (true)
                delay(1000);
        }

        uint8_t d[32];

        mbedtls_mpi_write_binary(
            &key.MBEDTLS_PRIVATE(d),
            d,
            32);

        prefs.putBytes("private", d, 32);

        Serial.println("Private key stored.");
    }

    prefs.end();
}

void printPublicKey()
{
    uint8_t x[32];
    uint8_t y[32];

    mbedtls_mpi_write_binary(
        &key.MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X),
        x,
        32);

    mbedtls_mpi_write_binary(
        &key.MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y),
        y,
        32);    

    Serial.println("Public X:");
    printHex(x, 32);

    Serial.println("Public Y:");
    printHex(y, 32);
}

    // uint8_t sig[80];
    // size_t sigLen = 0;

    // int ret = mbedtls_ecdsa_write_signature(
    //     &key,
    //     MBEDTLS_MD_SHA256,
    //     hash,
    //     sizeof(hash),
    //     sig,
    //     &sigLen,
    //     mbedtls_ctr_drbg_random,
    //     &ctr);

String signMessage_(const String &msg)
{
    uint8_t hash[32];

    mbedtls_sha256(
        (const unsigned char *)msg.c_str(),
        msg.length(),
        hash,
        0);


    uint8_t sig[80];
    size_t sigLen = 0;

    int ret = mbedtls_ecdsa_write_signature(
        &key,
        MBEDTLS_MD_SHA256,
        hash,
        sizeof(hash),
        sig,
        sizeof(sig),
        &sigLen,
        mbedtls_ctr_drbg_random,
        &ctr);

    if (ret != 0)
    {
        Serial.printf("Sign failed %d\n", ret);
        return String("Failed");
    }

    Serial.print("Signature (" );
    Serial.print(sigLen);
    Serial.println(" bytes):");

    printHex(sig, sigLen);
    return String((char *)sig);
    // return String((char *)b64);
}

String signMessage(const String &msg)
{
    // Compute SHA-256 hash of the message
    uint8_t hash[32];

    mbedtls_sha256(
        reinterpret_cast<const unsigned char *>(msg.c_str()),
        msg.length(),
        hash,
        0);

    // r and s components of the signature
    mbedtls_mpi r, s;
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    int ret = mbedtls_ecdsa_sign(
        &key.MBEDTLS_PRIVATE(grp),
        &r,
        &s,
        &key.MBEDTLS_PRIVATE(d),
        hash,
        sizeof(hash),
        mbedtls_ctr_drbg_random,
        &ctr);

    if (ret != 0)
    {
        Serial.printf("Sign failed: %d\n", ret);

        mbedtls_mpi_free(&r);
        mbedtls_mpi_free(&s);

        return "";
    }

    // IEEE P1363 format = r || s
    uint8_t sig[64];

    mbedtls_mpi_write_binary(&r, sig, 32);
    mbedtls_mpi_write_binary(&s, sig + 32, 32);

    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    Serial.println("Signature (IEEE P1363):");
    printHex(sig, sizeof(sig));

    // Base64 encode the binary signature
    unsigned char b64[128];
    size_t b64Len = 0;

    ret = mbedtls_base64_encode(
        b64,
        sizeof(b64),
        &b64Len,
        sig,
        sizeof(sig));

    if (ret != 0)
    {
        Serial.println("Base64 encoding failed.");
        return "";
    }

    b64[b64Len] = '\0';

    return String(reinterpret_cast<char *>(b64));
}

String getPublicKeySPKI()
{
    uint8_t x[32];
    uint8_t y[32];

    mbedtls_mpi_write_binary(
        &key.MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X),
        x,
        sizeof(x));

    mbedtls_mpi_write_binary(
        &key.MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y),
        y,
        sizeof(y));

    uint8_t der[91];

    // Fixed SPKI header
    memcpy(der, SPKI_PREFIX, sizeof(SPKI_PREFIX));

    // Uncompressed EC point
    der[26] = 0x04;

    memcpy(der + 27, x, 32);
    memcpy(der + 59, y, 32);

    // Optional: print DER as hex
    Serial.println("SPKI DER:");
    printHex(der, sizeof(der));

    // Base64 encode for transmission
    unsigned char b64[128];
    size_t b64Len = 0;

    int ret = mbedtls_base64_encode(
        b64,
        sizeof(b64),
        &b64Len,
        der,
        sizeof(der));

    if (ret == 0)
    {
        b64[b64Len] = '\0';
        Serial.println("SPKI Base64:");
        Serial.println((char *)b64);
        return String((char *)b64);
    }
}


///////////////////////
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
        case WStype_CONNECTED:{
            Serial.println("Connected!");
            // ws.sendTXT("Hello from ESP32");
            break;}

        case WStype_TEXT:{
            Serial.print("RX: ");
            Serial.println((char *)payload);            
            JsonDocument request;
            // deserializeJson(request,payload);
            // DeserializationError err = deserializeJson(request, payload);

            // if (err)
            // {
            //     Serial.println("Invalid JSON");
            //     return;
            // }            
            //If Json request, 
            // if(!err){
            // if(1){
            if(!deserializeJson(request,(char *)payload)){
                // Serial.println(request["Type"]);
                if(request["Type"]=="auth"){
                    Serial.println("Authentication requested by the server.");
                    JsonObject payload_ = request["payload"];

                    String str_challenge = payload_["challenge"];

                    // JsonDocument
                    JsonDocument response;
                    response["Type"] = "device";
                    JsonObject device_ = response["device"].to<JsonObject>();
                    device_["publicKey"] = getPublicKeySPKI(); //"Working on it";
                    device_["id"] = "ESP32-1";
                    device_["signature"] = signMessage(str_challenge); //"Working on it";
                    device_["Type"] = "ESP32";
                    String str_response;
                    serializeJson(response,str_response);
                    Serial.println("Sending:" + str_response);
                    ws.sendTXT(str_response);
                }
            }
            break;}

            // ws.sendTXT("{\"Type\":\"device\"}")

        case WStype_DISCONNECTED:{
            Serial.println("Disconnected");
            break;}

        case WStype_ERROR:{
            Serial.println("WebSocket error");
            break;}

        default:{
            break;}
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
///////---------------------v
    initRandom();

    loadOrCreateKey();

    printPublicKey();

    signMessage("Hello World");
///////---------------------^


//////-------------------v
    // LittleFS.begin();
    // initRandom();

    // if (!loadKey())
    // {
    //     Serial.println("Generating new key...");
    //     generateKey();
    //     saveKey();
    // }
    // else
    // {
    //     Serial.println("Loaded existing key.");
    // }    

    // printPublicKey();

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

