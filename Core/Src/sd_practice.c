/*
 * sd_practice.c
 *
 *  Created on: Apr 12, 2025
 *      Author: Daniel L.Estevez
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f4xx_hal.h"
#include "ff.h"

FATFS fat_fs_handle; //Fatfs handle

void InitSD(UART_HandleTypeDef *uart) {
	HAL_Delay(1000); //a short delay is important to let the SD card settle

	SendViaUART("Starting micro SD !!!\n\r", uart);
	FRESULT result; //Result after operations

	//Open the file system
	result = f_mount(&fat_fs_handle, "", 1); // 1=mount now

	if (result != FR_OK) {

		if (result == FR_NOT_READY) {
			SendViaUART("MicroSD not inserted !!!\n\r", uart);
			return;
		}

		SendViaUART("f_mount error", uart);

		while (1);
	}

	SendViaUART("MicroSD mounted !!!\n\r", uart);
}

/**
 * WARNING: this is causing stack overflow
 */
void AnalyzeSD(UART_HandleTypeDef *uart) {

	if (CheckMount(uart))
		return;

	SendViaUART("Starting micro SD stats analyzer !!! \n\r", uart);
	//Let's get some statistics from the SD card
	DWORD free_clusters, free_sectors, total_sectors;
	FRESULT result; //Result after operations
	FATFS *getFreeFs;
	char label[24];

	// Begin space information
	result = f_getfree("", &free_clusters, &getFreeFs);

	if (result != FR_OK) {
		SendViaUART("f_getfree error", uart);
		while (1)
			;
	}

	result = f_getlabel("", label, 0);

	if (result != FR_OK) {
		SendViaUART("f_getlabel error", uart);
		while (1)
			;
	}

	char label_buffer[32];
	sprintf(label_buffer, "*SD stats (volume label): \n\r %s\n\r", label);
	SendViaUART(label_buffer, uart);

	//Formula comes from ChaN's documentation
	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;

	// Get KiB then convert to KB
	const float total_space_kb = ((total_sectors / 2) * 1.024);

	// Get KiB then convert to KB
	const float available_space_kb = ((free_sectors / 2) * 1.024);

	char total_space_buffer[64];
	sprintf(total_space_buffer,
			"*SD stats (total space):\n\r %.2f KB (%.2f MB)\n\r",
			total_space_kb, total_space_kb / 1000);

	SendViaUART(total_space_buffer, uart);

	char available_space_buffer[64];
	sprintf(available_space_buffer,
			"*SD stats (available space):\n\r %.2f KiB (%.2f MB)\n\r",
			available_space_kb, available_space_kb / 1000);

	SendViaUART(available_space_buffer, uart);

	HAL_Delay(1000);

	// Clear buffer
	memset(label_buffer, 0, sizeof(label_buffer));
	memset(total_space_buffer, 0, sizeof(total_space_buffer));
	memset(available_space_buffer, 0, sizeof(available_space_buffer));
	// End space information

	SendViaUART("Micro SD stats analyzer done !!! \n\r", uart);
}

void CreateFile(UART_HandleTypeDef *uart) {

	if (CheckMount(uart))
		return;

	SendViaUART("Creating a new file...\n\r", uart);

	FRESULT result; // Operation result
	FIL fil; 		//File handle

	//Creating file
	result = f_open(&fil, "test.txt",
	FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS);

	HAL_Delay(1000);

	if (result != FR_OK) {
		SendViaUART("f_open error");
		while (1)
			;
	}

	SendViaUART("File open or created...\n\r", uart);

	HAL_Delay(1000);

	SendViaUART("Writing file...\n\r", uart);

	UINT bytesWrote;
	char content[] = "A new content";

	result = f_write(&fil, content, strlen(content), &bytesWrote);

	if (result == FR_OK) {
		SendViaUART("Content was added !!!\n\r", uart);
	} else {
		SendViaUART("f_write error", uart);
		while (1)
			;
	}

	//don't forget to close your file!
	f_close(&fil);

	//We're done, so de-mount the drive
	f_mount(NULL, "", 0);
}

int CheckMount(UART_HandleTypeDef *uart) {
	if (!fat_fs_handle.id) {
		SendViaUART("Fail: SD is unmount \n\r", uart);
		return 1;
	}

	return 0;
}

void SendViaUART(const char text[], UART_HandleTypeDef *uart) {
	HAL_UART_Transmit(uart, (uint8_t*) text, strlen(text), 10);
}
