/*
 * File:   spi4teensy3.cpp
 * Author: xxxajk
 *
 * Created on November 21, 2013, 10:54 AM
 */

// TO-DO: KINETISKL

#if defined(__arm__) && defined(TEENSYDUINO)
#include "spi4teensy3.h"
#if defined(KINETISK)
/**
 * spi4teesny3 is a library for the freescale microcontroller
 * on a teensy 3.x from http://pjrc.com
 * <PRE>
 * Documentation for initialization of SPI.
 *
 * Speed:
 * Speed is the internal buss speed is _always_ divided at least by 2.
 * This table shows the speeds supported for a given value.
 *
 * speed | buss speed  | uC speed | uC speed
 * value |   divisor   | 96/48MHz |  24MHz
 * ------+-------------+----------+----------
 *   0   |      2      |  24MHz   | 12MHz
 * ------+-------------+----------+----------
 *   1   |      4      |  12MHz   | 6MHz
 * ------+-------------+----------+----------
 *   2   |      8      |  6MHz    | 3MHz
 * ------+-------------+----------+----------
 *   3   |      12     |  4MHz    | 2MHz
 * ------+-------------+----------+----------
 *   4   |      16     |  3MHz    | 1.5MHz
 * ------+-------------+----------+----------
 *   5   |      32     |  1.5MHz  | 750KHz
 * ------+-------------+----------+----------
 *   6   |      64     |  750KHz  | 375KHz
 * ------+-------------+----------+----------
 *   7   |      128    |  375KHz  | 187.5KHz
 * ------+-------------+----------+----------
 *
 * cpol is the SPI clock Polarity
 * cpha is the SPI clock capture Phase
 *
 * CPOL | CPHA | Description
 * -----+------+----------------------------------------------------------------
 *      |      | Idle clock state is low
 *  0   |  0   | data captured on clock low->high
 *      |      | data propagated on high->low
 * -----+------+----------------------------------------------------------------
 *      |      | Idle clock state is low
 *  0   |  1   | data captured on clock high->low
 *      |      | data propagated on low->high
 * -----+------+----------------------------------------------------------------
 *      |      | Idle clock state is high
 *  1   |  0   | data captured on clock high->low
 *      |      | data propagated on low->high
 * -----+------+----------------------------------------------------------------
 *      |      | Idle clock state is high
 *  1   |  1   | data captured on clock low->high
 *      |      | data propagated on high->low
 * -----+------+----------------------------------------------------------------
 *
 * cpol and cpha are specified separately instead of using mode numbers in order
 * to simplify the design, however, A set of handy macros are available for
 * initialization as follows:
 *
 * SPI4TEENSY3_MODE_0
 * SPI4TEENSY3_MODE_1
 * SPI4TEENSY3_MODE_2
 * SPI4TEENSY3_MODE_3
 * MODE_TO_SPI4TEENSY3_MODE(x) -- where 'x' is 0, 1, 2 or 3
 *
 * The following examples are all equal, but the first one is the best
 * to use for maximum speed and mode 0 as it produces the smallest amount of code.
 * This default was chosen since nearly all devices support mode 0, and all the
 * devices I use work at the maximum speed possible. Your devices might too.
 *
 * spi4teensy3::init()
 * spi4teensy3::init(0)
 * spi4teensy3::init(0, 0)
 * spi4teensy3::init(0, 0, 0)
 * spi4teensy3::init(SPI4TEENSY3_MODE_0)
 * spi4teensy3::init(MODE_TO_SPI4TEENSY3_MODE(0))
 * spi4teensy3::init(0, SPI4TEENSY3_MODE_0)
 * spi4teensy3::init(0, MODE_TO_SPI4TEENSY3_MODE(0))
 *
 * </PRE>
 */
namespace spi4teensy3 {
        uint32_t ctar0;
        uint32_t ctar1;

