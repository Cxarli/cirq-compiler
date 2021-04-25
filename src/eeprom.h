#ifndef EEPROM_H
#define EEPROM_H


#include <stdint.h>
#include <stdio.h>


typedef struct {
    uint8_t values[0xff];
} eeprom_t;


void eeprom_set(eeprom_t *eeprom, uint8_t address, uint8_t value);
uint8_t eeprom_get(eeprom_t *eeprom, uint8_t address);

void eeprom_init(eeprom_t *eeprom);
void eeprom_print(eeprom_t *eeprom, uint8_t min_addr, uint8_t max_addr);
void eeprom_fprint(eeprom_t *eeprom, uint8_t min_addr, uint8_t max_addr, FILE* outstream);
void eeprom_free(eeprom_t *eeprom);


#endif
