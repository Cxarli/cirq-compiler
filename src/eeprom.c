#include "eeprom.h"

#include <assert.h>

#include "utils.h"
#include "benchmark.h"


void eeprom_init(eeprom_t *eeprom) {
    FUNC_START();
    assert(eeprom != NULL);


    // Initial values are all 1's
    for (uint8_t i=0; i < 0xff; i++) {
        eeprom->values[i] = 0b11111111;
    }

    FUNC_END();
}


void eeprom_set(eeprom_t *eeprom, uint8_t address, uint8_t value) {
    FUNC_START();

    assert(eeprom != NULL);

    eeprom->values[address] = value;

    FUNC_END();
}


uint8_t eeprom_get(eeprom_t *eeprom, uint8_t address) {
    FUNC_START();

    assert(eeprom != NULL);

    uint8_t value = eeprom->values[address];

    FUNC_END();
    return value;
}


void eeprom_print(eeprom_t *eeprom, uint8_t min_addr, uint8_t max_addr) {
    FUNC_START();
    eeprom_fprint(eeprom, min_addr, max_addr, stdout);
    FUNC_END();
}


void eeprom_fprint(eeprom_t *eeprom, uint8_t min_addr, uint8_t max_addr, FILE* outstream) {
    FUNC_START();
    assert(eeprom != NULL);
    assert(outstream != NULL);


    for (uint8_t i=min_addr; i < max_addr; i++) {
        uint8_t value = eeprom_get(eeprom, i);

        fprintf(outstream, "%02x: " BINARY_PATTERN " (0x%02x)\n", i, TO_BINARY(value), value);
    }

    FUNC_END();
}


void eeprom_free(eeprom_t *eeprom) {
    FUNC_START();
    assert(eeprom != NULL);
    FUNC_END();
}
