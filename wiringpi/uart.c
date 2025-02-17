#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	int serialPort;
	char receivedChar;

	// Initialize wiringPi
	wiringPiSetup();

	// Open the serial port (ttyAMA0 or ttyS0 depending on your setup)
	serialPort = serialOpen("/dev/ttyS0", 9600);  // You can also try "/dev/ttyAMA0" or "/dev/ttyS0"

	if (serialPort < 0) {
		printf("Unable to open serial port\n");
		return -1;
	}

	printf("Serial Port Opened\n");

	// Loopback Test: Send data and immediately check if it's received
	while (1) {
		// Send a character (for testing, we will send 'A')
		serialPuts(serialPort, "A");

		// Wait for the data to be received
		usleep(100000);  // Sleep for 100ms to allow data to be transmitted and received

		// Check if any data is available
		if (serialDataAvail(serialPort)) {
			// Read the received character
			receivedChar = serialGetchar(serialPort);
			printf("Received: %c\n", receivedChar);
		} else {
			printf("No data received\n");
		}

		usleep(500000);  // Sleep for 500ms before sending the next character
	}

	return 0;
}
