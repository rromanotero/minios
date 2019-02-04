/**
 * @file	hal_io.c
 * @author  
 * @version 
 *
 * @brief IO part of the Hardware Abstraction Layer 
 *
 */

#include <asf.h>
#include <stdint-gcc.h>	//defs for size-specific primitive data types
#include <stdbool.h>	//defs for true and false
#include "hal.h"

//Button definitions
#define BUTTON0_PIN IOPORT_CREATE_PIN( PIOA, 2 )
#define BUTTON1_PIN IOPORT_CREATE_PIN( PIOA, 0 )
#define BUTTON2_PIN IOPORT_CREATE_PIN( PIOC, 29 )
#define BUTTON3_PIN IOPORT_CREATE_PIN( PIOC, 30 )

static const int buttons_irq_priority = 5;
static void (*button_callback)(uint32_t);
static void button_handler(uint32_t, uint32_t);

//Display definitions
static uint32_t display_curr_line = 0;

//Timer definitions
static void (*timer_callback) (void);

//Time definitions
static void rtc_setup(void);

//Sensor definitions
static void light_start(void);
static void temp_start(void);
static uint32_t light_read(void);
static uint32_t temp_read(void);

//Serial definitions

//UART1
//(8-bit, no parity, 1 stop bit)
#define UART_SERIAL_MODE    UART_MR_PAR_NO
#define PINS_UART1_PIO      PIOB		  //Rx and Tx pins are in Parallel IO B
#define PINS_UART1_ID       ID_UART1
#define PINS_UART1_TYPE     PIO_PERIPH_A  //UART1 is part of Peripheral A pg. 762 SAM4S Data sheet
#define PINS_UART1_ATTR     PIO_DEFAULT
#define PINS_UART1_MASK     (PIO_PB2A_URXD1| PIO_PB3A_UTXD1 )

static void configure_uart1(uint32_t baudrate);
static void uart1_putc( uint8_t );
static void uart1_getc( uint8_t* );
static void uart1_start( uint32_t );
static void uart1_start_int( uint32_t, void (*) (uint32_t) );
static void (*uart1_callback)(uint32_t);	//UART1 character received callback

//USART 1
//(8-bit, no parity, 1 stop bit)
#define USART_SERIAL                USART1
#define USART_SERIAL_ID             ID_USART1
#define USART_SERIAL_CHAR_LENGTH    US_MR_CHRL_8_BIT
#define USART_SERIAL_PARITY         US_MR_PAR_NO
#define USART_SERIAL_STOP_BIT       US_MR_NBSTOP_1_BIT
#define PINS_USART1_PIO				PIOA
#define PINS_USART1_ID				ID_USART1
#define PINS_USART1_TYPE			PIO_PERIPH_A
#define PINS_USART1_ATTR			PIO_DEFAULT
#define PINS_USART1_MASK			(PIO_PA21A_RXD1| PIO_PA22A_TXD1 )

static void configure_usart1(uint32_t baudrate);
static void usart1_putc( uint8_t );
static void usart1_getc( uint8_t* );
static void usart1_start( uint32_t );
static void usart1_start_int( uint32_t, void (*) (uint32_t) );
static void (*usart1_callback)(uint32_t);	//USART1 character received callback

/**
*	HAL IO Init
*
*	Initializes the board and IO pins. This function must be called before
*	any other call to an IO device. Example: hal_io_init(); hal_mtimer_Start();...
*
*/
void hal_io_init(void){
	board_init();		//initialize board pins
	ioport_init();		//initialize GPIO
}

/**
*	Millisecond Timer Start (poll)
*
*	Starts the millisecond timer without interrupts
*
*   The ms timer counts in steps of 1 ms (with a max of 1000ms). So, for instance,
*	a ms_steps parameter of 2 will make the timer increment every 2 ms. The
*	maximum ms_Step is 1000.
*
*	@param ms_steps steps needed to increment the timer by one
*/
#define RTT_MAX_VAL	32768
void hal_io_mtimer_start( uint32_t ms_steps ){
	// The RTT counts in steps of ~30us (1/2^15 seconds or  1/32768 seconds). 
	// 33 steps are 1.007ms (close enough)	
	uint32_t steps = ms_steps*33 > RTT_MAX_VAL ? RTT_MAX_VAL : ms_steps*33; 
	
	rtt_sel_source(RTT, false);
	rtt_init(RTT, steps ); 
}

