#include <inttypes.h>
#include <n7OS/irq.h>

void init_irq_entry(int irq_num, uint32_t addr) {
    uint16_t offset_inf = addr & 0xFFFF;
    uint16_t offset_sup = addr >> 16;

    idt_entry_t *idte = &idt[irq_num];

    idte->offset_inf = offset_inf;
    idte->offset_sup = offset_sup;
    idte->sel_segment = KERNEL_CS;
    idte->zero = 0;

    //                    P         DPL         S      Type (Interrupt Gate)
    idte->type_attr = (1 << 7) | (0 << 5) | (0 << 4) | 14;
}
