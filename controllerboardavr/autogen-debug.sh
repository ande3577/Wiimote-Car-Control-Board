echo creating debug configuration
echo make distclean
make distclean
echo autoreconf -i
autoreconf -i
echo making configuration
./configure AR=avr-ar --build=x86_64-unknown-linux-gnu --host=avr 
