echo creating debug configuration
echo make distclean
make distclean
echo autoreconf -i
autoreconf -i
echo making configuration
./configure AR=avr-ar --host=avr 