/**
*	Millisecond Timer Start (interrupt)
*
*	Starts the millisecond timer with interrupts
*
*   The ms timer counts in steps of 1 ms (with a MAX of 1000ms). So, for instance,
*	a ms_steps parameter of 2 will make the timer increment every 2 ms. Every time 
*	the timer is incremented the registered callback is called.
*
*	@param ms_steps steps needed to increment the timer by one
*	@param callback the function that will be called on every increment 
*/
void hal_io_mtimer_start_int( uint32_t ms_steps, void (*callback) (void) ){
	//sets callback
	timer_callback = callback;

	//starts timer
	hal_io_mtimer_start(ms_steps);
	
	//Wait until timer starts going? (not sure about this step)
	volatile uint32_t ul_previous_time = rtt_read_timer_value(RTT);
	while (ul_previous_time == rtt_read_timer_value(RTT));

	//Clear previous on-going ints? (not sure about this step)
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	
	//Sets new int
	NVIC_SetPriority(RTT_IRQn, 0);
	NVIC_EnableIRQ(RTT_IRQn);
	rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
}

//The Timer ISR. What in upper layers is called timer, here is implemented as an RTT
void RTT_Handler(void){
	uint32_t ul_status = rtt_get_status(RTT);
	
	//Check whether the Timer has been incremented since the last read
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC)
		(*timer_callback)(); //call registered callback
}

/**
*	MIllisecond Timer Stop
*
*	Stops the Timer
*/
void hal_io_mtimer_stop(void){
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN);
}

/**
*	Millisecond Timer Read
*
*	Returns the counter value in the millisecond timer
*
*	@return the count so far
*/
uint32_t hal_io_mtimer_read( void ){
	return rtt_read_timer_value (RTT);
}

/**
*	Clock Start
*
*	Starts and initializes the system clock's time
*
*/
void hal_io_clock_start( void ){
	rtc_setup();
}

void rtc_setup(){
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
	while (!pmc_osc_is_ready_32kxtal());
	rtc_set_hour_mode(RTC, 0); //24-hrs mode by default
}


/**
*	Clock write
*
*	Write the system clock's time
*
*	@param t a pointer to a tTime structure specifying the time
*/
void hal_io_clock_write( tTime* t ){
	volatile tTime* t_ptr = t;
	rtc_set_date( RTC, t->year, t->month, t->day, 1 );	//not too sure how week works, but if week is
														//set to 0, setting of the rest fails
	rtc_set_time( RTC, t->hours, t->minutes, t->seconds );
}

/**
*	Clock read
*
*	Reads the system's clock time
*
*	@param t a pointer to a tTime structure 
*/
void hal_io_clock_read( tTime* t ){
	uint32_t dummy_week = 0;
	
	rtc_get_time(RTC, &(t->hours), &(t->minutes), &(t->seconds));
	rtc_get_date(RTC, &(t->year), &(t->month), &(t->day), &dummy_week);
}

/**
*	Display Start
*
*	Initializes and starts the board's display
*/
void hal_io_display_start(void){
	ssd1306_init();	//on-board OLED
}

/**
*	Display putc
*
*	Writes a character to the display
*
*	@param character the character to be printed
*/
void hal_io_display_putc( uint8_t character ){
	uint8_t *char_ptr, i=0;
	char_ptr = font_table[character - 32];
	
	for (i = 1; i <= char_ptr[0]; i++) {
		ssd1306_write_data(char_ptr[i]);
	}
	
	//put extra space between characters
	ssd1306_write_data(0x00); 
}

/**
*	Display CLS
*
*	Clears the Display (like the old MS-DOS cls)
*
*	@param string the formatted output
*/
void hal_io_display_cls(void){
	ssd1306_clear();
	ssd1306_set_page_address( display_curr_line = 0 );
	ssd1306_set_column_address(0);
}

/**
*	Display Gotoxy
*
*	Sets the cursor in a given coordinate. (Like Borland's gotoxy)
*
*	@param x x coordinate
*	@param y y coordinate
*/
void hal_io_display_gotoxy( uint32_t x, uint32_t y ){
	ssd1306_set_page_address( display_curr_line = x);
	ssd1306_set_column_address(y);
}


