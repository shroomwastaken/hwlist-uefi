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

// only required types until i figure out which other ones are needed
typedef enum : uint8_t {
	BiosInfo                 = 0,
	SystemInfo               = 1,
	SystemEnclosure          = 3,
	ProcessorInfo            = 4,
	PhysicalMemoryArray      = 16,
	MemoryDevice             = 17,
	MemoryArrayMappedAddress = 19,
	SystemBootInformation    = 32,
	EndOfTable               = 127
} SMBIOS_TYPE;

typedef struct {
	SMBIOS_TYPE type;
	uint8_t length;
	uint16_t handle;
} __attribute__((packed)) SMBIOS_HEADER;

// most times whenever fields are uint8_t, they're indices of strings

// --- System Information (type 1) ---

typedef struct {
	SMBIOS_HEADER header;
	uint8_t manufacturer;
	uint8_t product_name;
	uint8_t version;
	uint8_t serial_number;
	uint8_t uuid[16];
	uint8_t wakeup_type;
	uint8_t sku_number;
	uint8_t family;
} __attribute__((packed)) SMBIOS_SYSTEM_INFO;

// helper
char* smbios_string_by_index(SMBIOS_HEADER* header, uint8_t idx);

#endif // SMBIOS_H