#pragma once

#include "../support/types/TypeConfig.h"
#include <vector>
class VtkOutput;

/**
*/
class PowerDiagramCell {
public:
    using        TF                 = FP64;
   
    /**/         PowerDiagramCell   ();

    virtual void get_verts_and_faces( std::vector<double> &v, std::vector<int> &f, std::vector<double> &e ) = 0;
    virtual bool is_infinite        () const = 0;
    virtual void display            ( VtkOutput &vo, TF *offset = nullptr ) = 0;
    virtual TF   volume             () = 0;
   
    TF           weight;            ///<
    PI           num;               ///<
};

