/*
 * FLASH_SECTOR.h
 *
 *  Created on: May 15, 2020
 *      Author: controllerstech
 */
#include "main.h"
#ifndef INC_FLASH_SECTOR_H_
#define INC_FLASH_SECTOR_H_

uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t * DATA_32);
void Flash_Read_Data (uint32_t StartPageAddress, __IO uint32_t * DATA_32);
void Convert_To_Str (uint32_t *data, char *str);


#endif /* INC_FLASH_SECTOR_H_ */
