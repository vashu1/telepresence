D_BIG = 30;
CUT_BIG = 25;
H_BIG = 10;

WALL_TH = 3;

WALL_H = 50;

HOLE_CIRCLE_R = 14;
HOLE_D = 3.5;
AXLE_D = 6;

module handle() {
    difference() {
        cylinder(d=D_BIG, h=H_BIG, center=true);
        for(i = [-1, +1]) translate([i*(500+CUT_BIG/2),0,0]) cube([1000,1000,1000], center=true);
    }
}

// for (i = [0:5]) {rotate([0,0,60*i]) translate([14,0,0]) cylinder(d=3.5, h=100, center=true);}
translate([CUT_BIG/2+WALL_TH/2,0,1.5*WALL_H/2]) difference() {
    cube([WALL_TH, 3*HOLE_CIRCLE_R, 1.5*WALL_H], center=true);
    translate([0,0,WALL_H/2]) rotate([0,90,0]) cylinder(d=AXLE_D, h=1000, center=true);
    for (i = [0:5]) translate([0,0,WALL_H/2]) rotate([i*60,0,0]) translate([0,0,HOLE_CIRCLE_R]) rotate([0,90,0]) cylinder(d=HOLE_D, h=1000, center=true);
};
translate([0,0,H_BIG/2]) difference() {
    cylinder(d=D_BIG+2*WALL_TH, h=H_BIG, center=true);
    handle();
}