/**
*	Display Number of Lines
*
*	Returns the number of lines that fit in the display, as per the driver's font size.
*
*	@return number of lines in display
*/
uint32_t hal_io_display_numlines( void ){
	return 4; //4 lines in OLED
}

/**
*	Display Current Line
*
*	@return the display's current line
*/
uint32_t hal_io_display_currline( void ){
	return display_curr_line;
}

/**
*	Sensor Start
*
*	Initializes and starts a sensor
*
*	@param sensor the id of the sensor to be started
*/
void hal_io_sensor_start_poll( tSensorId sensor_id ){
	switch( sensor_id ){
		case SensorLight:	light_start();	break;
		case SensorTemp:	temp_start();	break;
		default:			/*Error*/;
	}
}

/**
*	Sensor Start (Interrupt)
*
*	This is an alternative for those sensors that trigger an interrupt 
*	to signal some event (e.g. In some IMU sensors, and INT is raised when they wake up).
*
*	@param sensor the id of the sensor to be started
*	@param callback the function to be called on sensor interrupt
*/
void hal_io_sensor_start_int( tSensorId sensor_id, void(*callback)(void) ){
	//so far there is none
	//...
}

/**
*	Sensor Read
*
*	Reads the specified sensor. 
*	 - Temperature is returned in degrees Celsius
*	 - Light is returned as a percentage value (0 being the darkest)
*
*	@param sensor the sensor to be read
*
*	@return the sensor reading
*/
uint32_t hal_io_sensor_read( tSensorId sensor_id ){
	uint32_t val = 0;
	
	switch( sensor_id ){
		case SensorLight:
			val = light_read();		
			val = 100 - ( val/4095.0 * 100 ); //normalize to 100, 0 being darkest
			break;
		case SensorTemp:	
			val = temp_read();		
			break;
		default:			/*Error*/;
	}
	
	return val;
}

static void light_start(void){
	//Configure ADC pin for light sensor
	gpio_configure_pin(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_FLAGS);
	
	//Enable ADC clock
	pmc_enable_periph_clk(ID_ADC);
	
	//Configure ADC4 in free Running mode
	//(no need to use interrupts)
	adc_init(ADC, sysclk_get_cpu_hz(), 1000000, ADC_MR_STARTUP_SUT0);
	adc_enable_channel(ADC, ADC_CHANNEL_4);		//ADC channel 4
	adc_configure_trigger(ADC, ADC_TRIG_SW, 1);	
	
	//Start 
	adc_start(ADC);		
}

static void temp_start(void){
	at30tse_init();	
}

static uint32_t temp_read(void){
	volatile double ret_val = 0;
	volatile uint64_t tries = 3;
	
	//do three attempts to read temperature
	while( at30tse_read_temperature(&ret_val) != TWI_SUCCESS && tries++ <= 3);
	
	return (uint32_t)ret_val; //return truncated temperature
}

static uint32_t light_read(void){
	//12-bit resolution read from channel 4
	return adc_get_channel_value(ADC, ADC_CHANNEL_4);
}

/**
*	Buttons Start (Poll)
*
*	Starts all buttons in the board
*
*/
void hal_io_button_startall_poll(){
	//Nothing to do (buttons are just GPIO) 
	
	//Maybe in the future set an interrupt on raising and falling egde
	//and update a state var on every int. That way ca use the debouce filter
	//even if we're polling. 
}


