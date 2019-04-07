#include <stdbool.h>
#include <stdint.h>

typedef uint32_t IrqType;

enum IrqType { IrqSystemTimer = 0 };

void irq_init(void);
void irq_register_callback( IrqType, bool(*)(void), void(*)(void) );
