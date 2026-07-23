#ifndef SMBIOS_H
#define SMBIOS_H

#include <stdint.h>

typedef struct {
	uint8_t anchor_string[4];
	uint8_t checksum;
	uint8_t entry_point_length;
	uint8_t major_version;
	uint8_t minor_version;
	uint16_t max_size;
	uint8_t revision;
	uint8_t formatted_area[5];
	uint8_t intermediate_anchor_string[5];
	uint8_t intermediate_checksum;
	uint16_t table_length;
	uint32_t table_address;
	uint16_t number_of_structures;
	uint8_t bcd_revision;
} __attribute__((packed)) SMBIOS_ENTRY_POINT;

typedef enum : uint8_t {
	BiosInfo             = 0,
	SystemInfo           = 1,
	BaseboardInformation = 2,
	SystemEnclosure      = 3,
	ProcessorInformation = 4,
	EndOfTable           = 127
} SMBIOS_TYPE;

typedef struct {
	SMBIOS_TYPE type;
	uint8_t length;
	uint16_t handle;
} __attribute__((packed)) SMBIOS_HEADER;

#endif // SMBIOS_H