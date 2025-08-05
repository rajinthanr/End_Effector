#include "main.h"
#include "TofSensor.h"
#include "peripherals.h"
#include "fork.h"
#include "pneumatics.h"
#include "functions.h"
#include "millis.h"

UART uart3(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
UART uart1(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1);

UART uart0(&UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, PORTE, PORTE5);

int main()
{
  millis_init();
  uart0.init();
  uart3.init();
  setup_seven_segment();

  seven_segment(3);

  uart0.transmit("Booting...\n");
  init_pneumatics();
  tof_init();
  init_fork();
  uart0.transmit("End Effector Ready\n");

  uart3.transmit("UART3 Initialized\n");

  seven_segment(1);

  while (1)
  {
    //read_tof();
    //_delay_ms(10);
    char command[64] = {0};
    if (uart0.receive_line(command, sizeof(command)))
    {
      process_command(command);
    }
  }
}