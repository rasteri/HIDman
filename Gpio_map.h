
#ifndef _GPIO_MAP_H_
#define _GPIO_MAP_H_

/**
 * Description: To use these macros, you must define PIN, DIR_PIN and OFFSET_PIN,
 * PIN is the IO pin of a port, eg. P1_0.
 * DIR_PIN is the direction register of the pin, eg. P1DIR.
 * OFFSET_PIN is the pin offset from a port, eg. for P1_0, the OFFSET_PIN is 0.
*/

#define MAKE_GPIO_PREP(port, pin)  (P##port##_##pin##)
#define PORT_DIR_PREP(port)        (P##port##_DIR)

#define MAKE_GPIO(port, pin)       MAKE_GPIO_PREP(port, pin)
#define PORT_DIR(port)             PORT_DIR_PREP(port)

#define SET_GPIO_VALUE(bit, val)   ((bit) = (val))

#define SET_GPIO_BIT(bit)          SET_GPIO_VALUE(bit, 1)

#define CLR_GPIO_BIT(bit)          SET_GPIO_VALUE(bit, 0)

#define GET_GPIO_BIT(bit)          (bit)

#define GET_P4_GPIO_BIT(bit)       ((P4_IN >> bit) & 0x01)

#define SET_GPIO_INPUT(bit)        (DIR_##bit## &= ~(1 << OFFSET_##bit##))
#define SET_GPIO_OUTPUT(bit)       (DIR_##bit## |= (1 << OFFSET_##bit##))

#endif

