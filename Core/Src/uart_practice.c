/*
 * uart_practice.c
 *
 *  Created on: Apr 11, 2025
 *      Author: Daniel L.Estevez
 */

#include "uart_practice.h"
#include "string.h"

uint8_t tx_buffer[30] = "Welcome to Nucleo practice!";

uint8_t rx_index = 0;
uint8_t rx_buffer[100];
uint8_t rx_data[RX_SIZE];

uint8_t sim_buffer[100];
uint8_t sim_data[100];

void SendHelloWorldViaUART(UART_HandleTypeDef *uart) {
	char msg[] = "Hello World:\n\r";
	SendTextViaUART(msg, uart);
}

void SendTextViaUART(const char text[], UART_HandleTypeDef *uart) {
	HAL_UART_Transmit(uart, (const uint8_t*) text, strlen(text), TIMEOUT);
}

void TXUARTCommunication(UART_HandleTypeDef *uart) {
	TurnONInternalLED();
	HAL_UART_Transmit(uart, tx_buffer, 30, TIMEOUT);
	JumpLineViaUART(uart);
	HAL_Delay(1000);
}

void RXUARTCommunication(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // LED ON
}

void WaitForRXCommunication(UART_HandleTypeDef *uart) {
	SendTextViaUART("Awaiting for data...", uart);
	JumpLineViaUART(uart);
	/**
	 * This enable a next RX communication to receive the data
	 * rx_data is store variable and will store ONLY declared size Ex. uint8_t rx_data[10] will ONLY store 10 char length text
	 * RX_SIZE is the waiting data size Ex. RX_SIZE = 1 will only take 1 data length
	 */
	HAL_UART_Receive_IT(uart, rx_data, RX_SIZE);
}

void WaitForSIMCommunication(UART_HandleTypeDef *sim_uart) {
	SendTextViaUART("Awaiting for SIM800L data...", sim_uart);
	JumpLineViaUART(sim_uart);

	HAL_UART_Receive_IT(sim_uart, sim_data, 100);
}

void JumpLineViaUART(UART_HandleTypeDef *uart) {
	HAL_UART_Transmit(uart, (const uint8_t*) NEW_LINE, strlen(NEW_LINE), 10);
}

void SendReceivedDataViaUART(UART_HandleTypeDef *uart) {
	HAL_UART_Receive_IT(uart, rx_data, RX_SIZE);

	HAL_UART_Transmit(uart, rx_data, RX_SIZE, 10);
	JumpLineViaUART(uart);

	WaitForRXCommunication(uart);

	rx_data[RX_SIZE] = 0;
}

void HandleLEDViaUART(UART_HandleTypeDef *uart) {
	HAL_UART_Receive_IT(uart, rx_data, RX_SIZE);

	if (sizeof(rx_data) > 0) {
		const char *char_buffer = (const char*) rx_buffer;

		// Hint: On ENTER pressed
		if (rx_data[0] == 13) {
			if (strcmp(char_buffer, "LED ON") == 0) {
				ClearBuffer();
				TurnONInternalLED();
			} else if (strcmp(char_buffer, "LED OFF") == 0) {
				ClearBuffer();
				TurnOFFInternalLED();
			}

			JumpLineViaUART(uart);
		}
		// Hint: Other key pressed
		else {
			rx_buffer[rx_index] = rx_data[0];
			SendTextViaUART((const char*) rx_data, uart);
			rx_index += 1;
		}
	}
}

void SIM800UART(UART_HandleTypeDef *main_uart, UART_HandleTypeDef *sim_800_uart) {
	HAL_UART_Receive_IT(main_uart, rx_data, RX_SIZE);
	HAL_UART_Receive_IT(sim_800_uart, sim_data, 100);

	if (sizeof(rx_data) > 0) {
		const char *char_buffer = (const char*) rx_buffer;

		// Hint: On ENTER pressed
		if (rx_data[0] == 13) {
			if (strcmp(char_buffer, "AT") == 0) {
				ClearBuffer();

				JumpLineViaUART(main_uart);
				SendTextViaUART("You send AT command", main_uart);
				HAL_UART_Transmit(sim_800_uart, char_buffer,
						strlen(char_buffer), 10);
			}

			JumpLineViaUART(main_uart);
		}
		// Hint: Other key pressed
		else {
			rx_buffer[rx_index] = rx_data[0];
			SendTextViaUART((const char*) rx_data, main_uart);
			rx_index += 1;
		}
	}

//	if (sizeof(sim_data) > 0) {
//		SendTextViaUART("Received data from SIM800L:", main_uart);
//		JumpLineViaUART(main_uart);
//		SendTextViaUART((const char*) sim_data, main_uart);
//		JumpLineViaUART(main_uart);
//	}
}

void TurnONInternalLED() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // LED ON
}

void TurnOFFInternalLED() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // LED OFF
}

void ClearBuffer() {
	rx_index = 0;
	memset(rx_buffer, 0, sizeof(rx_buffer));
}

