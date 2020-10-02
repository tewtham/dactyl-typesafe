/*
 * File:   spi4teensy3.h
 * Author: xxxajk
 *
 * Created on November 21, 2013, 10:54 AM
 */


// TO-DO: KINETISKL

#ifndef SPI4TEENSY3_H
#define  SPI4TEENSY3_H
#if defined(__arm__) && defined(TEENSYDUINO)
#include <mk20dx128.h> // same header for all Teensy
#if defined(KINETISK)
#include <core_pins.h>
#include <sys/types.h>

#ifndef SPI_SR_RXCTR
#define SPI_SR_RXCTR 0XF0
#endif
#ifndef SPI_PUSHR_CONT
#define SPI_PUSHR_CONT 0X80000000
#endif
#ifndef SPI_PUSHR_CTAS
#define SPI_PUSHR_CTAS(n) (((n) & 7) << 28)
#endif

#define SPI4TEENSY3_MODE_0 0, 0
#define SPI4TEENSY3_MODE_1 0, 1
#define SPI4TEENSY3_MODE_2 1, 0
#define SPI4TEENSY3_MODE_3 1, 1
#define MODE_TO_SPI4TEENSY3_MODE(x) (x & 1), (x&2)

namespace spi4teensy3 {
        void init();
        void init(uint8_t speed);
        void init(uint8_t cpol, uint8_t cpha);
        void init(uint8_t speed, uint8_t cpol, uint8_t cpha);
        void send(uint8_t b);
        void send(void *bufr, size_t n);
        uint8_t receive();
        void receive(void *bufr, size_t n);

        //void updatectars();
};
#endif /* defined(KINETISK) */
#endif /* defined(__arm__) && defined(TEENSYDUINO) && defined(KINETISK) */
#endif  /* SPI4TEENSY3_H */
