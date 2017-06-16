/*
 * This is a simple telegram bot
 * it will recieve two commands: "toast" and "untoast"
 * toast   : turns servo on oven toaster to 180 degrees (on oven toaster)
 * untoast : turns servo on oven toaster to 0 degrees (off oven toaster)
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>
#include <Servo.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

const int toastDegree = 0;
const int untoastDegree = 90;

WiFiManager wifiManager;
Servo myservo;

// Initialize Telegram BOT
#define BOTtoken "YOUR_BOT_TOKEN"  //token of TestBOT
#define BOTname "YOUR_BOT_NAME"
#define YOUR_BOT_USERNAME "YOUR_BOT_USERNAME"
TelegramBOT bot(BOTtoken, BOTname, YOUR_BOT_USERNAME);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
int degree = toastDegree;

void setup() {
    //pinMode(D1, OUTPUT);
    Serial.begin(57600);
    delay(3000);

    myservo.attach(D3, 900, 2100);
    
    // attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    wifiManager.autoConnect();
    
    /*
    // Wifi.status() is never WL_CONNECTED for some reason
    // even after it is connected to Wifi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    */

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
    
    bot.begin();      // launch Bot functionalities
}
 
void Bot_ReplyMessages() {
    Serial.println(">>>telegram>>>");

    // loop through all the messages recieved from user
    for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
        String chatID = bot.message[i][4];
        String message = bot.message[i][5];
        String reply;

        if (message == "/toast") {
            degree = toastDegree;
            reply = "toasting!";
            //turn();
            Serial.println("TOAST");
            
        } else if (message == "/untoast") {
            degree = untoastDegree;
            reply = "stopped toasting!";
            //turn();
            Serial.println("UNTOAST");
            
        } else {
            reply = "command unknown, try /toast or /untoast";
            Serial.println("UNKNOWN");
        }

        bot.sendMessage(bot.message[i][4], reply, "");
        //chat_id message
    }
    
    Serial.println("<<<telegram<<<");
    bot.message[0][0] = "";   // All messages have been replied - reset new messages
}

void loop() {
    if (millis() > Bot_lasttime + Bot_mtbs) {
        bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
        Bot_ReplyMessages();
        //Bot_EchoMessages();                // reply to message with Echo
        Bot_lasttime = millis();
        Serial.println(degree);
    }

    myservo.write(degree);
    delay(30);
}




