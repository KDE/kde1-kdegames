POVRAY=x-povray  # Must be Povray 3.0

all: rock3.pov
	gforge -s 2 -m 128 -dim 2.6 -type mat -na rock.mat
	orb rock.mat rock.inc 0.7 5
	rm rock.mat
	$(POVRAY) rock3.ini

clean:
	rm -f *~ *.png *.pgm

realclean: clean
	rm -f *.ppm *.pbm
