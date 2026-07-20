#include "efi.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con_out = SystemTable->con_out;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* con_in = SystemTable->con_in;
	EFI_RUNTIME_SERVIES* runtime_services = SystemTable->runtime_services;

	con_out->set_attribute(con_out, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK));
	con_out->clear_screen(con_out);
	con_out->output_string(con_out, u"Hello, world!");

	con_out->output_string(con_out, u"\r\n\r\nPress any key to shutdown...");
	EFI_INPUT_KEY key;
	while (con_in->read_key(con_in, &key) != EFI_SUCCESS);

	runtime_services->reset_system(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

	// unreachable
	return EFI_SUCCESS;
}