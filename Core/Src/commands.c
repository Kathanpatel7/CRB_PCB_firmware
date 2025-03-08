/*
 * commands.c
 *
 *  Created on: Nov 19, 2024
 *      Author: Kathanpatel07
 */
#include "main.h"
#include "can.h"
#include "timer.h"
#include "uart.h"
#include "functions.h"
#include "tests.h"
#include <string.h>
#include <stdlib.h>

void parseCommand(char *command) {
	// Variables to hold keys and values
	char key[20], value[20];
	int functionNumber = -1;

	// Pointer to traverse the string
	char *current = command;

	// Parse the command
	while (*current != '\0') {
		// Extract key
		char *keyStart = current;
		while (*current != ',' && *current != '\0') {
			current++;
		}
		if (*current == ',') {
			*current = '\0';  // Null-terminate the key
			current++;        // Move past ','
		}
		strncpy(key, keyStart, sizeof(key) - 1);
		key[sizeof(key) - 1] = '\0';  // Ensure null-termination

		// Extract value
		char *valueStart = current;
		while (*current != ';' && *current != '\0') {
			current++;
		}
		if (*current == ';') {
			*current = '\0';  // Null-terminate the value
			current++;        // Move past ';'
		}
		strncpy(value, valueStart, sizeof(value) - 1);
		value[sizeof(value) - 1] = '\0';  // Ensure null-termination

		// Process the key-value pair
		if (strcmp(key, "Funtion") == 0) {
			functionNumber = atoi(value);
		}

		// Switch processing based on `functionNumber`
		switch (functionNumber) {
		case 1:  // System initialization (homing of all motors)
			if (strcmp(key, "Sysinit") == 0) {
				if (atoi(value) == 1) {
					system_initialisation();  // Call system initialization
					sendUARTMessage("ok\n"); // Acknowledge successful completion
				}
			}
			break;

		case 2:  // Control Motor 5 for "door"
			if (strcmp(key, "Door") == 0) {
				if (atoi(value) == 1) {
					controlMotor5("open");  // Open the door
					sendUARTMessage("ok\n");
				} else if (atoi(value) == 0) {
					controlMotor5("close");  // Close the door
					sendUARTMessage("ok\n");
				} else {
					sendUARTMessage("Invalid door value\n");
				}
			}
			break;

		case 3:  // Rotate phone using "Revolve"
			if (strcmp(key, "Revolve") == 0) {
				int degree = atoi(value); // Convert the value to an integer (e.g., 0, 90, 180)
				rotatePhone(degree);     // Rotate phone by the specified degree
				sendUARTMessage("ok\n");  // Acknowledge successful rotation
			}
			break;
		case 4:  // Light brightness control
			if (strcmp(key, "MultipleBri") == 0) {
				// Debug: Show the received value
				sendUARTMessage("Parsing Light value: ");
				sendUARTMessage(value);
				sendUARTMessage("\n");

				// Split the values by `,`
				char *token = strtok(value, ",");
				uint8_t lightIndex = 0;

				while (token != NULL && lightIndex < 4) {
					// Convert value from string to float (percentage between 0.0 and 1.0)
					float brightnessFloat = atof(token);

					// Ensure the brightness is within the range 0 to 1
					if (brightnessFloat < 0)
						brightnessFloat = 0;
					if (brightnessFloat > 1)
						brightnessFloat = 1;

					// Convert to 0-100 range for SetLightBrightness
					int brightnessPercent = (int) (brightnessFloat * 100);

					// Debug: Show parsed brightness for the current light
					char debugMessage[50];
					snprintf(debugMessage, sizeof(debugMessage),
							"Light %d Brightness: %d%%\n", lightIndex + 1,
							brightnessPercent);  // +1 to match LED numbering
					sendUARTMessage(debugMessage);

					// Set brightness for the current light (lightIndex + 1 to match LED numbering)
					SetLightBrightness(lightIndex + 1, brightnessPercent);

					// Move to the next value
					token = strtok(NULL, ",");
					lightIndex++;
				}

				// Ensure all 4 lights have been processed
				if (lightIndex == 4) {
					sendUARTMessage("Light brightness set successfully\n");
				} else {
					// sendUARTMessage("Error: Not enough brightness values provided\n");
				}
			}
			break;

		case 7:
		    if (strcmp(key, "P1_X") == 0 || strcmp(key, "P1_Y") == 0 ||
		        strcmp(key, "P2_X") == 0 || strcmp(key, "P2_Y") == 0 || strcmp(key, "Speed") == 0) {
		        static int px1 = -1, py1 = -1, px2 = -1, py2 = -1, speed = -1;

		        if (strcmp(key, "P1_X") == 0) px1 = atoi(value);
		        else if (strcmp(key, "P1_Y") == 0) py1 = atoi(value);
		        else if (strcmp(key, "P2_X") == 0) px2 = atoi(value);
		        else if (strcmp(key, "P2_Y") == 0) py2 = atoi(value);
		        else if (strcmp(key, "Speed") == 0) speed = atoi(value);

		        if (px1 >= 0 && py1 >= 0 && px2 >= 0 && py2 >= 0 && speed >= 0) {
		            multitouch_screen_test(px1, py1, px2, py2, speed);
		            sendUARTMessage("Multi-touch screen test ok\n");

		            px1 = py1 = px2 = py2 = speed = -1;
		        }
		    }
		    break;


		case 8:
		    if (strcmp(key, "P1_X") == 0 || strcmp(key, "P1_Y") == 0 ||
		        strcmp(key, "P2_X") == 0 || strcmp(key, "P2_Y") == 0 ||
		        strcmp(key, "Swipe") == 0 || strcmp(key, "Speed") == 0) {
		        static int px1 = -1, py1 = -1, px2 = -1, py2 = -1, numSteps = -1, speed = -1;

		        if (strcmp(key, "P1_X") == 0) px1 = atoi(value);
		        else if (strcmp(key, "P1_Y") == 0) py1 = atoi(value);
		        else if (strcmp(key, "P2_X") == 0) px2 = atoi(value);
		        else if (strcmp(key, "P2_Y") == 0) py2 = atoi(value);
		        else if (strcmp(key, "Swipe") == 0) numSteps = atoi(value);
		        else if (strcmp(key, "Speed") == 0) speed = atoi(value);

		        if (px1 >= 0 && py1 >= 0 && px2 >= 0 && py2 >= 0 && numSteps >= 0 && speed >= 0) {
		            screen_test(px1, py1, px2, py2, numSteps, speed);
		            sendUARTMessage("Screen test with swipe ok\n");

		            px1 = py1 = px2 = py2 = numSteps = speed = -1;
		        }
		    }
		    break;

		case 13:
		    if (strcmp(key, "P1_X") == 0 || strcmp(key, "P1_Y") == 0 ||
		        strcmp(key, "P2_X") == 0 || strcmp(key, "P2_Y") == 0 || strcmp(key, "Speed") == 0) {
		        static int px1 = -1, py1 = -1, px2 = -1, py2 = -1, speed = -1;

		        if (strcmp(key, "P1_X") == 0) px1 = atoi(value);
		        else if (strcmp(key, "P1_Y") == 0) py1 = atoi(value);
		        else if (strcmp(key, "P2_X") == 0) px2 = atoi(value);
		        else if (strcmp(key, "P2_Y") == 0) py2 = atoi(value);
		        else if (strcmp(key, "Speed") == 0) speed = atoi(value);

		        if (px1 >= 0 && py1 >= 0 && px2 >= 0 && py2 >= 0 && speed >= 0) {
		            speaker_test(px1, py1, px2, py2, speed);
		            sendUARTMessage("Microphone test ok\n");

		            px1 = py1 = px2 = py2 = speed = -1;
		        }
		    }
		    break;


		case 15:
		    if (strcmp(key, "TOP_Chart") == 0 || strcmp(key, "Speed") == 0) {
		        static int top_chart = -1, speed = -1;

		        if (strcmp(key, "TOP_Chart") == 0) top_chart = atoi(value);
		        else if (strcmp(key, "Speed") == 0) speed = atoi(value);

		        if (top_chart >= 0 && speed >= 0) {
		            camera_test(top_chart, speed);
		            sendUARTMessage("Camera test ok\n");

		            top_chart = speed = -1;
		        }
		    }
		    break;


		case 6:  // System initialization (homing of all motors)
			if (strcmp(key, "XYZ_return") == 0) {
				if (atoi(value) == 1) {
					Return_xy();  // Call system initialization
					sendUARTMessage("Home ok\n"); // Acknowledge successful completion
				}
			}
			break;

		default:
			if (functionNumber != -1) {
				sendUARTMessage("Unsupported function or key\n");
			}
			break;
		}
	}
}
