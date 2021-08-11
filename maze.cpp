#include "maze.h"

#include <stack>

void Maze::spread_(const Point& point)
{
    char passiable_connections[4];
    int count_passiable_connections;

    do
    {
        count_passiable_connections = 0;

        if((point.y > 0) && !this->operator[](point.offset(UP)).some())
        {
            passiable_connections[count_passiable_connections] = 'U';
            ++count_passiable_connections;
        }

        if((point.y < this->size().y - 1) && !this->operator[](point.offset(DOWN)).some())
        {
            passiable_connections[count_passiable_connections] = 'D';
            ++count_passiable_connections;
        }

        if((point.x > 0) && !this->operator[](point.offset(LEFT)).some())
        {
            passiable_connections[count_passiable_connections] = 'L';
            ++count_passiable_connections;
        }

        if((point.x < this->size().x - 1) && !this->operator[](point.offset(RIGHT)).some())
        {
            passiable_connections[count_passiable_connections] = 'R';
            ++count_passiable_connections;
        }

        if(count_passiable_connections)
        {
            switch(passiable_connections[rand() % count_passiable_connections])
            {
            case 'U' : {this->operator[](point).add(8); this->spread_(point.offset(UP)); break;}
            case 'D' : {this->operator[](point).add(4); this->spread_(point.offset(DOWN)); break;}
            case 'L' : {this->operator[](point).add(2); this->spread_(point.offset(LEFT)); break;}
            case 'R' : {this->operator[](point).add(1); this->spread_(point.offset(RIGHT)); break;}
            }
        }
    }
    while(count_passiable_connections);
}

void Maze::generate()
{
    srand(time(0));
    //srand(0);

    this->operator[](NULLPOINT);

    for(Point p = NULLPOINT; p.y < this->size().y; ++p.y)
    {
        for(p.x = 0; p.x < this->size().x; ++p.x)
        {
            this->operator[](p).value() = 1;
            this->operator[](p).set(0);
        }
    }

    this->spread_(Point(rand() % this->size().x, rand() % this->size().y));
}

void Maze::draw()
{
    system("clear");

    if(this->size().operator!=(NULLPOINT))
    {
        std::cout << '_';
        for(int x = 0; x < this->size().x; ++x)
        {
            std::cout << "__";
        }
        std::cout << std::endl;

        for(Point p  = NULLPOINT; p.y < this->size().y; ++p.y)
        {
            std::cout << '|';
            for(p.x = 0; p.x < this->size().x; ++p.x)
            {
                if(this->operator[](p).down())
                {
                    std::cout << ' ';
                }
                else
                {
                    std::cout << '_';
                }

                if(this->operator[](p).right())
                {
                    std::cout << '_';
                }
                else
                {
                    std::cout << '|';
                }
            }
            std::cout << std::endl;
        }
    }
}

void Maze::draw(const Point &point)
{
    system("clear");

    if(this->size().operator!=(NULLPOINT))
    {
        std::cout << '_';
        for(int x = 0; x < this->size().x; ++x)
        {
            std::cout << "__";
        }
        std::cout << std::endl;

        for(Point p = NULLPOINT; p.y < this->size().y; ++p.y)
        {
            std::cout << '|';
            for(p.x = 0; p.x < this->size().x; ++p.x)
            {
                if((p == point))
                {
                    std::cout << "\033[1;41m_\033[0m";
                }
                else if(this->operator[](p).down())
                {
                    std::cout << ' ';
                }
                else
                {
                    std::cout << '_';
                }

                if(this->operator[](p).right())
                {
                    std::cout << '_';
                }
                else
                {
                    std::cout << '|';
                }
            }
            std::cout << std::endl;
        }
    }
}

void Maze::draw(const Path &path, const unsigned long int before)
{
    system("clear");

    if(this->size().operator!=(NULLPOINT))
    {
        std::cout << '_';
        for(int x = 0; x < this->size().x; ++x)
        {
            std::cout << "__";
        }
        std::cout << std::endl;

        for(Point p = NULLPOINT; p.y < this->size().y; ++p.y)
        {
            std::cout << '|';
            for(p.x = 0; p.x < this->size().x; ++p.x)
            {
                for(unsigned long int i = 0; i < before; ++i)
                {
                    if(path[i] == p)
                    {
                        if(this->operator[](p).down())
                        {
                            std::cout << "\033[1;41m \033[0m";
                        }
                        else
                        {
                            std::cout << "\033[1;41m_\033[0m";
                        }

                        if(this->operator[](p).right())
                        {
                            if(((i < path.size() - 1) && (path[i + 1] == path[i].offset(RIGHT))) || ((i > 0) && (path[i - 1] == path[i].offset(RIGHT))))
                            {
                                std::cout << "\033[1;41m_\033[0m";
                            }
                            else
                            {
                                std::cout << '_';
                            }
                        }
                        else
                        {
                            std::cout << '|';
                        }

                        goto next;
                    }
                }

                if(this->operator[](p).down())
                {
                    std::cout << ' ';
                }
                else
                {
                    std::cout << '_';
                }

                if(this->operator[](p).right())
                {
                    std::cout << '_';
                }
                else
                {
                    std::cout << '|';
                }
next:;
            }
            std::cout << std::endl;
        }
    }
}
