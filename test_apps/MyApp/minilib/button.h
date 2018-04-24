/*
 * led.h
 * 
 * LED declarations
 *
 */ 

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint-gcc.h>	

typedef uint32_t tButtonNum;	
typedef uint32_t tButtonState;	

enum tButtonNum	{ Button0 = 0, Button1, Button2, Button3, Button4 };
enum tButtonState { ButtonUnpressed = false, ButtonPressed };	

//void hal_button_startall_ev( tButtonNum ); //Not supported
void button_startall_poll( void ); //Not supported
tButtonState button_read( tButtonNum );

#endif /* BUTTON_H_ */