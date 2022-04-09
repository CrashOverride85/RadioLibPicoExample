#ifndef _GPIOINTERRUPTMANAGER_H
#define _GPIOINTERRUPTMANAGER_H

#include <inttypes.h>

#define GPIO_MAX_NUMBER 26

typedef void (*interrupt_cb)(void);

class GpioInterruptManager
{
    public:
        static GpioInterruptManager& getInstance()
        {
            static GpioInterruptManager    instance; // Guaranteed to be destroyed.
                                                     // Instantiated on first use.
            return instance;
        }
        GpioInterruptManager(GpioInterruptManager const&)  = delete;
        void operator=(GpioInterruptManager const&)        = delete;

        void attach_interrupt(uint8_t gpio, interrupt_cb cb, uint32_t events);
        void detach_interrupt(uint8_t gpio);
    
        void interrupt(unsigned int gpio, uint32_t events);

    private:
        interrupt_cb _cb[GPIO_MAX_NUMBER] = {0};
        GpioInterruptManager() {};                    // Constructor? (the {} brackets) are needed here.
        
        
};

#endif
