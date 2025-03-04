/* 
   Driver for the RX5808 5.8 GHz receiver and other peripherals (Arduino port)
   Creator: @SamcraftSam

   All the RX5808 (vid) stuff below is not well dcumented
*/

/* channels */

//      1    2    3    4    5    6    7    8
// A 5865 5845 5825 5805 5785 5765 5745 5725
// B 5733 5752 5771 5790 5809 5828 5847 5866
// E 5705 5685 5665 5645 5885 5905 5925 5945
// F 5740 5760 5780 5800 5820 5840 5860 5880
// R 5658 5695 5732 5769 5806 5843 5880 5917
// D 5362 5399 5436 5473 5510 5547 5584 5621
// U 5325 5348 5366 5384 5402 5420 5438 5456
// O 5474 5492 5510 5528 5546 5564 5582 5600
// L 5333 5373 5413 5453 5493 5533 5573 5613
// H 5653 5693 5733 5773 5813 5853 5893 5933
// X 4990 5020 5050 5080 5110 5140 5170 5200
// Z 6002 6028 6054 6080 6106 6132 6158 6184
// P 4900 4921 4940 4958 5060 5070 5090 5100
// Q 5120 5130 5150 5160 5180 5190 5210 5220
// S 5230 5240 5250 5260 5270 5280 5290 5300
// T 5313 5633 5675 5715 5955 5965 5975 5990
// U 5960 5980 6000 6020 6030 6040 6050 6060
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <Arduino.h>
#include <SPI.h>

#define SPI_MOSI_PIN 11
#define SPI_SCK_PIN  13 //pb1 15 9
#define SPI_SS_PIN   10


#define SIG_TIMING 2

const char bands[] PROGMEM = {'A','B','E','F','R','D','U','O','L','H','X','Z','P','Q','S','T','W'};
#define BANDS_COUNT (sizeof(bands)/sizeof(bands[0]))
#define CHANNELS_PER_BAND 8

const uint16_t channelTable[] PROGMEM = {
    // A Band
    5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725,
    // B Band
    5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866,
    // E Band
    5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945,
    // F Band
    5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880,
    // R Band
    5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917,
    // D Band
    5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621,
    // U Band
    5325, 5348, 5366, 5384, 5402, 5420, 5438, 5456,
    // O Band
    5474, 5492, 5510, 5528, 5546, 5564, 5582, 5600,
    // L Band
    5333, 5373, 5413, 5453, 5493, 5533, 5573, 5613,
    // H Band
    5653, 5693, 5733, 5773, 5813, 5853, 5893, 5933,
    // X Band
    4990, 5020, 5050, 5080, 5110, 5140, 5170, 5200,
    // Z Band
    6002, 6028, 6054, 6080, 6106, 6132, 6158, 6184,
    // P Band
    4900, 4921, 4940, 4958, 5060, 5070, 5090, 5100,
    // Q Band
    5120, 5130, 5150, 5160, 5180, 5190, 5210, 5220,
    // S Band
    5230, 5240, 5250, 5260, 5270, 5280, 5290, 5300,
    // T Band
    5313, 5633, 5675, 5715, 5955, 5965, 5975, 5990,
    // W Band
    5960, 5980, 6000, 6020, 6030, 6040, 6050, 6060
};

void manual_spi_init() {
    pinMode(SPI_MOSI_PIN, OUTPUT);
    pinMode(SPI_SCK_PIN, OUTPUT);
    pinMode(SPI_SS_PIN, OUTPUT);
    digitalWrite(SPI_SS_PIN, HIGH); 
}

void spi_send_bits(uint32_t data, uint8_t num_bits) {
    for (uint8_t i = 0; i < num_bits; i++) {
        
        digitalWrite(SPI_MOSI_PIN, (data >> i) & 0x01);
        
        digitalWrite(SPI_SCK_PIN, HIGH);
        _delay_us(1);
        digitalWrite(SPI_SCK_PIN, LOW);
    }
}

void set_5ghz_channel_freq(uint16_t freq) {
    uint32_t div = ((((freq - 479) / 2) / 32) << 7) | (((freq - 479) / 2) % 32);
    digitalWrite(SPI_SS_PIN, LOW);
    _delay_us(1);
    spi_send_bits(0x01, 4);
    _delay_us(1);
    spi_send_bits(0x01, 1);
    _delay_us(1);
    spi_send_bits(div, 20);
    _delay_us(1);
    digitalWrite(SPI_SS_PIN, HIGH); 
} 

#ifdef TEST
int main() {
    manual_spi_init();
    // TEST CODE HERE
}
#endif
