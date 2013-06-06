/*
 * command_decoder.h
 *
 * Created: 13.12.2012 02:01:56
 *  Author: sunside
 */ 


#ifndef COMMAND_DECODER_H_
#define COMMAND_DECODER_H_

#include <stdint.h>

#define MAXIMUM_COMMAND_LENGTH			16U
#define COMMAND_READ_BUFFER_SIZE		16U			// next power of two
#define MAXIMUM_COMMAND_PAYLOAD_BYTES	10U

typedef enum {
	DECODE_SUCCESS = 0U,						//!< Decode was successful
	DECODE_PENDING = 1U,						//!< Decode still in progress
	DECODE_DONE_AWAITING_PAYLOAD = 2U,			//!< Decode was successful, yet payload is still retrieved
	DECODE_FAILED_UNKNOWN_PARTIAL_TOKEN = 254U,	//!< Decode failed because the partial token was invalid
	DECODE_FAILED_UNKNOWN_TOKEN = 255U			//!< Decode failed
} command_decode_result_t;

typedef struct {
	
	uint_least8_t command_code;								//!< The command code
	uint_least8_t payload_size;								//!< The actual payload size
	uint_least8_t payload[MAXIMUM_COMMAND_PAYLOAD_BYTES];	//!< The payload
	
} comm_command_t;

typedef struct {
	
	uint_least8_t command_code;
	uint_least8_t command_length;
	uint_least8_t payload_length;

	char command[MAXIMUM_COMMAND_LENGTH];
	uint_least8_t payload[MAXIMUM_COMMAND_PAYLOAD_BYTES];
	
} command_definition_t;

void command_define(command_definition_t *definition, uint_least8_t command_code, const char *command, uint_least8_t payload_length);

void command_decoder_init(command_definition_t definitions[], uint_least8_t count);

/*! \brief Tries to decode the command that was completed by the given data byte

	\param data		The data byte
	\param command	The completed command, if any
	\return			Nonzero if a command was decoded; 0 otherwise
 */
command_decode_result_t command_decode(uint_least8_t data, comm_command_t *command);

#endif /* COMMAND_DECODER_H_ */