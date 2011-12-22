/*
 * config.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

// Encoder Settings
#if !_DEV_BOARD // STK500 has inverted p/b
#define READ_ENCODER_CHANNEL1() (PIND & (1<<PD3)) // PCINT19, INT1
#else
#define READ_ENCODER_CHANNEL1() (PIND & (1<<PD3))^(1<<PD3) // PCINT19, INT1
#endif
#define ENABLE_ENCODER_CHANNEL1_ISR() EICRA |= (1 << ISC10);EIMSK |= (1 << INT1)

#if !_DEV_BOARD // STK500 has inverted p/b
#define READ_ENCODER_CHANNEL2() (PIND & (1<<PD2)) // PCINT18, INT0
#else
#define READ_ENCODER_CHANNEL2() (PIND & (1<<PD2))^(1<<PD2) // PCINT18, INT0
#endif
#define ENABLE_ENCODER_CHANNEL2_ISR() EICRA |= (1 << ISC00);EIMSK |= (1 << INT0)

#define ENCODER_DEBOUNCE_COUNT 10
#define ENCODER_DEBOUNCE_DELAY 1

#define ENCODER_SPEED_TIME 100

//Generic LED Settings
#ifndef LED_STARTUP_TIME
#if !_FAST_DEBUG
#define LED_STARTUP_TIME 500
#else
#define LED_STARTUP_TIME 10
#endif
#endif

//Error Led Settings
#define ERROR_LED_PORT PORTD
#define ERROR_LED_DDR DDRD
#define ERROR_LED_PIN PD4

#define ERROR_LED_INIT() ERROR_LED_DDR |= (1<<ERROR_LED_PIN)
#define SET_ERROR_LED(on) (on ? (ERROR_LED_PORT &= ~(1<<ERROR_LED_PIN)) : (ERROR_LED_PORT |= (1<<ERROR_LED_PIN)))

//IR Led Settings
#define IR_PORT PORTB
#define IR_DDR DDRB
#define IR_PIN PB0

#define IR_LED_INIT() IR_DDR |= (1<<IR_PIN)
#define SET_IR_LED(on) (on ? (IR_PORT |= (1<<IR_PIN)) : (IR_PORT &= ~(1<<IR_PIN)))

//Motor Settings
#define MAX_ABSOLUTE_MOTOR_LEVEL 256
#define MIN_ABSOLUTE_MOTOR_LEVEL -MAX_ABSOLUTE_MOTOR_LEVEL

#define MOTOR_LEVEL_BRAKE INT16_MIN

#define MAX_ABSOLUTE_DIRECTION 0x03FF
#define MIN_ABSOLUTE_DIRECTION 0

#define DEFAULT_DIRECTION_NULL_VALUE (MAX_ABSOLUTE_DIRECTION/2)

#define MAX_ABSOLUTE_SPEED MAX_ABSOLUTE_DIRECTION
#define MIN_ABSOLUTE_SPEED MIN_ABSOLUTE_DIRECTION
#define DEFAULT_SPEED_NULL_VALUE DEFAULT_DIRECTION_NULL_VALUE

#if _DEBUG
#define DEFAULT_MOTOR_TIMEOUT COMPUTE_TICKS(5000)
#else
#define DEFAULT_MOTOR_TIMEOUT COMPUTE_TICKS(250)
#endif

//Motor PWM Settings
#define SPEED_SET_PIN_DDR() DDRD |= ((1<<PD6)|(1<<PD7));DDRB |= (1<<PB1)

// STK500 has leds inverted
#if !_DEV_BOARD

#define DISABLE_SPEED_PWM_PIN() PORTD &= ~(1 << PD6)
#define ENABLE_SPEED_PWM_PIN() PORTD |= (1 << PD6)

#define ENABLE_FWD() PORTB |= (1<<PB1)
#define DISABLE_FWD() PORTB &= ~(1<<PB1)

#define ENABLE_REV() PORTD |= (1<<PD7)
#define DISABLE_REV() PORTD &= ~(1<<PD7)

#define DIRECTION_SET_PIN_DDR() DDRB |= (1<<PB2)

#define DISABLE_DIR_PWM_PIN() PORTB &= ~(1 << PB2)
#define ENABLE_DIR_PWM_PIN() PORTB |= (1 << PB2)
#else

#define ENABLE_SPEED_PWM_PIN() PORTD &= ~(1 << PD6)
#define DISABLE_SPEED_PWM_PIN() PORTD |= (1 << PD6)

#define DISABLE_FWD() PORTB |= (1<<PB1)
#define ENABLE_FWD() PORTB &= ~(1<<PB1)

#define DISABLE_REV() PORTD |= (1<<PD7)
#define ENABLE_REV() PORTD &= ~(1<<PD7)

#define DIRECTION_SET_PIN_DDR() DDRB |= (1<<PB2)

#define ENABLE_DIR_PWM_PIN() PORTB &= ~(1 << PB2)
#define DISABLE_DIR_PWM_PIN() PORTB |= (1 << PB2)

#endif

#define DISABLE_SPEED_PWM() (TCCR0A &= ~(1 << COM0A1))
#define ENABLE_SPEED_PWM() (TCCR0A |= (1 << COM0A1))

#define DISABLE_DIR_PWM() (TCCR1A &= ~(1 << COM1B1))
#define ENABLE_DIR_PWM() (TCCR1A |= (1 << COM1B1))

// Sensor Settings
// The number of channels that have to be read every iteration, rest are round robin
#define NUMBER_OF_SENSOR_CHANNELS 6
#define SENSOR_CHANNEL_MAX (NUMBER_OF_SENSOR_CHANNELS - 1)

#define MAX_SENSOR_LEVEL 1023
#define MAX_SENSOR_ADC MAX_SENSOR_LEVEL

#define SENSOR_REQUIRED_CHANNELS 2
#define SENSOR_REQUIRED_MAX (SENSOR_REQUIRED_CHANNELS-1)

#if (!_DEBUG)
#define SENSOR_UPDATE_TIME COMPUTE_TICKS(1) // read sensor every 1 ms
#else
#define SENSOR_UPDATE_TIME COMPUTE_TICKS(1) // read sensor every 1 ms
#endif

#if _DIP_PACKAGE
#define SENSOR_CHANNELS { 0x44, 0x45, 0x40, 0x41, 0x42, 0x43 }
#else
#define SENSOR_CHANNELS { 0x47, 0x46, 0, 0x41, 0x42, 0x43 }
#endif

//Status LED Config
#define STATUS_LED_DDR DDRD
#define STATUS_LED_PORT PORTD
#define STATUS_LED_PIN PD5

#define INIT_STATUS_LED() STATUS_LED_DDR |= (1 << STATUS_LED_PIN)
#define SET_STATUS_LED(on) (on ? (STATUS_LED_PORT &= ~(1<<STATUS_LED_PIN)) : (STATUS_LED_PORT |= (1<<STATUS_LED_PIN)))

//Timer Config
#define TIMER_TICK_SHIFT (0)

//Comm settings
#if defined __AVR_ATmega168__
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128
#else
#define RX_BUFFER_SIZE 255
#define TX_BUFFER_SIZE 255
#endif

#ifndef F_CPU
#define F_CPU 14745600
#endif

#ifndef BAUD
#define BAUD 115200
#endif

// Ring buffer config
typedef char buffer_data_t;
typedef uint8_t buffer_index_t;

//Sleep config
#define SELECTED_SLEEP_MODE SLEEP_MODE_IDLE

//Error codes
typedef enum ErrorID_t
{
	ERR_NONE = 0, //
	ERR_MOTOR_TIMEOUT = -1, //
	ERR_PARAM = -2, //
	ERR_CMD = -3, //
	ERR_EXEC = -4, //
	ERR_BUFFER_FULL = -5, //
	ERR_BUFFER_EMPTY = -6, //
	ERR_FRAME = -7,
	ERR_UNKN = -8,
	ERR_NVM_CORRUPT = -64,
	ERR_MOTOR_FAULT = -65
} ErrorID_t;

#endif /* CONFIG_H_ */
