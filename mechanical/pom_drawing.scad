l = 152.4;
h = 50.8;

// Triangle
polygon(points = [[0,0], [l * cos(60), l * sin(60)], [l, 0], [0, 0]]);

// Bottom side
polygon(points = [[0, -h], [l, -h], [l, 0], [0, 0]]);

// Right side
polygon(points = [
    [l, 0],
    [h*cos(30) + l, h*sin(30)],
    [l*cos(60) + h*cos(30), l*sin(60) + h*sin(30)],
    [l*cos(60), l*sin(60)]
]);

// Left side
polygon(points = [
    [l*cos(60), l*sin(60)],
    [l*cos(60) - h*cos(30), l*sin(60) + h*sin(30)],
    [h*cos(150), h*sin(150)],
    [0, 0]
]);
