/* SmartCityNode
   ahdavidson, 05/2016

   This is a simple demo of a program that can use an XBee to receive messages
   send by a SmartCity gateway. See accompanying docs for details on how to connect the XBee.

   Then use the SmartCityNodeLib library to simplify your code to receive the messages.
   The basic idea is that you need to ask the node object each time through your main loop
   (via the alertReceived() method) if there is an incoming alert message for your zone.
   If there is, then you can process it; otherwise just go on and do wjatever else you
   need to do (checking your sensors, etc.) in your main loop. If there is an alert, you
   can decide what action your node should take based on that information.

   Note that you will only receive alerts that are targeted for the zone that you declare
   your node to be a part of when you instantiate the node object.

*/

// you must incldue these 2 libraries for the SmartCity and XBee message stuff
#include <SoftwareSerial.h>
#include <SmartCityNodeLib.h>

// use a button in the program
#include <Button.h>

// some constants to identify my node to the library object
const String myNodeName = "Andy's Office";
const int    myZone = SmartCityNode::RESIDENTIAL;  // declare what zone you are

// because every Arduino program has to blink an LED
const int ledPin = LED_BUILTIN;  // pin 13, no doubt
const int btnPin = 7;

// this connects your program to your XBee module
// you must use pin 2 for RX and pin 3 for TX
// the XBee must be set to 9600 baud
SmartCityNode myNode;

// set up a button to use for input
Button button(btnPin);

// state variables
boolean myDebugging = true;


void setup() {

  // just for our own debugging
  Serial.begin(9600);
  Serial.println("\n\n*** Starting SmartCityNode demo");

  // you want to blink that LED, right?
  pinMode(ledPin, OUTPUT);

  // just flash it for a second at startup so we know things have initialized
  digitalWrite(ledPin, HIGH);
  delay(1000); 
  digitalWrite(ledPin, LOW);

  // comment this line out to suppress debugging messages from the library
  myNode.setDebugOn();

  // this starts up your XBee and does other SmartCity initialization stuff.
  // the second argument is just to identify your node by a name.
  // you must declare what zone your node is in so that you only receive alerts
  // targeted for that zone. to do that, use one of these codes for the first
  // argument:
  //       SmartCityNode::RESIDENTIAL
  //       SmartCityNode::BUSINESS
  //       SmartCityNode::INDUSTRIAL

  myNode.begin(myZone, myNodeName);

  pinMode(2, OUTPUT);

}


void loop() {

  // check for an incoming alert
  // alertReceived() returns 0 (zero) if none have been sent to your zone.
  // it returns a code as follows if there is one:
  //       SmartCityNode::FIRE
  //       SmartCityNode::GAS
  //       SmartCityNode::FLOOD
  //       SmartCityNode::BURGLARY
  //       SmartCityNode::EARTHQUAKE
  //       SmartCityNode::ZOMBIE
  //       SmartCityNode::CONTAINMENT_BREACH
  //       SmartCityNode::HIPSTER_INVASION
  //       SmartCityNode::UNWANTED_SOLICITORS
  //       SmartCityNode::BLACK_FRIDAY
  //       SmartCityNode::CUTE_ANIMALS
  //       SmartCityNode::APOCALYPSE

  int alert = myNode.alertReceived();
  if (alert != 0) {
    // in this demo, we'll just print out a message about it
    logAlert(myNodeName, myZone, alert);
    // here is where you would do the real work of processing this alert:
    if (alert == SmartCityNode::FIRE) {
      digitalWrite(2, HIGH);
      delay(250);
      digitalWrite(2, LOW);
     }
  }

  // do other stuff for your node now
  // in this example, I'll just use the button to toggle the SmartCity debugging on and off
  if (button.checkButtonAction() == Button::PRESSED) {
    myDebugging = !myDebugging;
    Serial.print("+++ Setting debugging ");
    Serial.println(myDebugging ? "ON" : "OFF");
    myNode.setDebug(myDebugging);
  }

}


void logAlert (String myName, int zone, int alert) {

  // just blink the led and write a message out to the serial monitor

  digitalWrite(ledPin, HIGH);

  Serial.print("*** alert received at ");
  float sec = millis() / 1000.0;
  Serial.println(sec);
  Serial.print("***   node = ^");
  Serial.print(myName);
  Serial.print("^ in zone # ");
  Serial.print(zone);
  Serial.print(": ");
  Serial.println(myNode.zoneName(zone));
  Serial.print("***   alert = ");
  Serial.print(alert);
  Serial.print(": ");
  Serial.println(myNode.alertName(alert));

  // end of the blink
  digitalWrite(ledPin, LOW);

}
