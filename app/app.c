/*
 * app.c
 *
 *  Created on: Jul 1, 2022
 *      Author: ciles
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "app.h"

void app_init(void)
{

}
void app_loop(void)
{
//	ac_states_t estado = AC_INIT;	//Estado
	float setpoint = 25;
	float histerese = 2;
	static float temperatura = 25;
	static uint32_t contagem = 0;
	uint32_t piscada_ms;
	uint32_t tempo_apos_pressionar_ms = 250;
	static bool led_ligado = true;

	GPIO_PinState Subiu = !HAL_GPIO_ReadPin(SOBE_GPIO_Port, SOBE_Pin);
	GPIO_PinState Desceu = !HAL_GPIO_ReadPin(DESCE_GPIO_Port, DESCE_Pin);


	if(Subiu){
		temperatura += 0.5;
		HAL_Delay(tempo_apos_pressionar_ms);
	}

	if(Desceu){
		temperatura -= 0.5;
		HAL_Delay(tempo_apos_pressionar_ms);
	}

	if(temperatura <= setpoint - histerese){
		piscada_ms = 100;
//		estado = AC_COOLER;
	} else if(temperatura > setpoint - histerese && temperatura < setpoint + histerese){
		piscada_ms = 500;
//		estado = AC_IDLE;
	} else if(temperatura >= setpoint + histerese){
		piscada_ms = 1000;
//		estado = AC_HEATER;
	} else{ // Não deveria chegar aqui
//		estado = AC_INIT;
	}

	if(contagem >= piscada_ms){
		if(led_ligado){
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
			led_ligado = false;
		}else{
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
			led_ligado = true;
		}
		contagem = 0;
	}
	HAL_Delay(1);
	contagem++;
}
