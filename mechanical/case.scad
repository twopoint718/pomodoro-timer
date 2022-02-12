/////////////////////////////////////////////////////////////////////
// Parameters

side_length = 153;  // 6"
wall_thickness = 5; // ~3/16"
side_height = 50.8 + wall_thickness; // 2" (from box bottom to lip)
lid_clearance = 30;
porthole_rad = 25.4;
magnet_r = 5;
magnet_tab_r = magnet_r + wall_thickness;
magnet_tab_h = 10;

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
// Modules

module MagnetTab(pos) {
    translate(pos) {
        difference() {
            linear_extrude(height = magnet_tab_h + wall_thickness) {
                offset(r = wall_thickness) {
                    circle(r = magnet_r);
                }
            }
            translate([0, 0, -1]) {
                linear_extrude(height = magnet_tab_h) {
                    circle(r = magnet_r);
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////
// Features

// Box Bottom
union() {
    // Wall
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
    
    // Floor
    linear_extrude(height = wall_thickness) {
        offset(r = wall_thickness) {
            // outer wall
            polygon(corners);
        }
    }

    // Magnet Closure tab 1
    MagnetTab([
        (wall_thickness + magnet_r) * cos(30),
        (wall_thickness + magnet_r) * sin(30),
        side_height - wall_thickness - magnet_tab_h
    ]);

    // Magnet Closure tab 2
    MagnetTab([
        side_length/2,
        side_length * sin(60) - magnet_r - wall_thickness,
        side_height - wall_thickness - magnet_tab_h
    ]);
    
    // Magnet Closure tab 3
    MagnetTab([
        side_length - (wall_thickness + magnet_r) * cos(30),
        (wall_thickness + magnet_r) * sin(30),
        side_height - wall_thickness - magnet_tab_h
    ]);
}

// Box Lid
union() {
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

    // Label (25)
    translate([-side_length/2 * cos(60), lid_clearance + side_length/2 * sin(60), 0]) {
        linear_extrude(height = wall_thickness + 2) {
            rotate(-60) {
                text("25", halign = "center", valign = "center");
            }
        }
    }

    // Label (5)
    translate([side_length/2 * cos(60), lid_clearance + side_length/2 * sin(60), 0]) {
        linear_extrude(height = wall_thickness + 2) {
            rotate(60) {
                text("5", halign = "center", valign = "center");
            }
        }
    }

    // Label (15)
    translate([0, lid_clearance + side_length * sin(60), 0]) {
        linear_extrude(height = wall_thickness + 2) {
            rotate(180) {
                text("15", halign = "center", valign = "center");
            }
        }
    }
}