include <motor.scad>

wall_th = 2;

cut_w = 22;
selfie_d = 32;
selfie_height = 20; // TODO

translate([0,0,cylinder_height/2 + mount_wall_th]) mount();
//translate([0,0,cylinder_height/2 + mount_wall_th]) stepper();
difference() {
    union() {
        //translate([0,-11-1,(cut_w/2+wall_th)/2]) cube([selfie_height*1.6, 10, cut_w/2+wall_th], center=true);
        translate([0,-33,(cut_w/2+wall_th)/2]) cube([selfie_height, 40, cut_w/2+wall_th], center=true);
    }
    translate([0,0,cylinder_height/2 + mount_wall_th])stepper();
    translate([0,-40+2,selfie_height/2+wall_th]) rotate([90,90,90]) difference() {
        union() {
            cylinder(d=selfie_d+windage, h=selfie_height, center=true);
            //translate([0,0,1/2+2]) cylinder(d=selfie_d+2*windage,h=1, center=true);
        }
        translate([+500+cut_w/2,0,0]) cube([1000,1000,1000], center=true);
        translate([-500-cut_w/2,0,0]) cube([1000,1000,1000], center=true);
    }
    translate([0,0,0]) cylinder(d=hole_d*0.75, h=cut_w/2+wall_th, center=true); 
}

/*
$fn=30;
// module name ( parameters ) { actions }
translate([0,0,6/2]) difference() {
    cylinder(d1=10, d2=10-4, h=6, center=true);
    translate([+500+3,0,0]) cube([1000,1000,1000], center=true);
    translate([-500-3,0,0]) cube([1000,1000,1000], center=true);
}
translate([0,0,-6/2]) difference() {
    union() {
        cylinder(d1=10, d2=10, h=6, center=true);
        translate([0,0,1/2+2]) cylinder(d=10.5,h=1, center=true);
    }
    translate([+500+3,0,0]) cube([1000,1000,1000], center=true);
    translate([-500-3,0,0]) cube([1000,1000,1000], center=true);
}
*/