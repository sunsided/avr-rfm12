/*
 * command_decoder.c
 *
 * Created: 14.12.2012 02:57:41
 *  Author: sunside
 */ 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "command_decoder.h"
#include "char_ringbuffer.h"


typedef enum {
	UNINITIALIZED,				//!< The parser is not initialized
	READY_OR_FAULTED,			//!< Parser is ready and waits for a new command token
	DECODE,						//!< Decoding
	AWAITING_PAYLOAD_END,		//!< A command token was found, but the total number of expected payload bytes was not reached
} command_state_t;

static command_definition_t *command_definitions;
static uint_least8_t command_definition_count;

static uint_least8_t command_read_buffer_data[COMMAND_READ_BUFFER_SIZE];
static char_ringbuffer_t payload_read_buffer;

typedef enum {
	NONE,
	PARTIAL,
	FULL,
} found_command_t;

void command_decoder_init(command_definition_t definitions[], uint_least8_t count)
{
	command_definitions = definitions;
	command_definition_count = count;
}

command_decode_result_t command_decode(uint_least8_t data, comm_command_t *command)
{
	assert (0 != command);
	
	static command_state_t state = UNINITIALIZED;
	static uint_least8_t expected_payload_bytes = 0;
	static uint_least8_t missing_payload_bytes = 0;

	static command_definition_t *detected_command = 0;
	static char command_word[MAXIMUM_COMMAND_LENGTH];
	static uint_least8_t command_word_index = 0;
	
	static uint_least8_t allow_recursion = 1;

	switch (state)
	{
		case UNINITIALIZED:
		{
			rb_init(&payload_read_buffer, command_read_buffer_data, COMMAND_READ_BUFFER_SIZE);
		}
		
		case READY_OR_FAULTED:
		{	
			// initialize
			command_word_index = 0;
			detected_command = 0;
			expected_payload_bytes = 0;
			missing_payload_bytes = 0;
						
			state = DECODE;
		}	
		
		case DECODE:
		{
			// store word
			command_word[command_word_index++] = data;
			
			// check all definitions
			found_command_t found_command = NONE;
			for (uint_least8_t i=0; i<command_definition_count; ++i)
			{			
				detected_command = &command_definitions[i];
				
				// skip commands that are too short for the given data
				if (detected_command->command_length < command_word_index) {
					// usart_comm_send_zstr("too short, skip\r\n");
					continue;
				}					
				
				// check all characters
				uint_least8_t possibly_found_command = 1;
				for (uint_least8_t ci=0; ci<command_word_index; ++ci)
				{
					if (detected_command->command[ci] != command_word[ci]) 
					{
						possibly_found_command = 0;
						break;
					}
				}
				
				// check the length
				if (possibly_found_command)
				{
					if (detected_command->command_length == command_word_index)
					{
						found_command = FULL;
						break;
					}
					
					found_command = PARTIAL;
					break;
				}
				else 
				{
					// reset the 
					found_command = NONE;
					detected_command = 0;
				}				
			}
			
			// at this point, a command was found and is kept in the definition field
			if (FULL == found_command)
			{			
				// set the read expectations
				expected_payload_bytes = missing_payload_bytes = detected_command->payload_length;
				assert (expected_payload_bytes <= COMMAND_READ_BUFFER_SIZE);
				
				// reset the read buffer
				rb_reset(&payload_read_buffer);
				
				// start reading
				allow_recursion = 1;
				state = AWAITING_PAYLOAD_END;
				break;
			}
			else if (PARTIAL == found_command)
			{
				allow_recursion = 1;
				state = DECODE;
				break;
			}

			// That's a failure.
			state = READY_OR_FAULTED;
			
			// At this point, the user may have already started a new command, so recurse
			// WARN: The problem is that a half-correct command immediately followed by a valid command will go unnoticed
			if (allow_recursion) {
				allow_recursion = 0;
				return command_decode(data, command);
			}				
			break;
		}

		case AWAITING_PAYLOAD_END:
		{
			state = AWAITING_PAYLOAD_END; // redundant, but safe
			rb_put_data_blocking(&payload_read_buffer, data);
			--missing_payload_bytes;
			break;
		}
	}
	
	// if missing payload bytes are zero, fill out the command template and return zero
	if (AWAITING_PAYLOAD_END == state)
	{
		if (0 == missing_payload_bytes)
		{
			// set the command code
			command->command_code = detected_command->command_code;
			command->payload_size = detected_command->payload_length;

			// copy the payload data from the 
			uint_least8_t copydata;
			uint_least8_t payloadidx = 0;
			++expected_payload_bytes; while (--expected_payload_bytes)
			{
				rb_get_data(&payload_read_buffer, &copydata); // TODO: assertion ...?
				command->payload[payloadidx++] = copydata;
			}
		
			// all is well
			state = READY_OR_FAULTED;
			return DECODE_SUCCESS;
		}

		// still waiting
		return DECODE_DONE_AWAITING_PAYLOAD;
	}
	else if (DECODE == state)
	{
		return DECODE_PENDING;
	}

	// crash, boom, bang.
	state = READY_OR_FAULTED;
	// usart_comm_send_zstr("faulted.");
	return DECODE_FAILED_UNKNOWN_TOKEN;
}

void command_define(command_definition_t *definition, uint_least8_t command_code, const char *command, uint_least8_t payload_length)
{
	assert (0 != definition);
	
	definition->command_code = command_code;
	
	definition->command_length = strlen(command);
	strcpy(definition->command, command);
	
	definition->payload_length = payload_length;
}