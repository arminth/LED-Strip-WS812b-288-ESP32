$length_of_glass = 180;
$thickness_of_glas=1.35; //max 6-7mm
$play=0.4;

$fn=60;
module upper_stand()

{
union()
{
difference()
{minkowski () {cube([$length_of_glass+10,40,40],center=true);sphere(d=6);}
translate ([0,0,-10] )cube([$length_of_glass+10,40,21],center=true); 
 translate ([0,0,3] )cube([$length_of_glass+8,12.5,5],center=true);  
 translate ([0,0,0] )cube([$length_of_glass+3,$thickness_of_glas+$play,50],center=true); 
translate ([0,0,-22] )cube([$length_of_glass+20,50,3],center=true); 
translate([95,4,-10]) rotate([0,90,0])cylinder(d=7,h=30,center=true);
translate([95,-4,-10]) rotate([0,90,0])cylinder(d=7,h=30,center=true);

translate ([$length_of_glass/2+5,0,-10] )cube([20,8,7],center=true); 
};
translate ([0,0,1] )cube([10,15,1],center=true);
translate ([40,0,1] )cube([10,15,1],center=true);
translate ([-40,0,1] )cube([10,15,1],center=true);
translate ([-80,0,1] )cube([10,15,1],center=true);
translate ([80,0,1] )cube([10,15,1],center=true);
};
};

module lid()
{minkowski () {cube([$length_of_glass+10,40,1],center=true);cylinder(d=6, h=0.000001);}
translate([0,0,1.25]) cube([$length_of_glass+9.6,39.6,2],center=true);
};



lid();
translate([0,0,21]) upper_stand();