// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>

byte server[] = { 192, 168, 2, 84 }; //192.168.2.84

const char* DeviceID = System.deviceID();
const char* Username = "xxxxxxxxxxxxx"; //Home Assistant generates this when you setup MQTT
const char* Password = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //Home Assistant generates this when you setup MQTT

MQTT client(server,
  1883 /* port */,
  600 /* keep alive in seconds (10 mins) */,
  callback,
  512 /* max packet size */);


// Used when a message is recieved; not using in this project, so blank
void callback(char* topic, byte* payload, unsigned int length) 
{
}

const char* discovery_payload_1 = "{\"name\":\"front_doorbell\",\"unique_id\":\"front_doorbell\",\"automation_type\":\"trigger\",\"type\":\"button_short_press\",\"subtype\":\"button_1\",\"payload\":\"button_1\",\"topic\":\"doorbell/action\",\"command_topic\":\"~/control\",\"device\":{\"identifiers\":\"doorbell\",\"model\":\"Argon\",\"manufacturer\":\"Particle\"}}";//const char* discovery_payload_1 = "{\"name\":\"test_button_1\",\"unique_id\":\"test_button_1\",\"automation_type\":\"trigger\",\"type\":\"button_short_press\",\"subtype\":\"button_1\",\"payload\":\"button_1\",\"topic\":\"test_platform/action\",\"command_topic\":\"~/control\",\"device\":{\"identifiers\":\"test_platform\",\"model\":\"Photon\",\"manufacturer\":\"Particle\"}}";


const char* discovery_payload_2 = "{\"name\":\"back_doorbell\",\"unique_id\":\"back_doorbell\",\"automation_type\":\"trigger\",\"type\":\"button_short_press\",\"subtype\":\"button_2\",\"payload\":\"button_2\",\"topic\":\"doorbell/action\",\"command_topic\":\"~/control\",\"device\":{\"identifiers\":\"doorbell\",\"model\":\"Argon\",\"manufacturer\":\"Particle\"}}";

bool Front_Door_Flag = false;
bool Back_Door_Flag = false;



String Status = "Startup";


void setup() {
    
    setupMQTT();
    
    
    //Manually select the external mesh antenna
    digitalWrite(ANTSW1, 1);
    digitalWrite(ANTSW2, 0);
    
    Mesh.subscribe("Front_Door", Front_Door);
    Mesh.subscribe("Back_Door", Back_Door);
    
    Status = "Running";
	Particle.variable("Status", Status);
}

void loop() {
    
    if (client.isConnected())
    {
        if (Front_Door_Flag == true /*and client.isConnected()*/) {
            client.publish("doorbell/action", "button_1");
            Front_Door_Flag = false;
        }
        if (Back_Door_Flag == true /*and client.isConnected()*/) {
            client.publish("doorbell/action", "button_2");
            Back_Door_Flag = false;
        }
        
        delay(1000);
        // CALL THIS at the end of your loop
        client.loop();
    }
    
     else {
        delay(100000); //changed this from 1000
        
        if (!Particle.connected()) {
            Particle.connect();
        }
        
        if (setupMQTT()) {
        }
    }
}

void Front_Door(const char *event, const char *data)
{
    Front_Door_Flag = true;
}

void Back_Door(const char *event, const char *data)
{
    Back_Door_Flag = true;
}

bool setupMQTT() {
    
    client.connect(DeviceID, Username, Password);

    if (client.isConnected()) {

        client.publish("homeassistant/button/doorbell/action_button_1/config", discovery_payload_1);
        client.publish("homeassistant/button/doorbell/action_button_2/config", discovery_payload_2);

        return true;
    } else {
        return false;
    }
}