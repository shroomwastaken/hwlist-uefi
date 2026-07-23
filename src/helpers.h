#ifndef HELPERS_H
#define HELPERS_H

#include <uchar.h>

#include "guid.h"
#include "efi.h"

// locates configuration table by its guid
EFI_CONFIGURATION_TABLE* config_table_by_guid(EFI_CONFIGURATION_TABLE* tables, uint64_t num_tables, EFI_GUID guid);

// self-explanatory
void output_field_hex(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out, uint64_t num, char16_t* label);
void output_field_decimal(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out, uint64_t num, char16_t* label);

#endif // HELPERS_H