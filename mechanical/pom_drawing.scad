l = 152.4;
h = 50.8;
gap = 1;

// triangle
polygon(points = [[0,0], [l * cos(60), l * sin(60)], [l, 0], [0, 0]]);

// side 1
translate([-gap, 0]) {
    rotate(60) {
        square(size = [l, h]);
    }
}

// side 2
translate([l, -gap]) {
    rotate(180) {
        square(size = [l, h]);
    }
}

translate([l + gap + h * cos(30), h * sin(30)]) {
    rotate(120) {
        square(size = [l, h]);
    }
}