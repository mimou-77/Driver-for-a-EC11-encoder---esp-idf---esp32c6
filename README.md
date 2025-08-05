
# EC11 ENCODER DRIVER

- 2 interrupts with the same isr
  - on pin clk (pin A)
  - on pin dta (pin B)

- CW = clockwise rotation ; CCW = counter-clockwise rotation
- 1 logging task that logs the values of sense_of_rotation and position, if a CW rotation happens: position++, if a CCW rotation happens: position--


`- According to datasheet:` 

  - For the sense_of_rotation to be considered CW:
    - an interrupt (FALLING EDGE) happens on pin clk THEN an interrupt (FALLING EDGE) happens on pin dta
  
  - For the sense_of_rotation to be considered CCW:
    - an interrupt (FALLING EDGE) happens on pin dta THEN an interrupt (FALLING EDGE) happens on pin clk 
  

`The configuration is stored in the variable "state":`

- state = 1111 (initial value)
- 1st interrupt happens on pin clk : state = 0b0111 : the 1st 2 bits 01 represent the values of pin clk and pin dta (01 : pin clk = 0 and pin dta = 1) ; the 2nd 2 bits represent if the 1st interrupt is happening or the second interrupt is happening
- 2nd interrupt happens on pin dta : state = 0b0100 : the 2nd 2 bits represent that interrupt 2 is happening (00 : pin clk = pin dta = 0)
- if these 2 conditions are met => CW rotation


same logic for a CCW rotation:

- state = 1111 (initial value)
- 1st interrupt happens on pin dta : state = 0b1011 : the 1st 2 bits 10 represent the values of pin clk and pin dta (10 : pin clk = 1 and pin dta = 0) ; the 2nd 2 bits represent if the 1st interrupt is happening or the second interrupt is happening
- 2nd interrupt happens on pin dta : state = 0b0100 : the 2nd 2 bits represent that interrupt 2 is happening (00 : pin clk = pin dta = 0)
- if these 2 conditions are met => CCW rotation

this mecanism prevent wrong values due to the debounce of the encoder: only the values of state = 0b0100 or 0b1000 are taken 