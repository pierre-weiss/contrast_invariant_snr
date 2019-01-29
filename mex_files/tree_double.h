#ifndef TREE_H
#define TREE_H

#include "shape_double.h"

/// Tree of shapes.
struct LsTree {
    LsTree(const double* gray, int w, int h);
    ~LsTree();

    double* build_image() const;
    LsShape* smallest_shape(int x, int y);
    LsShape* smallest_shape(int i);

    int ncol, nrow; ///< Dimensions of image
    LsShape* shapes; ///< The array of shapes
    int iNbShapes; ///< The number of shapes

    /// For each pixel, the smallest shape containing it
    LsShape** smallestShape;
private:
    void flst_td(const double* gray); ///< Top-down algo
};

#endif
