# esp32_MQTT_Car
4 WD Bot that can be controlled by WiFi. The bot responds to commands over MQTT protocol. The 4 wheel can be driven individually. 
The bot has a obstacle sensing mode that uses proximity sensors on the front and on the back to detect obstacles and reverse its direction.


Upcoming features:
Magnetic charging dock
Onboarding measuring of battery voltage
Machine Learning mode - the bot predicts obstacles if it had encountered it before,even before the proximity obstacle detector senses it
Line Following mode - can follow predefined paths using proximity sensors(uses insfrared to detect black/white patterens).
Returns to charging docks by following line, when the battery  becomes low.

