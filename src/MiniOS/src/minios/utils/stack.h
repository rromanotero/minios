/**
 * @file	stack.h
 * @author  
 * @version 
 *
 * @brief Header file for the stack.c
 *
 */

#ifndef STACK_H_
#define STACK_H_

void stack_init(uint32_t);
void stack_alloc(uint32_t);
uint32_t* stack_top(void);

#endif /* STACK_H_ */