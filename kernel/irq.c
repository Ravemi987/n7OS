#include <inttypes.h>
#include <n7OS/irq.h>

// typedef struct {
//   uint16_t offset_inf;
//   uint16_t sel_segment;
//   uint8_t zero;
//   uint8_t type_attr;
//   uint16_t offset_sup;
// } idt_entry_t;


void init_irq_entry(int irq_num, uint32_t addr) {
    uint16_t offset_inf = addr & 0xFFFF;
    uint16_t offset_sup = addr >> 16;

    idt_entry_t *idt_entry = &idt[irq_num];

    idt_entry->offset_inf = offset_inf;
    idt_entry->offset_sup = offset_sup;
    idt_entry->sel_segment = KERNEL_CS;
    idt_entry->zero = 0;
    idt_entry->type_attr = 0xE8;
}
