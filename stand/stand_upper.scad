$fn=60;

union()
{
difference()
{minkowski () {cube([190,40,40],center=true);sphere(d=6);}
translate ([0,0,-10] )cube([190,40,21],center=true); 
 translate ([0,0,3] )cube([188,12.5,5],center=true);  
 translate ([0,0,0] )cube([183,1.75,50],center=true); 
translate ([0,0,-22] )cube([200,50,3],center=true); 
translate([95,4,-10]) rotate([0,90,0])cylinder(d=7,h=30,center=true);
translate([95,-4,-10]) rotate([0,90,0])cylinder(d=7,h=30,center=true);

translate ([95,0,-10] )cube([20,8,7],center=true); 
};
translate ([0,0,1] )cube([10,15,1],center=true);
translate ([40,0,1] )cube([10,15,1],center=true);
translate ([-40,0,1] )cube([10,15,1],center=true);
translate ([-80,0,1] )cube([10,15,1],center=true);
translate ([80,0,1] )cube([10,15,1],center=true);
};

