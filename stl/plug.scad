$fn = 90;

outer_d = 36-0.5;
inner_d = 20+0.2;
height = 20;

difference() {
    union() {
        difference() {
            union() {
                translate([0,0,-height/4]) cylinder(d=outer_d, h=height*0.25, center=true);
                translate([0,0,height/4]) cylinder(d1=outer_d, d2=outer_d*0.95, h=height*0.75, center=true);
            }
            difference() {
                cylinder(d=inner_d, h=2*height, center=true);
                
            }
        }
        //translate([+inner_d/2,0,height*0.125]) cube([2,2,height], center=true);
        translate([-inner_d/2,0,height*0.125]) cube([1.2,1.2,height], center=true);
    }
    difference() {
        translate([500,0,0]) cube([1000,1000,1000], center=true);
        translate([0,+(outer_d+inner_d)/4,0]) cube([1.2,1.2,1000], center=true);
    }
    translate([0,-(outer_d+inner_d)/4,0]) cube([2,2,1000], center=true);
}
