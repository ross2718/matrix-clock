#include <avr/io.h>
#include "gpio.hpp"
#include "common.hpp"
#include "shared.hpp"

using namespace Gpio;

static constexpr uint8_t *port_reg(Pin pin)
{
        switch (pin/8) {
#ifdef PORTA
        case 0: return const_cast<uint8_t*>(&PORTA);
#endif
#ifdef PORTB
        case 1: return const_cast<uint8_t*>(&PORTB);
#endif
#ifdef PORTC
        case 2: return const_cast<uint8_t*>(&PORTC);
#endif
#ifdef PORTD
        case 3: return const_cast<uint8_t*>(&PORTD);
#endif
#ifdef PORTE
        case 4: return const_cast<uint8_t*>(&PORTE);
#endif
#ifdef PORTF
        case 5: return const_cast<uint8_t*>(&PORTF);
#endif
#ifdef PORTG
        case 6: return const_cast<uint8_t*>(&PORTG);
#endif
        default: return nullptr;
        }
}

static constexpr uint8_t *ddr_reg(uint8_t *port)
{
        return port ? (port - 1) : nullptr;
}

static constexpr uint8_t *portin_reg(uint8_t *port)
{
        return port ? (port - 2) : nullptr;
}

static constexpr uint8_t pin_mask(Pin pin)
{
        return 1 << (pin % 8);
}

void Gpio::set(Pin pin, State state)
{
        uint8_t *const port = port_reg(pin);
        uint8_t *const ddr = ddr_reg(port);
        const uint8_t mask = pin_mask(pin);

        if (port) {
                memory_barrier();
                set_bits(*ddr, mask, state & 0b10);
                set_bits(*port, mask, state & 0b01);
        }
}

void Gpio::write(Pin pin, bool val)
{
        set(pin, val ? high : low);
}

void Gpio::toggle(Pin pin)
{
        uint8_t *const port = port_reg(pin);

        if (port) {
                memory_barrier();
                *port ^= pin_mask(pin);
        }
}

bool Gpio::read(Pin pin)
{
        uint8_t *const port = port_reg(pin);

        memory_barrier();
        return port ? (*portin_reg(port) & pin_mask(pin)) : 0;
}
