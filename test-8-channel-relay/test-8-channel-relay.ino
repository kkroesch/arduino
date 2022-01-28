
#include <arduino-timer.h>

auto timer = timer_create_default();

const int relay_ports[] = {2, 3, 4, 5, 6, 7, 8, 9};
int current_port = 2;

void setup() {
    for (const int &port : relay_ports) {
      pinMode(port, OUTPUT);
      digitalWrite(port, HIGH);  // Relays is "off" on high pegel
    }

    timer.every(1000, flash_relay);
}

void loop() {
  timer.tick();
}

bool flash_relay() {
  set_relay(current_port);
  timer.in(500, release_relay, current_port);
  if (current_port >= 9)
    current_port = 2;
  else
    current_port ++;

}

bool set_relay(int port) {
  digitalWrite(port, LOW);

}

bool release_relay(int port) {
  digitalWrite(port, HIGH);
}
