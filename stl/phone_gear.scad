include <BOSL2/std.scad>
include <BOSL2/gears.scad>

$fn = 60;

hole = 6;
gear_thickness = 5.5;
teeth_cnt = 90;


//spur_gear(mod=2, teeth=20, thickness=8, shaft_diam=5);

*difference() {
    spur_gear(pitch=6, teeth=12, thickness=gear_thickness, shaft_diam=hole);
    spur_gear(pitch=2, teeth=20, thickness=gear_thickness, shaft_diam=0);
}
*cube([50,10,10], center=true);
*cube([12,5,5], center=true);
difference() {
    intersection() {
        spur_gear(pitch=2, teeth=teeth_cnt, thickness=gear_thickness, shaft_diam=hole);
        union() {
            cylinder(d=48, h=2*gear_thickness, center=true);
            difference() {
                cylinder(d=100, h=2*gear_thickness, center=true);
                rotate([0,0,-2]) translate([500,0,0]) cube([1000,1000,1000], center=true);
                rotate([0,0,30-1]) translate([500,0,0]) cube([1000,1000,1000], center=true);
            };
        };
    };
    for (i = [0:5]) {rotate([0,0,60*i]) translate([14,0,0]) cylinder(d=3.5, h=100, center=true);}
}
translate([0,0,11/2]) difference() {
    cylinder(d=6+2*3, h=11, center=true);
    cylinder(d=6, h=100, center=true);
}
//cube([50,10,10], center=true);