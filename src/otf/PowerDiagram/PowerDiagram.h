#pragma once

#include "../support/pointers/RefCounted.h"
#include "../support/containers/Vec.h"
#include "PowerDiagramCell.h"
#include <functional>

/**
*/
class PowerDiagram : public RefCounted {
public:
    using        Cell            = PowerDiagramCell;
    using        TF              = Cell::TF;

    virtual     ~PowerDiagram    ();

    virtual void for_each_point  ( const std::function<void( PI num_point, const Vec<PI> &connected_items, PI num_thread )> &f ) = 0;
    virtual void for_each_cell   ( const std::function<void( Cell &cell, PI num_thread )> &f ) = 0;

    PI           max_nb_threads; ///<
};
