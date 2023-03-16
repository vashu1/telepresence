include <BOSL2/std.scad>
include <BOSL2/gears.scad>

hole = 5;
gear_thickness = 5.5;
teeth_cnt = 80;

intersection() {
    spur_gear(pitch=2, teeth=teeth_cnt, thickness=gear_thickness, shaft_diam=hole);
    union() {
        cylinder(d=45, h=2*gear_thickness, center=true);
        difference() {
            cylinder(d=100, h=2*gear_thickness, center=true);
            translate([500,0,0]) cube([1000,1000,1000], center=true);
            rotate([0,0,30]) translate([500,0,0]) cube([1000,1000,1000], center=true);
        }
    }
}
//cube([50,10,10], center=true);