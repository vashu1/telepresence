
TH = 2.4;
MIC_D_BOTTOM = 105;
MIC_D_UPPER = 95;
MIC_H = 9;

ELONGATION = 1.5;

ANGLE = 70;

difference() {
    rotate([ANGLE,0,0]) translate([0,ELONGATION*MIC_D_BOTTOM/2+TH,0]) difference() {
        translate([0,0,TH]) difference() {
            translate([0,0,(MIC_H+TH)/2-TH]) cube([MIC_D_BOTTOM + 2*TH,MIC_D_BOTTOM*ELONGATION+2*TH,MIC_H+TH], center=true);
            scale([1,ELONGATION,1]) cylinder(d=MIC_D_UPPER, h=MIC_H);
            scale([1,ELONGATION,1]) cylinder(d=MIC_D_BOTTOM, h=MIC_H-TH);
        }
        translate([0,500,0]) cube([1000,1000,1000], center=true);
    }
    translate([0,0,500+(MIC_D_BOTTOM*ELONGATION+2*TH)*sin(ANGLE)/2]) cube([1000,1000,1000],center=true);
}
translate([0,-(MIC_H+TH)*sin(ANGLE)/2,TH/2]) cube([MIC_D_BOTTOM+2*TH,MIC_D_BOTTOM*0.4,TH], center=true);
translate([0,-(MIC_H+TH)*sin(ANGLE)/2,(MIC_H+TH)*cos(ANGLE)/2]) cube([MIC_D_BOTTOM+2*TH,(MIC_H+TH)*sin(ANGLE),(MIC_H+TH)*cos(ANGLE)], center=true);

