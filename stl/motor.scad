// model for 28BYJ-48 stepper and it's mount
// telepresence/stl/28BYJ-48-dimensions.png

mount_wall_th = 2;

cylinder_height = 19;
cylinder_d = 29;
windage = 0.5;
mount_hole_distance = 35;
mount_plank_width = 7;
holes_d = 4;
axis_offset = -8;
shaft_angle = 45;
shaft_len = 10;

module stepper() {
    // main body cylinder
    cylinder(d=cylinder_d+windage, h=cylinder_height, center=true, $fn=90);
    // shaft exit
    translate([0,axis_offset,cylinder_height/2+1.5/2]) cylinder(d=9, h=1.5, center=true, $fn=24);
    // shaft
    translate([0,axis_offset,cylinder_height/2+ shaft_len/2]) rotate([0,0,shaft_angle]) difference() {
        cylinder(d=5, h=10, center=true, $fn=12);
        // flat parts of axis
        translate([-500-3/2,0,shaft_len/2-6/2]) cube([1000,1000,6], center=true);
        translate([+500+3/2,0,shaft_len/2-6/2]) cube([1000,1000,6], center=true);
    }
    difference() {
        union() {
            // fastening plank
            translate([0,0,cylinder_height/2-1/2]) cube([35,mount_plank_width,1], center=true);
            // round ends of fastening plank
            translate([+35/2,0,cylinder_height/2-1/2]) cylinder(d=mount_plank_width, h=1, center=true);
            translate([-35/2,0,cylinder_height/2-1/2]) cylinder(d=mount_plank_width, h=1, center=true);
        }
        // holes to fasten motor
        translate([+35/2,0,0]) cylinder(d=holes_d, h=1000, center=true);
        translate([-35/2,0,0]) cylinder(d=holes_d, h=1000, center=true);
    }
    // control box
    translate([0,17/2,1.5/2]) cube([16,17,cylinder_height-1.5], center=true);
    translate([0,17/2,0]) cube([16+3,17-2*3,cylinder_height-3], center=true);
    intersection() {
        cylinder(d=cylinder_d+2, h=cylinder_height-1.5, center=true);
        translate([0,10,-5]) cube([16+3,cylinder_d+2,cylinder_height], center=true);
    }
}

module mount() {
    difference() {
        union() {
            translate([0,0,-1]) cylinder(d=cylinder_d+2*mount_wall_th, h=cylinder_height+mount_wall_th, center=true);
            translate([0,17/2,-2/2]) cube([17+4, 16+2*mount_wall_th,cylinder_height+mount_wall_th],center=true);
            //holes
            translate([0,0,-2/2]) cube([cylinder_d+12, 7, cylinder_height+2] , center=true);
            
            /*
            // projectile launcher connector
            translate([-2/2-3,-cylinder_d/2-10,-2/2]) cube([2,20,cylinder_height+2], center=true);
            translate([+2/2+3,-cylinder_d/2-15/2,-2/2]) cube([2,15,cylinder_height+2], center=true);
            // calibrate barrel angle, 3 plate thickness
            translate([0,axis_offset-15-2/2-3,-2/2]) cube([10,2,cylinder_height+2], center=true);
            //
            translate([0,axis_offset-14,-2/2]) cube([10,2,cylinder_height+2], center=true);
            */
        }
        stepper();
        // cut hole for stepper wires
        translate([0,cylinder_d/2,cylinder_height/2-5/2]) cube([10,10,5], center=true);
        //holes
        translate([+35/2,0,0]) cylinder(d=holes_d*0.75, h=100, center=true);
        translate([-35/2,0,0]) cylinder(d=holes_d*0.75, h=100, center=true);
        // drop parts that would go into stepper holes 
        translate([0,0,19/2-1/2]) cube([mount_hole_distance+mount_plank_width,7,1], center=true);
    }
}

*stepper();

*mount();

// gear that holds the barrel
*translate([0, axis_offset, 2 + cylinder_height/2 + 16/2]) cylinder(d=16, h=16, center=true);
