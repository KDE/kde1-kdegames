#include "colors.inc"
#include "textures.inc"
#include "finish.inc"

global_settings {
	assumed_gamma 2.0
}

camera {
	orthographic
	up z*2.5
	right x*2.5
	location <0,50,-100>
	look_at <0,0,0>
}

#declare SideView =
camera {
	up y
	right x
	location <-2,1,-1>
	look_at <0,0,0>
}

//camera { SideView }

light_source {
	<-100,250,100>
	color White
}

// Generate all rotations
union {
	sphere {
		<0,0,0>,1
		pigment { White_Marble }
		finish {
			Dull
			ambient 0.4
		}
	}

	rotate <360*clock,360*clock,30>
}

