#include "GpioInterruptManager.h"
#include "pico/stdlib.h"


static void s_interrupt_callback(unsigned int gpio, uint32_t events)
{
    GpioInterruptManager::getInstance().interrupt(gpio, events);
}

void GpioInterruptManager::attach_interrupt(uint8_t gpio, void (*userFunc)(void), uint32_t events)
{
    if (gpio > GPIO_MAX_NUMBER+1)
        return;

    _cb[gpio] = userFunc;
    gpio_set_irq_enabled_with_callback(gpio, events, true, &s_interrupt_callback);
}

void GpioInterruptManager::interrupt(unsigned int gpio, uint32_t events)
{
    if (gpio > GPIO_MAX_NUMBER+1)
        return;

    if (_cb[gpio] != NULL)
        _cb[gpio]();
}

void GpioInterruptManager::detach_interrupt(uint8_t gpio)
{
    if (gpio > GPIO_MAX_NUMBER+1)
        return;

    _cb[gpio] = NULL;
    gpio_set_irq_enabled(gpio, 0, false);
}

