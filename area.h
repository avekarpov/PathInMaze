#ifndef AREA_H
#define AREA_H

#include <bitset>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <exception>

class Point
{
public:
    int x, y;

    inline Point (const int x = 0, const int y = 0)
    {
        this->x = x;
        this->y = y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point)
    {
        os << point.x << ' ' << point.y;

        return os;
    }
    friend std::istream& operator>>(std::istream& is, Point& point)
    {
        is >> point.x >>point.y;

        return is;
    }

    inline bool operator==(const Point& point) const
    {
        return ((this->x == point.x) && (this->y == point.y));
    }
    inline bool operator!=(const Point& point) const
    {
        return ((this->x != point.x) || (this->y != point.y));
    }

    inline const Point& operator=(const Point& point)
    {
        this->x = point.x;
        this->y = point.y;

        return *this;
    }

    inline const Point operator+(const Point& point) const
    {
        return {this->x + point.x, this->y + point.y};
    }
    inline const Point& operator+=(const Point& point)
    {
        this->x += point.x;
        this->y += point.y;

        return *this;
    }

    inline const Point operator-(const Point& point) const
    {
        return {this->x - point.x, this->y - point.y};
    }
    inline const Point& operator-=(const Point& point)
    {
        this->x -= point.x;
        this->y -= point.y;

        return *this;
    }

    inline const Point operator*(const int number) const
    {
        return {this->x * number, this->y * number};
    }
    inline const Point& operator*=(const int number)
    {
        this->x *= number;
        this->y *= number;

        return *this;
    }

    inline const Point operator/(const int number) const
    {
        return {this->x / number, this->y / number};
    }
    inline const Point operator/=(const int number)
    {
        this->x /= number;
        this->y /= number;

        return *this;
    }

    inline double distance(const Point& point) const
    {
        return sqrt((pow(this->x - point.x, 2) + pow(this->y - point.y, 2)));
    }

    inline int taxicab_distance(const Point& point) const
    {
        return (abs(this->x - point.x) + abs(this->y - point.y));
    }

    inline const Point offset(const Point& point) const
    {
        return Point(this->x + point.x, this->y + point.y);
    }

    static std::string to_string(const Point& point)
    {
        return std::to_string(point.x) + ' ' + std::to_string(point.y);
    }
};

typedef std::vector<Point> Path;

const Point UP(0, -1);
const Point DOWN(0, 1);
const Point LEFT(-1, 0);
const Point RIGHT(1, 0);
const Point NULLPOINT(0, 0);

class Area
{
private:
    class Node_
    {
    public:
        void set(const std::bitset <4> connections);
        void add(const std::bitset <4> connection);
        void remove(const std::bitset<4> connection);

        unsigned long int get() const
        {
            std::bitset<4> connections;

            connections[3] = (this->up_ != nullptr);
            connections[2] = (this->down_ != nullptr);
            connections[1] = (this->left_ != nullptr);
            connections[0] = (this->right_ != nullptr);

            return connections.to_ulong();
        }

        inline Node_& offset(const Point& point) const
        {
            return this->area_->operator[](this->coordinates_.offset(point));
        }

        inline const Point& coordinates() const
        {
            return this->coordinates_;
        }

        inline bool up() const
        {
            return this->up_ != nullptr;
        }
        inline bool down() const
        {
            return this->down_ != nullptr;
        }
        inline bool left() const
        {
            return this->left_ != nullptr;
        }
        inline bool right() const
        {
            return this->right_ != nullptr;
        }
        inline bool some() const
        {
            return ((this->up_ != nullptr) || (this->down_ != nullptr) || (this->left_ != nullptr) || (this->right_ != nullptr));
        }

        inline int& value()
        {
            return this->value_;
        }

        friend std::ostream& operator<<(std::ostream& os, const Node_& node)
        {
            os << node.get() << ' ' << node.value_;

            return os;
        }
        friend std::istream& operator>>(std::istream& is, Node_& node)
        {
            unsigned short int connections;

            int value;

            is >> connections >> value;

            node.set(connections);
            node.value_ = value;

            return is;
        }

    private:
        friend class Area;

        Point coordinates_;

        Node_* up_ = nullptr, * down_ = nullptr, * left_ = nullptr, * right_ = nullptr;
        int value_;

        Node_* previous_;
        int cost_;

        Area* area_;

        inline int capacity_(const Node_* node) const
        {
            return (this->cost_ + this->coordinates().taxicab_distance(node->coordinates_));
        }

        Node_& operator=(const Node_&) = delete;

    };

public:
    class size_error : public std::exception
    {
    public:
        size_error(const Point& size)
        {
            this->size_ = size;
        }

        const char * what() const noexcept
        {
            return "size_error";
        }

        const Point& what_size() const
        {
            return this->size_;
        }

    private:
        Point size_;

    };
    class out_of_area_error : public std::exception
    {
    public:
        out_of_area_error(const Point& point)
        {
            this->point_ = point;
        }

        const char * what() const noexcept
        {
            return "out_of_area_error";
        }

        const Point& what_point()
        {
            return this->point_;
        }        

    private:
        Point point_;

    };

    void resize(const Point& size);

    inline Node_& operator[](const Point& point) const
    {
        if((point.x < 0) || (point.y < 0) || (this->size_.x <= point.x) || (this->size_.y <= point.y))
        {
            throw Area::out_of_area_error(point);
        }

        return this->grid_[point.x][point.y];
    }

    Path path(const Point& begin, const Point& end) const;

    inline const Point& size() const
    {
        return this->size_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Area& area);
    friend std::istream& operator>>(std::istream& is, Area& area);

    virtual ~Area();

private:
    Node_** grid_ = nullptr;
    Point size_ = NULLPOINT;

};

#endif // AREA_H
