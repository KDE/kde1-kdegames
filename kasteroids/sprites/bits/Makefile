POVRAY=x-povray  # Must be Povray 3.0

all: bits.pov
	$(POVRAY) bits.ini

clean:
	rm -f *~ *.pgm *.png

realclean: clean
	rm -f *.ppm *.pbm
