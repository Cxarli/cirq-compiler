#include "test.h"

#include <assert.h>

#include "utils.h"
#include "eeprom.h"
#include "benchmark.h"


void test_tonumber(void) {
    FUNC_START();

    assert(tonumber("b10") == 0b10);
    assert(tonumber("o10") == 010);
    assert(tonumber("d-10") == -10);
    assert(tonumber("h10") == 0x10);

    assert(tonumber("h09") == 0x09);
    assert(tonumber("h7f") == 0x7f);
    assert(tonumber("b101010") == 0b101010);
    assert(tonumber("o76") == 076);

    FUNC_END();
}


void test_eeprom(void) {
    FUNC_START();

    eeprom_t eeprom;
    eeprom_init(&eeprom);

    // Make sure initial values are 0xff
    for (uint8_t i=0; i < 0xff; i++) {
        assert(eeprom_get(&eeprom, i) == 0xff);
    }

    // Set values
    for (uint8_t i=0; i < 0xff; i++) {
        eeprom_set(&eeprom, i, i ^ 0xcc);
    }

    // Check values
    for (uint8_t i=0; i < 0xff; i++) {
        assert(eeprom_get(&eeprom, i) == (i ^ 0xcc));
    }

    #ifdef ENABLE_DEBUG
        if (get_debug_stream() != NULL) {
            eeprom_fprint(&eeprom, 0xc0, 0xd0, get_debug_stream());
        }
    #endif

    FUNC_END();
}


void test(void) {
    FUNC_START();
    test_tonumber();
    test_eeprom();

    printf("tests ok\n");
    FUNC_END();
}
