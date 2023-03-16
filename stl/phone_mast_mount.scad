$fn = 90;

height = 140 + 20;
cylinder_thickness = 3;
plate_thickness = cylinder_thickness;
hole_d = 36;
outer_d = hole_d+2*cylinder_thickness;
width = 130;
width2 = width*2/3;

difference() {
    //translate([25,0,25]) cube([50, 5, 50], center = true);
}

intersection() {
    difference() {
        union() {
            translate([0,0,height/2]) cylinder(d=outer_d, h=height, center=true);
            translate([0,0,cylinder_thickness/4]) cube([width,width2,plate_thickness], center = true);
            rotate([0,0,+60]) cube([plate_thickness, 1000, 1000], center=true);
            rotate([0,0,-60]) cube([plate_thickness, 1000, 1000], center=true);
            intersection() {
                cube([plate_thickness, 1000,1000], center=true);
                translate([0,0,height/2]) cylinder(d1=width2, d2=outer_d, h=height, center=true);
            };
        }
        cylinder(d=hole_d, h=1000, center=true);
    }
    translate([0,0,height/2]) cylinder(d1=width*1.1,d2=outer_d-plate_thickness*2/3,h=height, center=true);
}
*translate([outer_d/2+15+plate_thickness,0,0]) difference() {
    translate([0,0,30+plate_thickness/2]) cube([30+2*plate_thickness,30+2*plate_thickness,60+plate_thickness], center=true);
    translate([0,0,30-0.1]) cube([30,30,60], center=true);
}