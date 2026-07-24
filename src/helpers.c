#include "helpers.h"

#include <stdint.h>

const char16_t hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

EFI_CONFIGURATION_TABLE* config_table_by_guid(EFI_CONFIGURATION_TABLE* tables, uint64_t num_tables, EFI_GUID guid) {
	EFI_CONFIGURATION_TABLE* table = tables;
	for (uint64_t i = 0; i < num_tables; i++) {
		if (compare_guid(guid, table->vendor_guid)) {
			return table->vendor_table;
		}
		table++;
	}
	return NULL;
}

// converts number to hex representation and puts it in buffer
// buffer must be large enough (17 for uint64 max value)
void number_to_hex(uint64_t num, char16_t* buffer) {
	if (num == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return;
	}

	char16_t temp[17];
	uint8_t idx = 0;
	while (num > 0) {
		uint8_t digit = num & 0xF;
		if (digit < 10) {
			temp[idx++] = '0' + digit;
		} else {
			temp[idx++] = 'a' + digit - 10;
		}
		num >>= 4;
	}

	for (uint8_t i = 0; i < idx; i++) {
		buffer[i] = temp[idx - i - 1];
	}

	buffer[idx] = '\0';
}

// converts number to decimal representation and puts it in buffer
// buffer must be large enough (21 characters for uint64 max value);
void number_to_decimal(uint64_t num, char16_t* buffer) {
	if (num == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return;
	}

	char16_t temp[21];
	uint8_t idx = 0;
	while (num > 0) {
		uint8_t digit = num % 10;
		temp[idx++] = '0' + digit;
		num /= 10;
	}

	for (uint8_t i = 0; i < idx; i++) {
		buffer[i] = temp[idx - i - 1];
	}

	buffer[idx] = '\0';
}

void output_field_hex(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out, uint64_t num, char16_t* label) {
	char16_t buf[17];
	number_to_hex(num, buf);

	con_out->output_string(con_out, u"\r\n");
	con_out->output_string(con_out, label);
	con_out->output_string(con_out, u": ");
	con_out->output_string(con_out, buf);
}

void output_field_decimal(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out, uint64_t num, char16_t* label) {
	char16_t buf[21];
	number_to_decimal(num, buf);

	con_out->output_string(con_out, u"\r\n");
	con_out->output_string(con_out, label);
	con_out->output_string(con_out, u": ");
	con_out->output_string(con_out, buf);
}

uint64_t strlen8(char* c) {
	if (c == NULL) return 0;
	uint64_t res = 0;
	while (*(c++) != '\0') res++;
	return res;
}

void output_field_string8(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out, EFI_BOOT_SERVICES* bs, char* string, char16_t* label) {
	uint64_t len = strlen8(string);

	// TODO: add error checking for allocation

	char16_t* buffer = NULL;
	bs->allocate_pool(EfiBootServicesData, (len + 1)*sizeof(char16_t), (void**)&buffer);

	for (uint64_t i = 0; i <= len; i++) {
		buffer[i] = (char16_t)string[i];
	}

	con_out->output_string(con_out, u"\r\n");
	con_out->output_string(con_out, label);
	con_out->output_string(con_out, u": ");
	con_out->output_string(con_out, buffer);

	bs->free_pool(buffer);
}