#pragma once

#include "../support/pointers/RefCounted.h"
#include "PowerDiagramCell.h"
#include <functional>

/**
*/
class PowerDiagram : public RefCounted {
public:
    using        Cell            = PowerDiagramCell;
    using        TF              = Cell::TF;

    virtual     ~PowerDiagram    ();

    virtual void for_each_cell   ( const std::function<void( Cell &cell, PI num_thread )> &f ) = 0;

    PI           max_nb_threads; ///<
};
