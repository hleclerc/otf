#pragma once

#include "containers/Vec.h"
#include <sstream>
#include <map>

/***/
class VtkOutput {
public:
    enum {          VtkPoint        = 1   };
    enum {          VtkLine         = 4   };
    enum {          VtkPoly         = 7   };
    enum {          VtkTetra        = 10  };

    using           TF              = double;
    using           Pt              = Vec<TF>;
    using           FieldMap        = std::map<String,Vec<TF>>;

    void            save            ( std::string filename ) const;
    void            save            ( std::ostream &os ) const;

    void            add_triangle    ( std::array<Pt,3> pts );
    void            add_pyramid     ( std::array<Pt,5> pts );
    void            add_wedge       ( std::array<Pt,6> pts );
    void            add_tetra       ( std::array<Pt,4> pts );
    void            add_hexa        ( std::array<Pt,8> pts );
    void            add_quad        ( std::array<Pt,4> pts );

    void            add_polygon     ( const Vec<Pt> &pts, const std::map<String,Vec<TF>> &point_data = {}, const std::map<String,TF> &cell_data = {} );
    void            add_line        ( const Vec<Pt> &pts );

    void            add_point       ( Pt pts );

    void            add_item        ( const Pt *pts_data, PI pts_size, PI vtk_type, const std::map<String,Vec<TF>> &point_data = {}, const std::map<String,TF> &cell_data = {} );

    FieldMap        point_fields;   ///<
    FieldMap        cell_fields;    ///<
    Vec<PI>         cell_items;     ///<
    Vec<PI>         cell_types;     ///<
    Vec<Pt>         points;         ///<
};

