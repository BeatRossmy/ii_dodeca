# ii_dodeca
Turning the dodeca eurorack module into a ii follower, which can be controlled via monome crow.

The [dodeca](https://github.com/MagpieModular/Dodeca) by Magpie Modular and Neutron Sound is a eurorack module intended to work together as an eurorack extension for the elektron octatrack.
This alternative firmware turns it into an [ii](https://monome.org/docs/modular/ii) (i2c) follower, so that the module can be controlled by ii leaders such as the [monome crow](https://monome.org/docs/crow/).

This firmware takes inspiration from the [ii expanders](https://github.com/bpcmusic/telex) originally designed for the monome teletype, but trys to remian the original functionality when controlled via the trs midi input.

## Hardware
No physical changes to the hardware have to be done, beside soldering three wires to unused i2c pins of the teensy footprint found on the dodeca pcb.
These pins are not used by default, therefore this change is very straight forward.
- Identify pins 18 (SDA) and 19 (SCL) and GND.
- Solder the respective wires.
- Make sure that on your ii line there is at least on emodule which can pull up the i2c lines.
