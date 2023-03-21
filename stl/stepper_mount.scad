include <motor.scad>

union() {
    mount();
    // projectile launcher connector
    translate([-2/2-3,-29/2-10,-2/2]) cube([2,20,19+2], center=true);
    translate([+2/2+3,-29/2-15/2,-2/2]) cube([2,15,19+2], center=true);
    // calibrate barrel angle, 3 plate thickness
    translate([0,-8-15-2/2-3,-2/2]) cube([10,2,19+2], center=true);
    //
    translate([0,-8-14,-2/2]) cube([10,2,19+2], center=true);
    // inclined projectile trigger, 3 is shoot plate thickness
    translate([0,-3/2-8-15-11/2-3,-2/2+30/2]) rotate([0,0,30]) cube([16, 3, 19+2+30],center=true);
}
// gear that holds the barrel
*translate([0, -8, 2 + 19/2 + 16/2]) cylinder(d=16, h=16, center=true);

//stepper();

// barrel
//*translate([0,-8,height/2 + 19/2 + 2 + 5]) rotate([180,0,360/10/2]) include <gun_barrel.scad>

// extra box to cool trigger layer
*translate([0,-3/2-8-15-11/2-3-10,-2/2+30/2]) rotate([0,0,30]) cube([16, 1, 19+2+30],center=true);

// check trigger distance
*translate([0,-10/2-8-14,0]) cube([10,10,100], center=true);