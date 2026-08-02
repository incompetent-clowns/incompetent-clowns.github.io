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
const char *ssid = "TP-Link_5DBC_2.4GHz";
const char *password = "20059132";
// const char *ssid = "Airtel_7000000000";
// const char *password = "air81588";

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


///// MOTOR
// const int in1 = 26;
// const int in2 = 25;
// const int in3 = 33;
// const int in4 = 32;
// const int in1 = 4;
// const int in2 = 5;
// const int in3 = 6;
// const int in4 = 7;

const int in1 = 26;
const int in2 = 25;
const int in3 = 33;
const int in4 = 32;


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
            JsonDocument request;

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
                    device_["id"] = "ESP32-H";
                    device_["signature"] = signMessage(str_challenge); //"Working on it";
                    device_["Type"] = "ESP32";
                    String str_response;
                    serializeJson(response,str_response);
                    Serial.println("Sending:" + str_response);
                    ws.sendTXT(str_response);
                }

                if(request["Type"] == "cmd" && request["from"] == "atul.singh.arora@gmail.com"){
                    Serial.println("Command sent by user.");
                    JsonDocument payload_ = request["payload"]; //.to<JsonObject>();
                    if (payload_["dir"]=="right"){
                        if(payload_["isPressed"]){
                            digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
                            digitalWrite(in3, HIGH); digitalWrite(in4, LOW);                            
                            Serial.println("Moving right");
                        }
                        else{
                                                     digitalWrite(in2, LOW);
                            digitalWrite(in3, LOW); 

                            Serial.println("Stop moving right");
                        }
                    }
                    if (payload_["dir"]=="left"){
                        if(payload_["isPressed"]){
                            //left
                            digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
                            digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
                            Serial.println("Moving left");                                                        
                        }
                        else{
                            digitalWrite(in1, LOW); 
                                                    digitalWrite(in4, LOW);
                            Serial.println("Stop moving left");
                        }

                    }
                    if (payload_["dir"]=="up"){
                        if(payload_["isPressed"]){
                            //forward
                            digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
                            digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
                            Serial.println("Moving up");
                        }
                        else{
                            digitalWrite(in2, LOW);
                                                    digitalWrite(in4, LOW);                            
                            Serial.println("Stop moving up");
                        }

                    }
                    if (payload_["dir"]=="down"){
                        if(payload_["isPressed"]){
                            //backwards
                            digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
                            digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
                            Serial.println("Moving down");
                        }
                        else{
                            digitalWrite(in1, LOW);// digitalWrite(in2, LOW);
                            digitalWrite(in3, LOW);// digitalWrite(in4, LOW);
                            Serial.println("Stop moving down");
                        }

                    }

                }
            }
            else
            {
                Serial.print("RX: ");
                Serial.println((char *)payload);
            }
            break;}

            // ws.sendTXT("{\"Type\":\"device\"}")

        case WStype_DISCONNECTED:{
            
            digitalWrite(in1, LOW); digitalWrite(in2, LOW);
            digitalWrite(in3, LOW); digitalWrite(in4, LOW);        

            Serial.println("Disconnected");
            Serial.println("Trying to reconnect.");
            ws.beginSSL(
                "mac.taild17908.ts.net",
                443,
                "/",
                ""          // <-- disable "arduino" subprotocol
            );
            delay(5000);
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
    Serial.println("Initialising Crypto Primitives.");
    initRandom();

    loadOrCreateKey();

    printPublicKey();

    signMessage("Hello World");
///////---------------------^

    Serial.println("Initialising motor pins.");
    // Set motor pins as output
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    Serial.println("Motor pins configured.")



////////////////////
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

