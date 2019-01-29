#include "shape_double.h"
#include <cassert>

/// Return in the subtree of root pShape a shape that is not removed
static LsShape* ls_shape_of_subtree(LsShape* pShape) {
    LsShape* pShapeNotRemoved = 0;
    if(pShape == 0 || ! pShape->bIgnore)
        return pShape;
    for(pShape = pShape->child; pShape != 0; pShape = pShape->sibling)
        if((pShapeNotRemoved = ls_shape_of_subtree(pShape)) != 0)
            break;
    return pShapeNotRemoved;
}

/// To find the true parent, that is the greatest non removed ancestor
LsShape* LsShape::find_parent() {
    LsShape* pShape = this;
    do
        pShape = pShape->parent;
    while(pShape && pShape->bIgnore);
    return pShape;
}

/// Find the first child, taking into account that some shapes are removed
LsShape* LsShape::find_child() {
    LsShape* pShapeNotRemoved = 0;
    for(LsShape* pShape = child; pShape; pShape = pShape->sibling)
        if((pShapeNotRemoved = ls_shape_of_subtree(pShape)) != 0)
            break;
    return pShapeNotRemoved;  
}

/// Find next sibling, taking into account that some shapes are removed
LsShape* LsShape::find_sibling() {
    LsShape *pShape1 = 0, *pShape2 = 0;
    // First look at the siblings in the original tree
    for(pShape1 = sibling; pShape1 != 0; pShape1 = pShape1->sibling)
        if((pShape2 = ls_shape_of_subtree(pShape1)) != 0)
            return pShape2;
    if(parent == 0 || ! parent->bIgnore)
        return 0; // Parent in original tree is also parent in true tree: done
    // Not found: find node in original tree just before the true parent
    LsShape* pShape = this;
    do
        pShape = pShape->parent;
    while(pShape->parent->bIgnore);
    // Look at the siblings of this node
    for(pShape1 = pShape->sibling; pShape1; pShape1 = pShape1->sibling)
        if((pShape2 = ls_shape_of_subtree(pShape1)) != 0)
            return pShape2;
    return 0;
}

LsShape* LsShape::find_prev_sibling() {
    LsShape* pNext = find_parent();
    if(! pNext)
        return 0;
    pNext = pNext->find_child();
    LsShape* s = 0;
    while(pNext != this) {
        s = pNext;
        pNext = s->find_sibling();
    }
    return s;
}

LsTreeIterator::LsTreeIterator(Order ord, LsShape* shape, bool /*dummy*/)
: s(shape), o(ord) {}

LsTreeIterator LsTreeIterator::end(Order ord, LsShape* s) {
    LsTreeIterator it(ord, s, true);
    if(s && !s->bIgnore) {
        if(ord == Pre)
            it.s = uncle(s);
        else // (ord == Post)
            ++it;
    }
    return it;
}

LsShape* LsTreeIterator::go_bottom(LsShape* s) {
    for(LsShape* t = s->find_child(); t; t = s->find_child())
        s = t;
    return s;
}

LsShape* LsTreeIterator::uncle(LsShape* s) {
    LsShape* sNew;
    while((sNew = s->find_sibling()) == 0)
        if((s = s->find_parent()) == 0)
            break;
    return sNew;
}

LsTreeIterator& LsTreeIterator::operator++() {
    if(o == Pre) {
        LsShape* sNew = s->find_child();
        s = (sNew == 0)? uncle(s): sNew;
    } else { // (o == Post)
        LsShape* sNew = s->find_sibling();
        s = (sNew == 0)? s->find_parent(): go_bottom(sNew);
    }
    return *this;
}
