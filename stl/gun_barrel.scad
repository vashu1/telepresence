
height = 33 + 33; // 33mm is block len, double that with projectile

rotate([180,0,0]) union() {
    difference() {
        // main body
        cylinder(d=100,h=height, center=true);
        // cut into decagon
        for (i = [0:9]) rotate([0,0,i*36]) translate([500+16,0,0]) cube([1000,1000,1000], center=true);
        // with step for fixing lego at side
        for (i = [0:9]) rotate([0,0,i*36]) translate([-500-14,500-3,0]) cube([1000,1000,1000], center=true);
        // laser holes
        for (i = [0:9]) rotate([0,0,i*36]) translate([11,0,0]) rotate([0,0,90]) cylinder(d=4.5,h=1000, center=true, $fn=6);
        // center hole
        cylinder(d1=16,d2=16.5,h=height, center=true, $fn=60);
    }
    *translate([12,0,0])cube([8,10,10],center=true);

    // stesp to fix lego's bottom
    for (i = [0:9]) rotate([0,0,i*36]) translate([15-0.5,-1.5,height/2-(height/2-1)/2]) cube([1,9,height/2-1], center=true);
}