        void updatectars() {
                // This function is only used internally.
                uint32_t mcr = SPI0_MCR;
                if(mcr & SPI_MCR_MDIS) {
                        SPI0_CTAR0 = ctar0;
                        SPI0_CTAR1 = ctar1;
                } else {
                        SPI0_MCR = mcr | SPI_MCR_MDIS | SPI_MCR_HALT;
                        SPI0_CTAR0 = ctar0;
                        SPI0_CTAR1 = ctar1;
                        SPI0_MCR = mcr;
                }
        }

        /**
         * Generic initialization. Maximum speed, cpol and cpha 0.
         */
        void init() {
                SIM_SCGC6 |= SIM_SCGC6_SPI0;
                CORE_PIN11_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);
                CORE_PIN12_CONFIG = PORT_PCR_MUX(2);
                CORE_PIN13_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);

                // SPI_CTAR_DBR, SPI_CTAR_BR(0), SPI_CTAR_BR(1)
                ctar0 = SPI_CTAR_DBR;
                ctar1 = ctar0;
                ctar0 |= SPI_CTAR_FMSZ(7);
                ctar1 |= SPI_CTAR_FMSZ(15);
                SPI0_MCR = SPI_MCR_MSTR | SPI_MCR_PCSIS(0x1F);
                SPI0_MCR |= SPI_MCR_CLR_RXF | SPI_MCR_CLR_TXF;
                updatectars();
        }

        /**
         * Initialization with max speed, cpol and cpha configurable.
         * @param cpol SPI Polarity
         * @param cpha SPI Phase
         */
        void init(uint8_t cpol, uint8_t cpha) {
                init();
                ctar0 |= (cpol == 0 ? 0 : SPI_CTAR_CPOL) | (cpha == 0 ? 0 : SPI_CTAR_CPHA);
                ctar1 |= (cpol == 0 ? 0 : SPI_CTAR_CPOL) | (cpha == 0 ? 0 : SPI_CTAR_CPHA);
                updatectars();
        }

        /**
         * Initialization with cpol and cpha 0, speed is configurable.
         *
         * @param SPI speed [0-7]
         */
        void init(uint8_t speed) {
                init();
                // Default 1/2 speed
                uint32_t ctar = SPI_CTAR_DBR;
                switch(speed) {
                        case 1: // 1/4
                                ctar = 0;
                                break;
                        case 2: // 1/8
                                ctar = SPI_CTAR_BR(1);

                                break;
                        case 3: // 1/12
                                ctar = SPI_CTAR_BR(2);
                                break;

                        case 4: // 1/16
                                ctar = SPI_CTAR_BR(3);
                                break;

                        case 5: // 1/32
                                ctar = SPI_CTAR_PBR(1) | SPI_CTAR_BR(4);
                                break;

                        case 6: // 1/64
                                ctar = SPI_CTAR_PBR(1) | SPI_CTAR_BR(5);
                                break;

                        case 7: //1/128
                                ctar = SPI_CTAR_PBR(1) | SPI_CTAR_BR(6);
                                // fall thru
                        default:
                                // default 1/2 speed, this is the maximum.
                                break;
                }
                ctar0 = ctar | SPI_CTAR_FMSZ(7);
                ctar1 = ctar | SPI_CTAR_FMSZ(15);
                updatectars();
        }

        /**
         * Initialization with speed, cpol, and cpha configurable.
         *
         * @param SPI speed [0-7]
         * @param cpol SPI Polarity
         * @param cpha SPI Phase
         */
        void init(uint8_t speed, uint8_t cpol, uint8_t cpha) {
                init(speed);
                ctar0 |= (cpol == 0 ? 0 : SPI_CTAR_CPOL) | (cpha == 0 ? 0 : SPI_CTAR_CPHA);
                ctar1 |= (cpol == 0 ? 0 : SPI_CTAR_CPOL) | (cpha == 0 ? 0 : SPI_CTAR_CPHA);
                updatectars();
        }

        /**
         * Send 1 byte.
         *
         * @param b byte to send over SPI
         */
        void send(uint8_t b) {
                // clear any data in RX/TX FIFOs, and be certain we are in master mode.
                SPI0_MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_CLR_TXF | SPI_MCR_PCSIS(0x1F);
                SPI0_SR = SPI_SR_TCF;
                SPI0_PUSHR = SPI_PUSHR_CONT | b;
                while(!(SPI0_SR & SPI_SR_TCF));
                // While technically needed, we reset anyway.
                // Not reading SPI saves a few clock cycles over many transfers.
                //SPI0_POPR;
        }

        /**
         * Send an array of bytes.
         *
         * @param bufr array of bytes to send
         * @param n number of bytes to send
         */
        void send(void *bufr, size_t n) {
                int i;
                int nf;
                uint8_t *buf = (uint8_t *)bufr;

                if(n & 1) {
                        send(*buf++);
                        n--;
                }
                // clear any data in RX/TX FIFOs, and be certain we are in master mode.
                SPI0_MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_CLR_TXF | SPI_MCR_PCSIS(0x1F);
                // initial number of words to push into TX FIFO
                nf = n / 2 < 3 ? n / 2 : 3;
                // limit for pushing data into TX fifo
                uint8_t* limit = buf + n;
                for(i = 0; i < nf; i++) {
                        uint16_t w = (*buf++) << 8;
                        w |= *buf++;
                        SPI0_PUSHR = SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1) | w;
                }
                // write data to TX FIFO
                while(buf < limit) {
                        uint16_t w = *buf++ << 8;
                        w |= *buf++;
                        while(!(SPI0_SR & SPI_SR_RXCTR));
                        SPI0_PUSHR = SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1) | w;
                        SPI0_POPR;
                }
                // wait for data to be sent
                while(nf) {
                        while(!(SPI0_SR & SPI_SR_RXCTR));
                        SPI0_POPR;
                        nf--;
                }
        }

        /**
         * @return byte from SPI
         */
        uint8_t receive() {
                // clear any data in RX/TX FIFOs, and be certain we are in master mode.
                SPI0_MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_CLR_TXF | SPI_MCR_PCSIS(0x1F);
                SPI0_SR = SPI_SR_TCF;
                SPI0_PUSHR = SPI_PUSHR_CONT | 0xFF;
                while(!(SPI0_SR & SPI_SR_TCF));
                return SPI0_POPR;
        }

        /**
         * Receive multiple bytes from SPI.
         *
         * @param bufr array that stores bytes from SPI.
         * @param n number of bytes to receive.
         */
        void receive(void *bufr, size_t n) {
                int i;
                uint8_t *buf = (uint8_t *)bufr;

                if(n & 1) {
                        *buf++ = receive();
                        n--;
                }

                // clear any data in RX/TX FIFOs, and be certain we are in master mode.
                SPI0_MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_CLR_TXF | SPI_MCR_PCSIS(0x1F);
                // initial number of words to push into TX FIFO
                int nf = n / 2 < 3 ? n / 2 : 3;
                for(i = 0; i < nf; i++) {
                        SPI0_PUSHR = SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1) | 0XFFFF;
                }
                uint8_t* limit = buf + n - 2 * nf;
                while(buf < limit) {
                        while(!(SPI0_SR & SPI_SR_RXCTR));
                        SPI0_PUSHR = SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1) | 0XFFFF;
                        uint16_t w = SPI0_POPR;
                        *buf++ = w >> 8;
                        *buf++ = w & 0XFF;
                }
                // limit for rest of RX data
                limit += 2 * nf;
                while(buf < limit) {
                        while(!(SPI0_SR & SPI_SR_RXCTR));
                        uint16_t w = SPI0_POPR;
                        *buf++ = w >> 8;
                        *buf++ = w & 0XFF;
                }

        }
}
#endif
#endif
