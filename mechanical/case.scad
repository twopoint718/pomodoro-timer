/////////////////////////////////////////////////////////////////////
// Parameters

side_length = 153;  // 6"
wall_thickness = 5; // ~3/16"
side_height = 50.8 + wall_thickness; // 2" (from box bottom to lip)
lid_clearance = 30;
porthole_rad = 25.4;

/////////////////////////////////////////////////////////////////////
// Derived

lid_center = [
    0,
    lid_clearance + (side_length * sqrt(3))/3 // thx Pythagoras!
];

corners = [
    [0,0],
    [side_length * cos(60), side_length * sin(60)],
    [side_length, 0]
];

lid_corners = [
    [0, lid_clearance],
    [side_length * cos(60), side_length * sin(60) + lid_clearance],
    [-side_length/2, side_length * sin(60) + lid_clearance]
];

/////////////////////////////////////////////////////////////////////
// Features

// Box Bottom
union() {
    linear_extrude(height = side_height) {
        difference() {
            offset(r = wall_thickness) {
                // outer wall
                polygon(corners);
            }
            // inner wall
            polygon(corners);
        }    
    }
    linear_extrude(height = wall_thickness) {
        offset(r = wall_thickness) {
            // outer wall
            polygon(corners);
        }
    }
}

// Box Lid
difference() {
    linear_extrude(height = wall_thickness) {
        offset(r = wall_thickness) {
            polygon(lid_corners);
        }
    }
    translate([0, 0, -1]) {
        linear_extrude(height = wall_thickness + 2) {
            translate(lid_center) {
                circle(r = porthole_rad);
            }
        }
    }
}
