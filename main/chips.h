typedef struct {
    float clock_ym2612;
    float clock_sn76489;
} chips_t;

void init_chips(chips_t *chips);
void SN76489_Write(uint8_t data);
void YM2612_Write(uint8_t adr, uint8_t data);
