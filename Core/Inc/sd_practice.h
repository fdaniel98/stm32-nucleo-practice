/*
 * sd_practice.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Daniel L.Estevez
 */

#ifndef SD_PRACTICE_H
#define SD_PRACTICE_H

void InitSD(UART_HandleTypeDef *uart);
void AnalyzeSD(UART_HandleTypeDef *uart);
void CreateFile(UART_HandleTypeDef *uart);
int CheckMount(UART_HandleTypeDef *uart);
void SendViaUART(const char text[], UART_HandleTypeDef *uart);

#endif /* INC_SD_PRACTICE_H_ */
