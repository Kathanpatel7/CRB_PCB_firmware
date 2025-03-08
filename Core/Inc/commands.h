/*
 * commands.h
 *
 *  Created on: Nov 19, 2024
 *      Author: owlnuc13
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

extern UART_HandleTypeDef huart2;  // Declare it as an external variable


void parseCommand(char *cmd) ;

#endif /* INC_COMMANDS_H_ */
