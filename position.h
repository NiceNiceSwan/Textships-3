#ifndef POSITION_H
#define POSITION_H

struct Position
{
    double x;
    double y;

    Position() {};
    Position(double position_x, double position_y)
    {
        x = position_x;
        y = position_y;
    }

    double distance(Position other_position);
    static double distance(Position position_1, Position position_2);

};


#endif