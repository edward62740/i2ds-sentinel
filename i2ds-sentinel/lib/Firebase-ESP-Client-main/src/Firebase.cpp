/**
 * The Firebase class, Firebase.cpp v1.1.0
 *
 *  Created May 17, 2022
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

#ifndef Firebase_CPP
#define Firebase_CPP

#include "Firebase.h"

#if defined(ESP8266) || defined(ESP32) || defined(FB_ENABLE_EXTERNAL_CLIENT)

#if defined(FIREBASE_ESP_CLIENT)

Firebase_ESP_Client::Firebase_ESP_Client()
{

    if (!mbfs)
        mbfs = new MB_FS();

    if (!ut)
        ut = new UtilsClass(mbfs);

    Signer.begin(ut, mbfs, nullptr, nullptr);
}

Firebase_ESP_Client::~Firebase_ESP_Client()
{
    if (ut)
        delete ut;

    if (mbfs)
        delete mbfs;

    if (auth)
        delete auth;

    if (cfg)
    {
        cfg->internal.so_addr_list.clear();
        cfg->internal.queue_addr_list.clear();
        delete cfg;
    }
}

void Firebase_ESP_Client::begin(FirebaseConfig *config, FirebaseAuth *auth)
{
    init(config, auth);

    if (!cfg->signer.test_mode)
    {
        Signer.authChanged(config, auth);

        struct fb_esp_url_info_t uinfo;
        cfg->internal.fb_auth_uri = cfg->signer.tokens.token_type == token_type_legacy_token || cfg->signer.tokens.token_type == token_type_id_token;

        if (cfg->host.length() > 0)
            cfg->database_url = cfg->host;

        if (cfg->database_url.length() > 0)
        {
            ut->getUrlInfo(cfg->database_url.c_str(), uinfo);
            cfg->database_url = uinfo.host.c_str();
        }

        if (cfg->cert.file.length() > 0)
            mbfs->checkStorageReady(mbfs_type cfg->cert.file_storage);
    }

    Signer.handleToken();
}

struct token_info_t Firebase_ESP_Client::authTokenInfo()
{
    return Signer.tokenInfo;
}

bool Firebase_ESP_Client::ready()
{
    return Signer.tokenReady();
}

bool Firebase_ESP_Client::authenticated()
{
    return Signer.authenticated;
}

bool Firebase_ESP_Client::mSignUp(FirebaseConfig *config, FirebaseAuth *auth, MB_StringPtr email, MB_StringPtr password)
{
    init(config, auth);
    Signer.setTokenType(token_type_id_token);
    return Signer.getIdToken(true, email, password);
}

bool Firebase_ESP_Client::msendEmailVerification(FirebaseConfig *config, MB_StringPtr idToken)
{
    init(config, nullptr);
    return Signer.handleEmailSending(idToken, fb_esp_user_email_sending_type_verify);
}

bool Firebase_ESP_Client::mDeleteUser(FirebaseConfig *config, FirebaseAuth *auth, MB_StringPtr idToken)
{
    init(config, auth);
    return Signer.deleteIdToken(idToken);
}

bool Firebase_ESP_Client::mSendResetPassword(FirebaseConfig *config, MB_StringPtr email)
{
    init(config, nullptr);
    return Signer.handleEmailSending(email, fb_esp_user_email_sending_type_reset_psw);
}

void Firebase_ESP_Client::mSetIdToken(FirebaseConfig *config, MB_StringPtr idToken, size_t expire, MB_StringPtr refreshToken)
{
    if (!config)
        return;

    MB_String _idToken = idToken;
    config->internal.refresh_token.clear();
    config->internal.refresh_token = refreshToken;

    if (_idToken.length() > 0)
    {
        if (_idToken.length() == 0 || strcmp(config->internal.auth_token.c_str(), _idToken.c_str()) == 0)
            return;

        _idToken.clear();

        config->internal.auth_token = idToken;
        config->internal.atok_len = config->internal.auth_token.length();
        config->internal.ltok_len = 0;

        if (expire > 3600)
            expire = 3600;

        config->signer.tokens.expires += Signer.getTime() + expire;

        config->signer.tokens.status = token_status_ready;
        config->signer.step = fb_esp_jwt_generation_step_begin;
        config->internal.fb_last_jwt_generation_error_cb_millis = 0;
        config->signer.tokens.token_type = token_type_id_token;
        config->signer.anonymous = true;
        config->signer.idTokenCutomSet = true;
    }
}

bool Firebase_ESP_Client::isTokenExpired()
{
    return Signer.isExpired();
}

void Firebase_ESP_Client::init(FirebaseConfig *config, FirebaseAuth *auth)
{
    this->auth = auth;
    cfg = config;

    if (!cfg)
        cfg = new FirebaseConfig();

    if (!this->auth)
        this->auth = new FirebaseAuth();

    ut->setConfig(cfg);

#ifdef ENABLE_RTDB
    RTDB.begin(ut);
#endif
#ifdef ENABLE_FCM
    FCM.begin(ut);
#endif
#ifdef ENABLE_FB_STORAGE
    Storage.begin(ut);
#endif
#ifdef ENABLE_FIRESTORE
    Firestore.begin(ut);
#endif
#ifdef ENABLE_FB_FUNCTIONS
    Functions.begin(ut);
#endif
#ifdef ENABLE_GC_STORAGE
    GCStorage.begin(ut);
#endif

    cfg->internal.fb_reconnect_wifi = Signer.autoReconnectWiFi;

    cfg->signer.lastReqMillis = 0;

    if (!cfg->signer.anonymous && !cfg->signer.signup)
        cfg->signer.tokens.expires = 0;

    cfg->signer.signup = false;
    Signer.begin(ut, mbfs, cfg, auth);
    cfg->signer.tokens.error.message.clear();
}

void Firebase_ESP_Client::reconnectWiFi(bool reconnect)
{
#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(reconnect);
#endif
    Signer.setAutoReconnectWiFi(reconnect);
}

time_t Firebase_ESP_Client::getCurrentTime()
{
    return Signer.getTime();
}

int Firebase_ESP_Client::getFreeHeap()
{
#if defined(ESP32) || defined(ESP8266)
    return ESP.getFreeHeap();
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_NANO_RP2040_CONNECT) || defined(TEENSYDUINO)
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
#else
    return 0;
#endif
}

const char *Firebase_ESP_Client::getToken()
{
    return Signer.getToken();
}

void Firebase_ESP_Client::setFloatDigits(uint8_t digits)
{
    if (!cfg)
        return;

    if (digits < 7 && cfg)
        cfg->internal.fb_float_digits = digits;
}

void Firebase_ESP_Client::setDoubleDigits(uint8_t digits)
{
    if (!cfg)
        return;

    if (digits < 9 && cfg)
        cfg->internal.fb_double_digits = digits;
}

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

bool Firebase_ESP_Client::sdBegin(int8_t ss, int8_t sck, int8_t miso, int8_t mosi, uint32_t frequency)
{
    return mbfs->sdBegin(ss, sck, miso, mosi, frequency);
}

#if defined(ESP8266)
bool Firebase_ESP_Client::sdBegin(SDFSConfig *sdFSConfig)
{
    return mbfs->sdFatBegin(sdFSConfig);
}
#endif

#if defined(ESP32)

bool Firebase_ESP_Client::sdBegin(int8_t ss, SPIClass *spiConfig, uint32_t frequency)
{
    return mbfs->sdSPIBegin(ss, spiConfig, frequency);
}
#endif

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
bool Firebase_ESP_Client::sdBegin(SdSpiConfig *sdFatSPIConfig, int8_t ss, int8_t sck, int8_t miso, int8_t mosi)
{
    return mbfs->sdFatBegin(sdFatSPIConfig, ss, sck, miso, mosi);
}
#endif

#endif

#if defined(ESP32) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD_MMC)

bool Firebase_ESP_Client::sdMMCBegin(const char *mountpoint, bool mode1bit, bool format_if_mount_failed)
{

    return mbfs->sdMMCBegin(mountpoint, mode1bit, format_if_mount_failed);
}

#endif

bool Firebase_ESP_Client::setSystemTime(time_t ts)
{
    return Signer.setTime(ts);
}

Firebase_ESP_Client Firebase = Firebase_ESP_Client();

#elif defined(FIREBASE_ESP32_CLIENT) || defined(FIREBASE_ESP8266_CLIENT)

FIREBASE_CLASS::FIREBASE_CLASS()
{
    if (!mbfs)
        mbfs = new MB_FS();

    if (!ut)
        ut = new UtilsClass(mbfs);

    Signer.begin(ut, mbfs, nullptr, nullptr);
}

FIREBASE_CLASS::~FIREBASE_CLASS()
{
    if (ut)
        delete ut;

    if (mbfs)
        delete mbfs;

    if (cfg)
    {
        cfg->internal.so_addr_list.clear();
        cfg->internal.queue_addr_list.clear();
    }

    if (!extConfig)
    {
        if (cfg)
            delete cfg;

        if (auth)
            delete auth;
    }
}

void FIREBASE_CLASS::begin(FirebaseConfig *config, FirebaseAuth *auth)
{
    init(config, auth);

    if (!cfg->signer.test_mode)
    {

        Signer.authChanged(config, auth);

        struct fb_esp_url_info_t uinfo;
        cfg->internal.fb_auth_uri = cfg->signer.tokens.token_type == token_type_legacy_token || cfg->signer.tokens.token_type == token_type_id_token;

        if (cfg->host.length() > 0)
            cfg->database_url = cfg->host;

        if (cfg->database_url.length() > 0)
        {
            ut->getUrlInfo(cfg->database_url.c_str(), uinfo);
            cfg->database_url = uinfo.host.c_str();
        }

        if (cfg->cert.file.length() > 0)
            mbfs->checkStorageReady(mbfs_type cfg->cert.file_storage);
    }
    Signer.handleToken();
}

void FIREBASE_CLASS::end(FirebaseData &fbdo)
{
#ifdef ENABLE_RTDB
    endStream(fbdo);
    removeStreamCallback(fbdo);
#endif
    fbdo.clear();
}

struct token_info_t FIREBASE_CLASS::authTokenInfo()
{
    return Signer.tokenInfo;
}

bool FIREBASE_CLASS::ready()
{
    return Signer.tokenReady();
}

bool FIREBASE_CLASS::authenticated()
{
    return Signer.authenticated;
}

bool FIREBASE_CLASS::mSignUp(FirebaseConfig *config, FirebaseAuth *auth, MB_StringPtr email, MB_StringPtr password)
{
    init(config, auth);
    Signer.setTokenType(token_type_id_token);
    return Signer.getIdToken(true, email, password);
}

bool FIREBASE_CLASS::msendEmailVerification(FirebaseConfig *config, MB_StringPtr idToken)
{
    init(config, nullptr);
    return Signer.handleEmailSending(idToken, fb_esp_user_email_sending_type_verify);
}

bool FIREBASE_CLASS::mDeleteUser(FirebaseConfig *config, FirebaseAuth *auth, MB_StringPtr idToken)
{
    init(config, auth);
    return Signer.deleteIdToken(idToken);
}

bool FIREBASE_CLASS::mSendResetPassword(FirebaseConfig *config, MB_StringPtr email)
{
    init(config, nullptr);
    return Signer.handleEmailSending(email, fb_esp_user_email_sending_type_reset_psw);
}

void FIREBASE_CLASS::mSetIdToken(FirebaseConfig *config, MB_StringPtr idToken, size_t expire, MB_StringPtr refreshToken)
{
    if (!config)
        return;

    MB_String _idToken = idToken;
    config->internal.refresh_token.clear();
    config->internal.refresh_token = refreshToken;

    if (_idToken.length() > 0)
    {
        if (_idToken.length() == 0 || strcmp(config->internal.auth_token.c_str(), _idToken.c_str()) == 0)
            return;

        _idToken.clear();

        config->internal.auth_token = idToken;
        config->internal.atok_len = config->internal.auth_token.length();
        config->internal.ltok_len = 0;

        if (expire > 3600)
            expire = 3600;

        config->signer.tokens.expires += Signer.getTime() + expire;

        config->signer.tokens.status = token_status_ready;
        config->signer.step = fb_esp_jwt_generation_step_begin;
        config->internal.fb_last_jwt_generation_error_cb_millis = 0;
        config->signer.tokens.token_type = token_type_id_token;
        config->signer.anonymous = true;
        config->signer.idTokenCutomSet = true;
    }
}

bool FIREBASE_CLASS::isTokenExpired()
{
    return Signer.isExpired();
}

void FIREBASE_CLASS::init(FirebaseConfig *config, FirebaseAuth *auth)
{
    if (!this->auth)
        this->auth = auth;

    if (!this->cfg)
        this->cfg = config;

    if (!this->cfg)
        this->cfg = new FirebaseConfig();

    if (!this->auth)
        this->auth = new FirebaseAuth();

    ut->setConfig(cfg);

#ifdef ENABLE_RTDB
    RTDB.begin(ut);
#endif

    cfg->internal.fb_reconnect_wifi = Signer.autoReconnectWiFi;

    cfg->signer.lastReqMillis = 0;

    if (!cfg->signer.anonymous && !cfg->signer.signup)
        cfg->signer.tokens.expires = 0;

    cfg->signer.signup = false;
    Signer.begin(ut, mbfs, this->cfg, this->auth);
    cfg->signer.tokens.error.message.clear();
}

void FIREBASE_CLASS::reconnectWiFi(bool reconnect)
{
#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(reconnect);
#endif
    Signer.setAutoReconnectWiFi(reconnect);
}

const char *FIREBASE_CLASS::getToken()
{
    return Signer.getToken();
}

int FIREBASE_CLASS::getFreeHeap()
{
#if defined(ESP32) || defined(ESP8266)
    return ESP.getFreeHeap();
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_NANO_RP2040_CONNECT) || defined(TEENSYDUINO)
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
#else
    return 0;
#endif
}

time_t FIREBASE_CLASS::getCurrentTime()
{
    return Signer.getTime();
}

void FIREBASE_CLASS::setFloatDigits(uint8_t digits)
{
    if (!cfg)
        return;

    if (digits < 7)
        cfg->internal.fb_float_digits = digits;
}

void FIREBASE_CLASS::setDoubleDigits(uint8_t digits)
{
    if (!cfg)
        return;

    if (digits < 9)
        cfg->internal.fb_double_digits = digits;
}

#ifdef ENABLE_FCM
bool FIREBASE_CLASS::handleFCMRequest(FirebaseData &fbdo, fb_esp_fcm_msg_type messageType)
{
    fbdo.tcpClient.setSPIEthernet(fbdo.fcm._spi_ethernet_module);

    if (!fbdo.reconnect())
        return false;

    if (!ut->waitIdle(fbdo.session.http_code))
        return false;

    FirebaseJsonData data;

    FirebaseJson *json = fbdo.to<FirebaseJson *>();
    json->setJsonData(fbdo.fcm.raw);

    MB_String s = fb_esp_pgm_str_577;

    json->get(data, s.c_str());

    if (data.stringValue.length() == 0)
    {
        fbdo.session.http_code = FIREBASE_ERROR_NO_FCM_SERVER_KEY_PROVIDED;
        return false;
    }

    if (fbdo.fcm.idTokens.length() == 0 && messageType == fb_esp_fcm_msg_type::msg_single)
    {
        fbdo.session.http_code = FIREBASE_ERROR_NO_FCM_ID_TOKEN_PROVIDED;
        return false;
    }

    FirebaseJsonArray *arr = fbdo.to<FirebaseJsonArray *>();
    arr->setJsonArrayData(fbdo.fcm.idTokens.c_str());

    if (messageType == fb_esp_fcm_msg_type::msg_single && fbdo.fcm.idTokens.length() > 0 && fbdo.fcm._index > arr->size() - 1)
    {
        fbdo.session.http_code = FIREBASE_ERROR_FCM_ID_TOKEN_AT_INDEX_NOT_FOUND;
        return false;
    }

    s = fb_esp_pgm_str_576;

    json->get(data, s.c_str());

    if (messageType == fb_esp_fcm_msg_type::msg_topic && data.stringValue.length() == 0)
    {
        fbdo.session.http_code = FIREBASE_ERROR_NO_FCM_TOPIC_PROVIDED;
        return false;
    }

    json->clear();
    arr->clear();

    fbdo.fcm.fcm_begin(fbdo);

    return fbdo.fcm.fcm_send(fbdo, messageType);
}

bool FIREBASE_CLASS::sendMessage(FirebaseData &fbdo, uint16_t index)
{
    fbdo.fcm._index = index;
    return handleFCMRequest(fbdo, fb_esp_fcm_msg_type::msg_single);
}

bool FIREBASE_CLASS::broadcastMessage(FirebaseData &fbdo)
{
    return handleFCMRequest(fbdo, fb_esp_fcm_msg_type::msg_multicast);
}

bool FIREBASE_CLASS::sendTopic(FirebaseData &fbdo)
{
    return handleFCMRequest(fbdo, fb_esp_fcm_msg_type::msg_topic);
}

#endif

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

bool FIREBASE_CLASS::sdBegin(int8_t ss, int8_t sck, int8_t miso, int8_t mosi, uint32_t frequency)
{
    return mbfs->sdBegin(ss, sck, miso, mosi, frequency);
}

#if defined(ESP8266)
bool FIREBASE_CLASS::sdBegin(SDFSConfig *sdFSConfig)
{
    return mbfs->sdFatBegin(sdFSConfig);
}
#endif

#if defined(ESP32)

bool FIREBASE_CLASS::sdBegin(int8_t ss, SPIClass *spiConfig, uint32_t frequency)
{
    return mbfs->sdSPIBegin(ss, spiConfig, frequency);
}
#endif

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
bool FIREBASE_CLASS::sdBegin(SdSpiConfig *sdFatSPIConfig, int8_t ss, int8_t sck, int8_t miso, int8_t mosi)
{
    return mbfs->sdFatBegin(sdFatSPIConfig, ss, sck, miso, mosi);
}
#endif

#endif

#if defined(ESP8266) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD_MMC)

bool FIREBASE_CLASS::sdMMCBegin(const char *mountpoint, bool mode1bit, bool format_if_mount_failed)
{

    return mbfs->sdMMCBegin(mountpoint, mode1bit, format_if_mount_failed);
}

#endif

fb_esp_mem_storage_type FIREBASE_CLASS::getMemStorageType(uint8_t old_type)
{
    return (fb_esp_mem_storage_type)(old_type);
}

bool FIREBASE_CLASS::setSystemTime(time_t ts)
{
    return ut->setTimestamp(ts) == 0;
}

FIREBASE_CLASS Firebase = FIREBASE_CLASS();

#endif /*  FIREBASE_ESP32_CLIENT || FIREBASE_ESP8266_CLIENT  */

#endif /* ESP8266 || ESP32 */

#endif /* Firebase_CPP */