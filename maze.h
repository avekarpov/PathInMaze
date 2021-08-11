#ifndef MAZE_H
#define MAZE_H

#include "area.h"

class Maze : public Area
{
public:
    void generate();

    void draw();
    void draw(const Point& point);
    void draw(const Path& path, const unsigned long int before);

private:
    void spread_(const Point& point);
};

#endif // MAZE_H
