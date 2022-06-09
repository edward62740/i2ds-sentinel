/**
 * Google's Firebase Token Generation class, Signer.h version 1.2.26
 *
 * This library supports Espressif ESP8266 and ESP32
 *
 * Created April 25, 2022
 *
 * This work is a part of Firebase ESP Client library
 * Copyright (c) 2022 K. Suwatchai (Mobizt)
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FIREBASE_SIGNER_H
#define FIREBASE_SIGNER_H

#include <Arduino.h>
#include "FB_Utils.h"
#include "./wcs/FB_Clients.h"
#include "./FirebaseFS.h"
#include "./mbfs/MB_FS.h"

using namespace mb_string;

class Firebase_Signer
{
#if defined(FIREBASE_ESP_CLIENT)
    friend class Firebase_ESP_Client;
    friend class FB_CM;
    friend class FB_Storage;
    friend class GG_CloudStorage;
    friend class FB_Firestore;
    friend class FB_Functions;
    friend class Binding;
    friend class PolicyBuilder;
    friend class AuditLogConfig;
    friend class AuditConfig;
    friend class FunctionsConfig;

#elif defined(FIREBASE_ESP32_CLIENT) || defined(FIREBASE_ESP8266_CLIENT)

#if defined(ESP32)
#define FIREBASE_CLASS FirebaseESP32
#elif defined(ES8266)
#define FIREBASE_CLASS FirebaseESP8266
#endif

    friend class FIREBASE_CLASS;
    friend class FCMObject;
#endif
    friend class FIREBASE_STREAM_CLASS;
    friend class FIREBASE_MP_STREAM_CLASS;
    friend class UtilsClass;
    friend class FB_RTDB;
    friend class FirebaseData;
    friend class QueryFilter;

public:
    Firebase_Signer();
    ~Firebase_Signer();

private:
    FB_TCPConnectionRequestCallback tcpConnectionCB = NULL;
    FB_NetworkConnectionRequestCallback networkConnectionCB = NULL;
    FB_NetworkStatusRequestCallback networkStatusCB = NULL;

    Client *extClient = nullptr;
    UtilsClass *ut = nullptr;
    FirebaseConfig *config = nullptr;
    FirebaseAuth *auth = nullptr;
    MB_FS *mbfs = nullptr;
#if defined(ESP8266)
    callback_function_t _cb = nullptr;
#endif
    struct token_info_t tokenInfo;
    bool authenticated = false;
    bool _token_processing_task_enable = false;
    unsigned long unauthen_millis = 0;
    unsigned long unauthen_pause_duration = 3000;
    FB_TCP_CLIENT *tcpClient = nullptr;
    FirebaseJson *jsonPtr = nullptr;
    FirebaseJsonData *resultPtr = nullptr;
    int response_code = 0;
    time_t ts = 0;
    bool autoReconnectWiFi = false;
    unsigned long last_reconnect_millis = 0;
    uint16_t reconnect_tmo = 10 * 1000;
    volatile bool networkStatus = false;

    void begin(UtilsClass *ut, MB_FS *mbfs, FirebaseConfig *config, FirebaseAuth *auth);
    bool parseSAFile();
    void clearSA();
    bool tokenSigninDataReady();
    void setTokenType(fb_esp_auth_token_type type);
    bool userSigninDataReady();
    bool isAuthToken(bool admin);
    bool isExpired();
    bool handleToken();
    bool parseJsonResponse(PGM_P key_path);
    bool refreshToken();
    void setTokenError(int code);
    bool handleSignerError(int code, int httpCode = 0);
    bool handleTokenResponse(int &httpCode);
    void tokenProcessingTask();
    bool createJWT();
    bool getIdToken(bool createUser, MB_StringPtr email, MB_StringPtr password);
    bool deleteIdToken(MB_StringPtr idToken);
    bool requestTokens();
    void checkToken();
    void getExpiration(const char *exp);
    bool handleEmailSending(MB_StringPtr payload, fb_esp_user_email_sending_type type);
    void errorToString(int httpCode, MB_String &buff);
    bool tokenReady();
    void sendTokenStatusCB();
    const char *getToken();
    bool authChanged(FirebaseConfig *config, FirebaseAuth *auth);
    fb_esp_auth_token_type getTokenType();
    MB_String getCAFile();
    fb_esp_mem_storage_type getCAFileStorage();
    FirebaseConfig *getCfg();
    FirebaseAuth *getAuth();
    MB_FS *getMBFS();
    UtilsClass *getUtils();
    bool initClient(PGM_P subDomain, fb_esp_auth_token_status status = token_status_uninitialized);
    void setClient(Client *client);
    void setTCPConnectionCallback(FB_TCPConnectionRequestCallback tcpConnectionCB);
    void setNetworkConnectionCallback(FB_NetworkConnectionRequestCallback networkConnectionCB);
    void setNetworkStatusCallback(FB_NetworkStatusRequestCallback networkStatusCB);
    void setNetworkStatus(bool status);
    time_t getTime();
    bool setTime(time_t ts);
    void setAutoReconnectWiFi(bool reconnect);

#if defined(ESP8266)
    void set_scheduled_callback(callback_function_t callback)
    {
        _cb = std::move([callback]()
                        { schedule_function(callback); });
        _cb();
    }
#endif
};

extern Firebase_Signer Signer;

#endif