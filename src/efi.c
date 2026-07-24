#include "efi.h"
#include "helpers.h"
#include "smbios.h"

#define HWLIST_VERSION "0.1"

void key_shutdown(EFI_SYSTEM_TABLE* sys) {
	sys->con_out->output_string(sys->con_out, u"\r\n\r\nPress any key to shutdown...");
	EFI_INPUT_KEY key;
	while (sys->con_in->read_key(sys->con_in, &key) != EFI_SUCCESS);

	sys->runtime_services->reset_system(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

void read_smbios_table(EFI_SYSTEM_TABLE* SystemTable, EFI_CONFIGURATION_TABLE* table) {
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out = SystemTable->con_out;
	EFI_BOOT_SERVICES* bs = SystemTable->boot_services;
	SMBIOS_ENTRY_POINT* entry_point = (SMBIOS_ENTRY_POINT*)table;

	con_out->output_string(con_out, u"\r\n");
	output_field_decimal(con_out, entry_point->major_version, u"SMBIOS major version");
	output_field_decimal(con_out, entry_point->minor_version, u"SMBIOS minor version");
	output_field_decimal(con_out, entry_point->number_of_structures, u"Total SMBIOS structures");

	SMBIOS_HEADER* header = (SMBIOS_HEADER*)entry_point->table_address;
	while (header->type != EndOfTable) {
		con_out->output_string(con_out, u"\r\n");
		output_field_decimal(con_out, header->type, u"Structure type");

		switch (header->type) {
			case SystemInfo: {
				SMBIOS_SYSTEM_INFO* data = (SMBIOS_SYSTEM_INFO*)header;
				// TODO: make this shorter somehow cause its quite unpleasant to look at
				output_field_string8(con_out, bs, smbios_string_by_index(header, data->manufacturer), u"Manufacturer");
				output_field_string8(con_out, bs, smbios_string_by_index(header, data->product_name), u"Product Name");
				output_field_string8(con_out, bs, smbios_string_by_index(header, data->serial_number), u"Serial Number");
				break;
			};
			// TODO: implement other types
			default: break;
		}

		header = (SMBIOS_HEADER*)((uint8_t*)header + header->length);  // now pointing to strings
		while (1) {
			// no more strings
			if (*(uint16_t*)header == 0x0000) {
				// TODO: maybe write advance functions to make this cleaner
				header = (SMBIOS_HEADER*)((uint8_t*)header + 2);
				break;
			}
			header = (SMBIOS_HEADER*)((uint8_t*)header + 1);
		}
	}
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out = SystemTable->con_out;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* con_in = SystemTable->con_in;
	EFI_RUNTIME_SERVICES* runtime_services = SystemTable->runtime_services;

	con_out->set_attribute(con_out, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK));
	con_out->clear_screen(con_out);

	con_out->output_string(con_out, u"hwlist-uefi v"HWLIST_VERSION"\r\n\r\n");

	con_out->output_string(con_out, u"Firmware vendor: ");
	con_out->output_string(con_out, SystemTable->firmware_vendor);

	EFI_CONFIGURATION_TABLE* smbios_table =
		config_table_by_guid(SystemTable->configuration_table, SystemTable->num_tables, SMBIOS_TABLE_GUID);
	if (smbios_table == NULL) {
		con_out->output_string(con_out, u"\r\nSMBIOS configuration table not found!");
		key_shutdown(SystemTable);
	}
	con_out->output_string(con_out, u"\r\nSMBIOS configuration table found!");

	read_smbios_table(SystemTable, smbios_table);

	key_shutdown(SystemTable);

	// unreachable
	return EFI_SUCCESS;
}