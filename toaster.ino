/*
 * This is a simple telegram bot
 * it will recieve two commands: "toast" and "untoast"
 * toast   : turns servo on oven toaster to 180 degrees (on oven toaster)
 * untoast : turns servo on oven toaster to 0 degrees (off oven toaster)
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>

// Initialize Wifi connection to the router
const char ssid[] = "Singtel7002-FBEF";              // your network SSID (name)
const char password[] = "0013499375";                // your network key

// Initialize Telegram BOT
#define BOTtoken "YOUR_BOT_TOKEN"  //token of TestBOT
#define BOTname "YOUR_BOT_NAME"
#define YOUR_BOT_USERNAME "YOUR_BOT_USERNAME"
TelegramBOT bot(BOTtoken, BOTname, YOUR_BOT_USERNAME);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/
 
void Bot_ReplyMessages() {
    Serial.println(">>>telegram>>>");
    for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
        String chatID = bot.message[i][4];
        String message = bot.message[i][5];

        String reply = "you said: " + message;
        bot.sendMessage(bot.message[i][4], reply, "");
        //chat_id message
    }
    Serial.println("<<<telegram<<<");
    bot.message[0][0] = "";   // All messages have been replied - reset new messages
}


void setup() {
    pinMode(D7, OUTPUT);
    Serial.begin(115200);
    delay(3000);
    
    // attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
    
    bot.begin();      // launch Bot functionalities
}

void loop() {
    if (millis() > Bot_lasttime + Bot_mtbs)  {
        digitalWrite(D7, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);              // wait for a second
        digitalWrite(D7, LOW);    // turn the LED off by making the voltage LOW
        
        bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
        Bot_ReplyMessages();
        //Bot_EchoMessages();   // reply to message with Echo
        Bot_lasttime = millis();
    }
}