/**
*	Buttons Start (Interrupt)
*
*	Starts buttons in the board. Every time a button is pressed, the registered callback
*	is called. 
*
*	@param callback Button pressed callback
*/
void hal_io_button_startall_int( void (*callback) (tButtonNum) ){
	//NOTES:
	//	-Button interrupts trigger on raising edge (see PIN_PUSHBUTTON_x_ATTR)
	
	//set callback
	button_callback = callback; 

	// Configure Pushbutton 0
	pmc_enable_periph_clk(PIN_PUSHBUTTON_0_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_0_PIO, PIN_PUSHBUTTON_0_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_0_PIO, PIN_PUSHBUTTON_0_ID,
					PIN_PUSHBUTTON_0_MASK, PIN_PUSHBUTTON_0_ATTR, button_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_0_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_0_PIO, (IRQn_Type) PIN_PUSHBUTTON_0_ID, buttons_irq_priority);
	pio_enable_interrupt(PIN_PUSHBUTTON_0_PIO, PIN_PUSHBUTTON_0_MASK);
	
	// Configure Pushbutton 1
	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
					PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, button_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO, (IRQn_Type) PIN_PUSHBUTTON_1_ID, buttons_irq_priority);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);

	// Configure Pushbutton 2
	pmc_enable_periph_clk(PIN_PUSHBUTTON_2_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID,
					PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR, button_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_2_PIO, (IRQn_Type) PIN_PUSHBUTTON_2_ID, buttons_irq_priority);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);

	// Configure Pushbutton 3
	pmc_enable_periph_clk(PIN_PUSHBUTTON_3_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_ID,
					PIN_PUSHBUTTON_3_MASK, PIN_PUSHBUTTON_3_ATTR, button_handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_3_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_3_PIO, (IRQn_Type) PIN_PUSHBUTTON_3_ID, buttons_irq_priority);
	pio_enable_interrupt(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK);
}

/**
*	Button Read
*
*	Reads a button
*
*	@param Button number to be read
*
*   @return The state of the button
*/
tButtonState hal_io_button_read( tButtonNum button_num ){
	
	//To do... update state by setting falling edge + rising edge interrupts
	
	switch( button_num ){
		case 0: return !ioport_get_pin_level( BUTTON0_PIN );
		case 1: return !ioport_get_pin_level( BUTTON1_PIN );
		case 2: return !ioport_get_pin_level( BUTTON2_PIN );
		case 3: return !ioport_get_pin_level( BUTTON3_PIN );
		default: return ButtonUnpressed; //Button does not exist		
	}
}

//The Button interrupt service routine (ISR)
static void button_handler(uint32_t id, uint32_t mask){
	
	if((PIN_PUSHBUTTON_0_ID == id) && (PIN_PUSHBUTTON_0_MASK == mask)){
		//Button 0 was pressed
		(*button_callback)(0);	
	}
	else if ((PIN_PUSHBUTTON_1_ID == id) && (PIN_PUSHBUTTON_1_MASK == mask)){
		//Button 1 was pressed
		(*button_callback)(1);
	}
	else if((PIN_PUSHBUTTON_2_ID == id) && (PIN_PUSHBUTTON_2_MASK == mask)){
		//Button 2 was pressed
		(*button_callback)(2);
	}
	else if((PIN_PUSHBUTTON_3_ID == id) && (PIN_PUSHBUTTON_3_MASK == mask)){
		//Button 3 was pressed
		(*button_callback)(3);
	}
	else{
		//Error
	}
}

/**
*	Serial Start (Poll)
*
*	Starts a specified serial interface. By default 8-bit, no parity, 1 stop bit.
*
*	@param serial_id Serial interface Id: SerialA, SerialB, ... 
*	@param baudrate baud rate
*/
void hal_io_serial_start_poll( tSerialId serial_id, uint32_t baudrate ){
	switch( serial_id ){
		case SerialA:	configure_uart1(baudrate);	break;	//SerialA is UART1
		case SerialB:	configure_usart1(baudrate);	break;	//SerialB is USART1
		default:			/*Error*/;
	}
}

/**
*	Serial Start (with Interrupt)
*
*	Starts a specified serial interface (8-bit, no parity, 1 stop bit) 
*	and registers a callback that will be called when data is received
*
*	@param serial_id Serial interface Id: SerialA, SerialB, ... 
*	@param baudrate baud rate
*	@param callback the callback
*/
void hal_io_serial_start_int( tSerialId serial_id, uint32_t baudrate, void (*callback) (uint32_t) ){
	switch( serial_id ){
		case SerialA:	uart1_start_int(baudrate, callback);	break; //SerialA is UART1
		case SerialB:	usart1_start_int(baudrate, callback);	break; //SerialB is USART1
		default:			/*Error*/;
	}
}

