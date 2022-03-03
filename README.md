
## A small locker (super early stage prototype) to retrieve deliveries
It won't be very energy efficient (using FreeRTOS is much easier than not using any OS on an ESP32) but it will still use low energy bluetooth and be as energy efficient as I can make it

### First step
No bluetooth, just a code lock that opens when the right (statically defined) code is entered.

### Second step
Add bluetooth support with a basic interface.

### Third step
Add code generation at execution time.
