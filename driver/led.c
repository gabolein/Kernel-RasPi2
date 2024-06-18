#define GPIO_BASE (0x7E200000 - 0x3F000000)
#define GREEN0_LED 4
#define YELLOW0_LED 5
#define RED_LED 6
#define YELLOW_LED 7
#define GREEN_LED 8
#define GPF_BITS 3

enum gpio_func {
	gpio_input  = 0x0, 	// GPIO Pin is an input
	gpio_output = 0x1, 	// GPIO Pin is an output
};

struct gpio {
    unsigned int func[6];
    unsigned int unused0;
    unsigned int set[2];
    unsigned int unused1;
    unsigned int clr[2];
};

static volatile struct gpio * const gpio_port = (struct gpio *)GPIO_BASE;

void yellow_on(void)
{
    /* Initialisieren */
    gpio_port->func[0] = gpio_output << (YELLOW_LED * GPF_BITS);

    /* Anschalten */
    gpio_port->set[0] = 1 << YELLOW_LED;
}

void yellow_off(void) {
    gpio_port->clr[0] = 1 << YELLOW_LED;
}

void green_on(void)
{
        /* Initialisieren */
        gpio_port->func[0] = gpio_output << (GREEN_LED * GPF_BITS);

        /* Anschalten */
        gpio_port->set[0] = 1 << GREEN_LED;
}

void green_off(void) {
        gpio_port->clr[0] = 1 << GREEN_LED;
}

void red_on(void)
{
        /* Initialisieren */
        gpio_port->func[0] = gpio_output << (RED_LED * GPF_BITS);

        /* Anschalten */
        gpio_port->set[0] = 1 << RED_LED;
}

void red_off(void) {
        gpio_port->clr[0] = 1 << RED_LED;
}

void yellow0_on(void)
{
        /* Initialisieren */
        gpio_port->func[0] = gpio_output << (YELLOW0_LED * GPF_BITS);

        /* Anschalten */
        gpio_port->set[0] = 1 << YELLOW0_LED;
}

void yellow0_off(void) {
        gpio_port->clr[0] = 1 << YELLOW0_LED;
}

void green0_on(void)
{
        /* Initialisieren */
        gpio_port->func[0] = gpio_output << (GREEN0_LED * GPF_BITS);

        /* Anschalten */
        gpio_port->set[0] = 1 << GREEN0_LED;
}

void green0_off(void) {
        gpio_port->clr[0] = 1 << GREEN0_LED;
}

