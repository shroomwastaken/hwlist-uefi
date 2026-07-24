#ifndef EFI_H
#define EFI_H

#include <uchar.h>
#include <stdint.h>

#include "guid.h"

#define __int128 int128_t
#define unsigned __int128 uint128_t

#define EFIAPI __attribute__((ms_abi))

#define EFI_SUCCESS 0

typedef void* EFI_HANDLE;
typedef uint64_t EFI_STATUS;  // here and onwards UINTN is uint64_t since im making this for 64-bit systems

// a lot of fields here are void*/unused, they're here for completeness' sake

// --- EFI_SIMPLE_TEXT_INPUT_PROTOCOL ---

// forward declaration
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
	uint16_t scan_code;
	char16_t unicode_char;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, EFI_INPUT_KEY* Key);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	void* reset;  // actually a function pointer, unimplemented
	EFI_INPUT_READ_KEY read_key;
	void* wait_for_key;  // unimplemented
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// --- EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL ---

// forward declaration
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

#define EFI_BLACK  0x00
#define EFI_BLUE   0x01
#define EFI_GREEN  0x02
#define EFI_CYAN   0x03
#define EFI_RED    0x04
#define EFI_YELLOW 0x0E
#define EFI_WHITE  0x0F

#define EFI_TEXT_ATTR(fg, bg) ((fg) | ((bg) << 4))

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, char16_t* String);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, uint64_t Attribute);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_MODE) (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, uint64_t ModeNumber);

typedef struct {
	int32_t max_mode;
	int32_t mode;
	int32_t attribute;
	int32_t cursor_column;
	int32_t cursor_row;
	bool cursor_visible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void* reset;
	EFI_TEXT_STRING output_string;
	void* test_string;
	void* query_mode;
	EFI_TEXT_SET_MODE set_mode;
	EFI_TEXT_SET_ATTRIBUTE set_attribute;
	EFI_TEXT_CLEAR_SCREEN clear_screen;
	void* set_cursor_position;
	void* enable_cursor;
	SIMPLE_TEXT_OUTPUT_MODE* mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// --- EFI_TABLE_HEADER ---

typedef struct {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t crc32;
	uint32_t reserved;
} EFI_TABLE_HEADER;

// --- EFI_RUNTIME_SERVIES ---

typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific,
} EFI_RESET_TYPE;

typedef void (EFIAPI *EFI_RESET_SYSTEM) (EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, uint64_t DataSize, void* ResetData);

typedef struct {
	EFI_TABLE_HEADER header;

	void* get_time;
	void* set_time;
	void* get_wakeup_time;
	void* set_wakeup_time;

	void* set_virtual_address_map;
	void* convert_pointer;

	void* get_variable;
	void* get_next_variable_name;
	void* set_variable;

	void* get_next_high_monotonic_count;
	EFI_RESET_SYSTEM reset_system;

	void* update_capsule;
	void* query_capsule_capabilities;
} EFI_RUNTIME_SERVICES;

// --- EFI_BOOT_SERVICES ---

typedef enum {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,  // i'll be using this one
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiPersistentMemory,
	EfiUnacceptedMemoryType,
	EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL) (EFI_MEMORY_TYPE PoolType, uint64_t size, void** buffer);
typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL) (void* buffer);

typedef struct {
	EFI_TABLE_HEADER header;

	void* raise_tpl;
	void* restore_tpl;

	void* allocate_pages;
	void* free_pages;
	void* get_memory_map;
	EFI_ALLOCATE_POOL allocate_pool;
	EFI_FREE_POOL free_pool;

	void* create_event;
	void* set_timer;
	void* wait_for_event;
	void* signal_event;
	void* close_event;
	void* check_event;

	void* install_protocol_interface;
	void* reinstall_protocol_interface;
	void* uninstall_protocol_interface;
	void* handle_protocol;
	void* reserved;
	void* register_protocol_notify;
	void* locate_handle;
	void* locate_device_path;
	void* install_configuration_table;

	void* load_image;
	void* start_image;
	void* exit;
	void* unload_image;
	void* exit_boot_services;

	void* get_next_monotonic_count;
	void* stall;
	void* set_watchdog_timer;

	void* connect_controller;
	void* disconnect_controller;

	void* open_protocol;
	void* close_protocol;
	void* open_protocol_information;

	void* protocols_per_handle;
	void* locate_handle_buffer;
	void* locate_protocol;
	void* install_multiple_protocol_interfaces;
	void* uninstall_multiple_protocol_interfaces;

	void* calculate_crc32;

	void* copy_mem;
	void* set_mem;
	void* create_event_ex;
} EFI_BOOT_SERVICES;

// --- EFI_CONFIGURATION_TABLE ---

typedef struct {
	EFI_GUID vendor_guid;
	void* vendor_table;
} EFI_CONFIGURATION_TABLE;

// --- EFI_SYSTEM_TABLE ---

typedef struct {
	EFI_TABLE_HEADER header;
	char16_t* firmware_vendor;
	uint32_t firmware_revision;
	EFI_HANDLE console_in_handle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* con_in;
	EFI_HANDLE console_out_handle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out;
	EFI_HANDLE stderr_handle;
	void* std_err;
	EFI_RUNTIME_SERVICES* runtime_services;
	void* boot_services;
	uint64_t num_tables;
	EFI_CONFIGURATION_TABLE* configuration_table;
} EFI_SYSTEM_TABLE;

#endif // EFI_H