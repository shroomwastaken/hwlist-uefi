#include "smbios.h"

#include <stddef.h>  // NULL

char* smbios_string_by_index(SMBIOS_HEADER* header, uint8_t idx) {
	if (idx == 0) return NULL;

	char* c = (char*)header + header->length;

	for (uint8_t i = 1; i < idx; i++) {
		while (*c != '\0') c++;
		c++;
	}

	return c;
}