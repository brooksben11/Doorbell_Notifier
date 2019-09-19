#define FRONT_DOOR A0
#define BACK_DOOR A1
String FD_Last_Ring = "Unknown";
String BD_Last_Ring = "Unknown";
bool FD_Reading;
bool BD_Reading;
int FD_Counter = 0;
int BD_Counter = 0;
int FD_Max = 0;
int BD_Max = 0;
SYSTEM_THREAD(ENABLED);

void setup() {
    pinMode(FRONT_DOOR, INPUT);
    pinMode(BACK_DOOR, INPUT);
    Particle.variable("FD_Last_Ring", FD_Last_Ring);
    Particle.variable("BD_Last_Ring", BD_Last_Ring);
    Particle.variable("FD_Max", FD_Max);
    Particle.variable("BD_Max", BD_Max);
    Time.zone(-5);
}

void loop() {
    FD_Reading = digitalRead(FRONT_DOOR);
    BD_Reading = digitalRead(BACK_DOOR);
    
    //If statements to increment the counter and determine if a new max value has been set
    if (FD_Reading == false) {
        FD_Counter += 1;
    }
    else {
        FD_Counter = 0;
    }
    if (FD_Counter > FD_Max) {
        FD_Max = FD_Counter;
    }
    if (BD_Reading == false) {
        BD_Counter += 1;
    }
    else {
        BD_Counter = 0;
    }
    if (BD_Counter > BD_Max) {
        BD_Max = BD_Counter;
    }
    
    /*
    Running on Xenon with SYSTEM_THREAD(ENABLED) it normally peaks at ~173 from AC voltage, seen it peak to ~300
    Very fast doorbell push as low as ~433
    Disable these if statements if you need to calibrate your setup 
    */
    if (FD_Counter > 400) {
        Particle.publish("Front_Door", "Ring", 60, PRIVATE);
        FD_Last_Ring = Time.timeStr();
        delay(3000); //A delay to allow for the doorbell to stop being pushed, to avoid multiple publishes for a single doorbell push
        FD_Counter = 0;
        FD_Max = 0;
    }
    if (BD_Counter > 400) {
        Particle.publish("Back_Door", "Ring", 60, PRIVATE);
        BD_Last_Ring = Time.timeStr();
        delay(3000); //A delay to allow for the doorbell to stop being pushed, to avoid multiple publishes for a single doorbell push
        BD_Counter = 0;
        BD_Max = 0;
    }
}
