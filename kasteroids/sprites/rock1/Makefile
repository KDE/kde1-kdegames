POVRAY=x-povray  # Must be Povray 3.0

all: rock1.pov
	gforge -s 0 -m 256 -crat 1.9 1.5 -type mat -na rock.mat
	orb rock.mat rock.inc 0.7 6
	rm rock.mat
	$(POVRAY) rock1.ini

clean:
	rm -f *~ *.png *.pgm

realclean: clean
	rm -f *.ppm *.pbm
