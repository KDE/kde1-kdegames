POVRAY=x-povray  # Must be Povray 3.0

all: ship.pov
	$(POVRAY) ship.ini

clean:
	rm -f *~ *.png *.pgm

realclean: clean
	rm -f *.ppm *.pbm
