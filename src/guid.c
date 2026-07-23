#include "guid.h"

bool compare_guid(EFI_GUID lhs, EFI_GUID rhs) {
	uint8_t* l8 = (uint8_t*)&lhs;
	uint8_t* r8 = (uint8_t*)&rhs;

	for (int i = 0; i < 16; i++) {
		if (l8[i] != r8[i]) {
			return false;
		}
	}
	return true;
}