/**
*	Serial putc
*
*	Writes a character to the specified Serial interface. Returns
*	until writing has successfully completed.
*
*	@param serial_id Serial interface Id: SerialA, SerialB, ...
*	@param c the character to be sent
*/
void hal_io_serial_putc( tSerialId serial_id, uint8_t c ){
	switch( serial_id ){
		case SerialA:	uart1_putc(c);	break; //SerialA is UART1
		case SerialB:	usart1_putc(c);	break; //SerialB is USART1
		default:			/*Error*/;
	}
}

/**
*	Serial getc
*
*	Reads a character from the specified Serial Interface. This
*	function will not return until a character is available.
*
*	@return the read character
*/
uint8_t hal_io_serial_getc( tSerialId serial_id ){
	uint8_t buf = 0;
	
	switch( serial_id ){
		case SerialA:	uart1_getc(&buf);	break;
		case SerialB:	usart1_getc(&buf);	break;
		default:			/*Error*/;
	}
	
	return buf;
}

void uart1_start_int( uint32_t baudrate, void (*callback) (uint32_t) ){
	configure_uart1(baudrate);	//Configures UART1
	uart1_callback = callback;	//registers callback
	
	//sets interrupt
	uart_enable_interrupt(UART1,UART_IER_RXRDY);
	NVIC_EnableIRQ(UART1_IRQn);
}

void usart1_start_int( uint32_t baudrate, void (*callback) (uint32_t) ){
	configure_usart1(115200);	//configures usart1
	usart1_callback = callback;	//registers callback
	
	//sets interrupt
	usart_enable_interrupt(USART_SERIAL, US_IER_RXRDY);
	NVIC_EnableIRQ(USART1_IRQn);
}


void configure_uart1(uint32_t baudrate){
	pio_configure(PINS_UART1_PIO, PINS_UART1_TYPE, PINS_UART1_MASK, PINS_UART1_ATTR);
	pmc_enable_periph_clk(ID_UART1);
	
	const sam_uart_opt_t uart1_sett = { sysclk_get_cpu_hz(), baudrate, UART_SERIAL_MODE };
	uart_init(UART1,&uart1_sett);
	uart_enable_tx(UART1);
	uart_enable_rx(UART1);
}

void configure_usart1(uint32_t baudrate){
	pio_configure(PINS_USART1_PIO, PINS_USART1_TYPE, PINS_USART1_MASK, PINS_USART1_ATTR);
	pmc_enable_periph_clk(ID_USART1);

	const sam_usart_opt_t usart_console_settings = {
		baudrate,
		USART_SERIAL_CHAR_LENGTH,
		USART_SERIAL_PARITY,
		USART_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};

	sysclk_enable_peripheral_clock(USART_SERIAL_ID);
	usart_init_rs232(USART_SERIAL, &usart_console_settings, sysclk_get_peripheral_hz());
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
}

static void uart1_putc( uint8_t c ){
	while ( !(UART1->UART_SR & UART_SR_TXRDY) ); //wait for Tx to be available
	uart_write( UART1, c );						 //transmit
}

static void usart1_putc( uint8_t c ){
	while( !(USART_SERIAL->US_CSR & US_CSR_TXRDY) ); //wait until TX is ready
	usart_write( USART_SERIAL, c );					 //write
}

static void uart1_getc( uint8_t* buf ){
	while( !(uart_get_status(UART1) & UART_SR_RXRDY) ); //wait until a character is ready
	uart_read( UART1, buf );							//read
}

static void usart1_getc( uint8_t* buf ){
	while( !(usart_get_status(USART1) & US_CSR_RXRDY) ); //wait until a character is available
	usart_read(USART1, buf);							 //read
}


//UART1 ISR
void UART1_Handler(){
	uint8_t rx_data;
	uint32_t status = uart_get_status(UART1);
	
	if(status & UART_SR_RXRDY){
		//read data received (1 byte)
		uart_read(UART1, &rx_data);
		
		//pass to upper layer
		uart1_callback(rx_data);
	}
}

//USART1 ISR
void USART1_Handler(){
	uint8_t received_byte;
	uint32_t dw_status = usart_get_status(USART1);
	
	if (dw_status & US_CSR_RXRDY) {
		//read received byte
		usart_read(USART1, &received_byte);
		
		//pass to upper layer
		usart1_callback(received_byte);
	}
}
