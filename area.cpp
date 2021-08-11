#include "area.h"

void Area::Node_::set(const std::bitset<4> connections)
{
    if((connections[3]) && (this->coordinates_.y > 0))
    {
        this->up_ = &this->offset(UP);
        this->up_->down_ = this;
    }
    else
    {
        this->up_ = nullptr;
        if(this->coordinates_.y > 0)
        {
            this->offset(UP).down_ = nullptr;
        }
    }

    if((connections[2]) && (this->coordinates_.y < this->area_->size_.y - 1))
    {
        this->down_ = &this->offset(DOWN);
        this->down_->up_ = this;
    }
    else
    {
        this->down_ = nullptr;
        if(this->coordinates_.y < this->area_->size_.y - 1)
        {
             this->offset(DOWN).up_ = nullptr;
        }
    }

    if((connections[1]) && (this->coordinates_.x > 0))
    {
        this->left_ = &this->offset(LEFT);
        this->left_->right_ = this;
    }
    else
    {
        this->left_ = nullptr;
        if(this->coordinates_.x > 0)
        {
           this->offset(LEFT).right_ = nullptr;
        }
    }

    if((connections[0]) && (this->coordinates_.x < this->area_->size_.x - 1))
    {
        this->right_ = &this->offset(RIGHT);
        this->right_->left_ = this;
    }
    else
    {
        this->right_ = nullptr;
        if(this->coordinates_.x < this->area_->size_.x - 1)
        {
            this->offset(RIGHT).left_ = nullptr;
        }
    }
}

void Area::Node_::add(const std::bitset<4> connection)
{
    if((connection[3]) && (this->coordinates_.y > 0))
    {
        this->up_ = &this->offset(UP);
        this->up_->down_ = this;

        return;
    }

    if((connection[2]) && (this->coordinates_.y < this->area_->size_.y - 1))
    {
        this->down_ = &this->offset(DOWN);
        this->down_->up_ = this;

        return;
    }

    if((connection[1]) && (this->coordinates_.x > 0))
    {
        this->left_ = &this->offset(LEFT);
        this->left_->right_ = this;

        return;
    }

    if((connection[0]) && (this->coordinates_.x < this->area_->size_.x - 1))
    {
        this->right_ = &this->offset(RIGHT);
        this->right_->left_ = this;

        return;
    }
}

void Area::Node_::remove(const std::bitset<4> connection)
{
    if((connection[3]) && (this->coordinates_.y > 0))
    {
        this->up_ = nullptr;
        this->offset(UP).down_ = nullptr;

        return;
    }

    if((connection[2]) && (this->coordinates_.y < this->area_->size_.y - 1))
    {
        this->down_ = nullptr;
        this->offset(DOWN).up_ = nullptr;

        return;
    }

    if((connection[1]) && (this->coordinates_.x > 0))
    {
        this->left_ = nullptr;
        this->offset(LEFT).right_ = nullptr;

        return;
    }

    if((connection[0]) && (this->coordinates_.x < this->area_->size_.x - 1))
    {
        this->right_ = nullptr;
        this->offset(RIGHT).left_ = nullptr;

        return;
    }
}

void Area::resize(const Point& size)
{
    if((size.x < 0) || (size.y < 0))
    {
        throw Area::size_error(size);
    }

    this->~Area();

    this->grid_ = new Node_* [size.x];
    for(int x = 0; x < size.x; ++x)
    {
        this->grid_[x] = new Node_ [size.y];
        for(int y = 0; y < size.y; ++y)
        {
            this->grid_[x][y].coordinates_ = Point(x, y);
            this->grid_[x][y].area_ = this;
        }
    }
    this->size_ = size;
}

Path Area::path(const Point& begin, const Point& end) const
{
    this->operator[](begin);
    this->operator[](end);

    std::list<Node_*> reachable;
    Node_* current_node = nullptr;

    Node_* end_node = &this->grid_[end.x][end.y];

    Path path;

    for(int y = 0; y < this->size_.y; ++y)
    {
        for(int x = 0; x < this->size_.x; ++x)
        {
            this->grid_[x][y].previous_ = nullptr;
            this->grid_[x][y].cost_ = this->grid_[x][y].value_;
        }
    }

    this->grid_[begin.x][begin.y].previous_ = &this->grid_[begin.x][begin.y];
    reachable.push_back(&this->grid_[begin.x][begin.y]);

    while((end_node->previous_ == nullptr) && (!reachable.empty()))
    {
        current_node = reachable.front();

        for(std::list <Node_*>::iterator it = reachable.begin(); it != reachable.end(); ++it)
        {
            if(it.operator*()->capacity_(end_node) < current_node->capacity_(end_node))
            {
                current_node = it.operator*();
            }
        }
        reachable.remove(current_node);

        if((current_node->up_ != nullptr) && (current_node->up_->previous_ == nullptr))
        {
            current_node->up_->previous_ = current_node;
            current_node->up_->cost_ += current_node->cost_;
            reachable.push_back(current_node->up_);
        }

        if((current_node->down_ != nullptr) && (current_node->down_->previous_ == nullptr))
        {
            current_node->down_->previous_ = current_node;
            current_node->down_->cost_ += current_node->cost_;
            reachable.push_back(current_node->down_);
        }

        if((current_node->left_ != nullptr) && (current_node->left_->previous_ == nullptr))
        {
            current_node->left_->previous_ = current_node;
            current_node->left_->cost_ += current_node->cost_;
            reachable.push_back(current_node->left_);
        }

        if((current_node->right_ != nullptr) && (current_node->right_->previous_ == nullptr))
        {
            current_node->right_->previous_ = current_node;
            current_node->right_->cost_ += current_node->cost_;
            reachable.push_back(current_node->right_);
        }
    }

    if(end_node->previous_ != nullptr)
    {
        while(current_node->previous_ != current_node)
        {
            path.push_back(current_node->coordinates_);
            current_node = current_node->previous_;
        }
        path.push_back(current_node->coordinates_);

        std::reverse(path.begin(), path.end());
    }

    return path;
}

std::ostream& operator<<(std::ostream& os, const Area& area)
{
    for(int y = 0; y < area.size_.y; ++y)
    {
        for(int x = 0; x < area.size_.x; ++x)
        {
            os << area.grid_[x][y] << '\t';
        }
        os << std::endl;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Area& area)
{
    for(int y = 0; y < area.size_.y; ++y)
    {
        for(int x = 0; x < area.size_.x; ++x)
        {
            is >> area.grid_[x][y];
        }
    }

    return is;
}

Area::~Area()
{
    for(int x = 0; x < this->size_.x; ++x)
    {
        delete [] this->grid_[x];
    }
    delete []  this->grid_;

    this->size_ = NULLPOINT;
}
