$fn = 30;

mic_d = 95;
height = 10;
wall_thickness = 3;

difference() {
    cylinder(h=10, d1=mic_d+16+2*wall_thickness, d2=mic_d+2*wall_thickness, center=true);
    translate([0,0,wall_thickness]) cylinder(h=10, d1=mic_d+16, d2=mic_d, center=true);
    difference() {
        translate([50+35,0,0]) rotate([0,0,45]) cube([100,100,100], center=true);
        cube([80,80,80], center=true);
    }
    translate([60/2,0,60/2-wall_thickness/2]) cube([60,60,60], center=true);
    for (i = [0:3-1]) rotate([0,0,i*120+30]) translate([0,10,-3.5]) cylinder(h=4, d1=1, d2=6, center=true);
    for (i = [0:6-1]) rotate([0,0,i*60]) translate([0,25,-3.5]) cylinder(h=4, d1=1, d2=6, center=true);
    for (i = [0:12-1-1]) rotate([0,0,i*30-60]) translate([0,40,-3.5]) cylinder(h=4, d1=1, d2=6, center=true);
}


