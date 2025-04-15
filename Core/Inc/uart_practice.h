/*
 * uart_practice.h
 *
 *  Created on: Apr 11, 2025
 *      Author: Daniel L.Estevez
 */
#ifndef __UART_PRACTICE_H
#define __UART_PRACTICE_H

#ifdef __cplusplus
 extern "C" {
#endif

#define TIMEOUT 100
#define RX_SIZE 1
#define NEW_LINE "\n\r"

#include "stm32f4xx_hal.h"

void SendHelloWorldViaUART(UART_HandleTypeDef *uart);
void SendTextViaUART(const char text[], UART_HandleTypeDef *uart);
void UARTCommunication(UART_HandleTypeDef *uart);
void SendReceivedDataViaUART(UART_HandleTypeDef *uart);
void HandleLEDViaUART(UART_HandleTypeDef *uart);
void WaitForRXCommunication(UART_HandleTypeDef *uart);
void WaitForSIMCommunication(UART_HandleTypeDef *sim_uart);
void JumpLineViaUART(UART_HandleTypeDef *uart);
void SIM800UART(UART_HandleTypeDef *uart, UART_HandleTypeDef *sim_800_uart);
void TurnONInternalLED();
void TurnOFFInternalLED();
void ClearBuffer();

#endif /* INC_UART_PRACTICE_H_ */
