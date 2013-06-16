# Things to do and think about

* In the specific commands, reduce the command word size to the absolute minimum,
  i.e. make commands consisting of 8 bit code and 8 bit payload an uint8_t.
  In this case, getCommandWord() could return (DEFAULT & MASK) | payload;