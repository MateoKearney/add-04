#include <stdio.h>
#include "pico/stdlib.h"

/* Elijo un pin de Trigger y Echo */
const uint TRIGGER = 27;
const uint ECHO = 26;
const uint LEDR = 28;
const uint LEDA = 21;
const uint LEDV = 17;

float ultrasonic_get_distance_cm (void){
  /* Escribo un 1 en el Trigger */
  gpio_put(TRIGGER, true);
  /* Espero 10 us con el pulso encendido */
  sleep_us(10);
  /* Escribo un 0 en el Trigger */
  gpio_put(TRIGGER, false);
  /* Espero a que el pulso llegue al Echo */
  while(!gpio_get(ECHO));
  /* Mido el tiempo de cuando llega el pulso */
  absolute_time_t from = get_absolute_time();
  /* Espero a que el pulso del Echo baje a cero */
  while(gpio_get(ECHO));
  /* Mido el tiempo cuando termina el pulso */
  absolute_time_t to = get_absolute_time();
  /* Saco la diferencia de tiempo (el ancho del pulso) en us */
  int64_t pulse_width = absolute_time_diff_us(from, to);
  /* Calculo la distancia */
  float distance = pulse_width / 59.0;
  return distance;
}

void ultrasonic_init(uint8_t trigger, uint8_t echo){
  gpio_init(trigger);  
    /* Configuro el Trigger como salida */
    gpio_set_dir(trigger, true);
    /* Pongo un 0 en el pin de Trigger */
    gpio_put(trigger, false);
    /* Habilito el pin de Echo */
    gpio_init(echo);
    /* Configuro el Echo como entrada */
    gpio_set_dir(echo, false);
}
int main() {
    /* Habilito el USB */
    stdio_init_all();
    sleep_ms(1000);
    printf("Habilitado!!!!!\r\n");
   
    ultrasonic_init(TRIGGER, ECHO);

    gpio_init(LEDR);
    gpio_init(LEDA);
    gpio_init(LEDV);

    gpio_set_dir(28, true);
    gpio_set_dir(21, true);
    gpio_set_dir(17, true);
    
    while (true) {
      float ultrasonic = ultrasonic_get_distance_cm();

      if(ultrasonic <10){
        gpio_put (28, true);
        gpio_put (21, false);
        gpio_put (17, false);
      }
      else if(ultrasonic <50 && ultrasonic >10){
        gpio_put (28, false);
        gpio_put (21, true);
        gpio_put (17, false);
      }
      else if(ultrasonic <100 && ultrasonic >50){
        gpio_put (28, false);
        gpio_put (21, false);
        gpio_put (17, true);
      }
      sleep_ms(500);
    }
}
