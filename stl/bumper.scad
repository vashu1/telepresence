d = 100;
th = 2;


outer = 225;
h = 50;
// 95 betw holes    22outside wheels
// h = 30    130 betw wheels 200 right outside

module hexagon(d, h) {
    difference() {
        cylinder(d=2*d, h=h, center=true);
        for (i = [0:5]) rotate([0,0,i*60]) translate([500+d/2,0,0]) cube([1000,1000,1000], center=true);
    }
}

translate([0,outer/6,0]) difference() {
    hexagon(d=outer+2*th, h=h);
    hexagon(d=outer, h=h);
    translate([-500,0,0]) cube([1000,1000,1000], center=true);
    translate([0,500-outer/6,0]) cube([1000,1000,1000], center=true);
}
difference() {
    translate([th/2,0,0]) cube([th,outer*5/6,h], center=true);
    translate([0,500,0]) cube([1000,1000,1000], center=true);
    for (i = [15,40,65]) translate([0,-i,0]) rotate([0,90,0]) cylinder(d=5,h=10*th, center=true);
}
translate([(130-20)/2/2, -th/2,0]) cube([(130-20)/2, th, h], center=true);

difference () {
    translate([0,outer/6,0]) intersection() {
        hexagon(d=outer+2*th, h=2*h);
        translate([0,0,-h/2+th/2]) cube([1000,1000,th], center=true);
    }
    translate([-500,0,0]) cube([1000,1000,1000], center=true);
    translate([0,500,0]) cube([1000,1000,1000], center=true);
    translate([100-35/2,-30/2,0]) cube([35+20, 20+10,1000], center=true);
}

//translate([0,th/2,0]) cube([200,th,h], center=true);
translate([100-35/2,-20/2,0]) cube([35, 20,h], center=true);
translate([100-35/2,65/2,0]) cube([35, 65,h], center=true);