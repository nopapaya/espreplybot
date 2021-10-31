#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <unordered_set>
#include <cstring>
#include <sstream>
using namespace std;

// Wifi network station credentials
#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<passwd>"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "<token>"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

//過濾（最多4個）
//std::unordered_set<std::string> filter_word = {"瓜", "幹", "papaya", "@"};

void handleNewMessages(int numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
        String message = bot.messages[i].text;
        char buf[100];
        message.toCharArray(buf, 100);
        string ms1;
        ms1 += buf;

        string papaya("木瓜");
        string at("@");

        std::size_t found1 = ms1.find(papaya);
        std::size_t found2 = ms1.find(at);
        if (found1 != std::string::npos) {
            bot.sendMessage(bot.messages[i].chat_id, "您好，您剛才發表的詞彙已經觸犯劉醬女裝方針，請立刻迴轉脫離！", "");
        } else if (found2 != std::string::npos) {
            continue;
        } else {
            bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
        }
        
        //int len1 = message.length();
        //char ms1[len1 + 1];
        //strcpy(ms1, message.c_str());
        //----
        
        //int len = strlen(message)
        //std::string papaya("木瓜");
        //std::string at("@");

        //std::size_t found1 = message.find(papaya);
        //std::size_t found2 = message.find(at);
        /*
        if (strcmp(message, "木瓜")) {
            bot.sendMessage(bot.messages[i].chat_id, "您好，您剛才發表的詞彙已經觸犯劉醬女裝方針，請立刻迴轉脫離！", "");
        //} else if (found2 != std::string::npos) {
            //continue;
        } else {
            bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
        }
        */
    }
}


void setup() {
    Serial.begin(115200);
    Serial.println();

    // attempt to connect to Wifi network:
    Serial.print("Connecting to Wifi SSID ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\nWiFi connected. IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Retrieving time: ");
    configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
    time_t now = time(nullptr);
    while (now < 24 * 3600) {
        Serial.print(".");
        delay(100);
        now = time(nullptr);
    }
    Serial.println(now);
}

void loop() {
    if (millis() - bot_lasttime > BOT_MTBS) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
    bot_lasttime = millis();
    }
}
