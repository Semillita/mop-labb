typedef struct {
    int up;
    int down;
} PlayerInput;

struct Input {
    PlayerInput player_1;
    PlayerInput player_2;
};

void InitKeyboard(void (*interrupt_handler)());

// Returnera en byte som motsvarar den knapp som är nedtryckt, 
// eller 0xFF om ingen knapp är nedtryckt
unsigned char keyb(void);

struct Input get_input();

void init_keyboard();