#ifndef _CONFIG_H
#define _CONFIG_H

#include "GpioInterruptManager.h"

#define PIN_DI0   7
#define PIN_CS    8
#define PIN_RST   9
#define PIN_DI1  10
#define PIN_MISO 16
#define PIN_SCK  18
#define PIN_MOSI 19

#define ATTACH_INTERRUPT(gpio, user_func, events) GpioInterruptManager::getInstance().attach_interrupt(gpio, user_func, events);
#define DETACH_INTERRUPT(gpio)                    GpioInterruptManager::getInstance().detach_interrupt(gpio);

#endif
