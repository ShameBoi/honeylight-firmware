/**
 * Display.cpp
 *
 * @author Shame Boi
 */

#include <honeylight/display_types.h>
#include <honeylight/Display.h>

void Display::startDataFrame() {
    static uint32_t const zero = 0U;
    spi->transfer(&zero, nullptr, sizeof(uint32_t));
}

void Display::begin() {
    spi->setDataMode(SPI_MODE0);
    spi->setClockDivider(SPI_CLOCK_DIV2);
    spi->begin();
}

void Display::end() {
    spi->end();
}

void Display::write()  {
    startDataFrame();
    spi->transfer(live->getConstBuffer(), nullptr, DisplayBuffer::LengthInBytes);
}
