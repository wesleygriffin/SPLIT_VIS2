/*
 *  *  Copyright (C) 2007  Simon Perreault
 *   *
 *    *  This program is free software: you can redistribute it and/or modify
 *     *  it under the terms of the GNU General Public License as published by
 *      *  the Free Software Foundation, either version 3 of the License, or
 *       *  (at your option) any later version.
 *        *
 *         *  This program is distributed in the hope that it will be useful,
 *          *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *            *  GNU General Public License for more details.
 *             *
 *              *  You should have received a copy of the GNU General Public License
 *               *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *                */
/**
 *  * \class Octree
 *   * \brief Generic octree template
 *    *
 *     * \author Simon Perreault <nomis80@nomis80.org>
 *      * \date April 2007
 *       *
 *        * This class template represents an octree, often used for manipulating 3-D
 *         * scattered data efficiently. The type of the contained data is supplied as a
 *          * template parameter.
 *           *
 *            * \param T Type of the contained data. Requirements on type: must be copyable
 *             * and default-constructible.
 *              *
 *               * \param AS Short for "aggregate size." As an optimization, leaves can be
 *                * aggregated so that the relative size of pointers is diminished. This is 1 by
 *                 * default, but should be set higher when the size of \a T is small. <b>Must be
 *                  * a power of two.</b>
 *                   */

/**
 *  * \param size Size of octree, in nodes. Should be a power of two. For example,
 *   * an octree with \a size = 256 will represent a cube divided into 256x256x256
 *    * nodes. <b>Must be a power of two.</b>
 *     *
 *      * \param emptyValue This is the value that will be returned when accessing
 *       * regions of the 3-D volume where no node has been allocated. In other words,
 *        * instead of following a null node pointer, this value is returned. Since the
 *         * octree root is initially a null pointer, the whole volume is initialized to
 *          * this value.
 *           */


#include "svTree.h"

using namespace std;


namespace __svl_lib {

template< typename T, int AS >
Tree<T,AS>::Tree( int size, const T& emptyValue )
    : root_(0) //root is NULL
    , emptyValue_(emptyValue)
    , size_(size)
{
    // Make sure size is power of two.

//    assert( ((size - 1) & size) == 0 );
//    assert( ((AS - 1) & AS) == 0 );
}

/**
 *   Performs a deep copy of an octree. All branch pointers will be followed
 *   recursively and new nodes will be allocated.
 *  
 *   \param o Octree to be copied.
 * */

template< typename T, int AS >
Tree<T,AS>::Tree( const Tree<T,AS>& o )
    : emptyValue_( o.emptyValue_ )
    , size_( o.size_ )
{
    if ( !o.root_ ) {
        root_ = 0;
    } else {
        switch ( o.root_->type() ) {
            case BranchNode:
                root_ = new Branch   ( *reinterpret_cast<Branch   *>(o.root_) );
                break;
            case LeafNode:
                root_ = new Leaf     ( *reinterpret_cast<Leaf     *>(o.root_) );
                break;
            case AggregateNode:
                root_ = new Aggregate( *reinterpret_cast<Aggregate*>(o.root_) );
                break;
        }
    }
}

/**
   * Recursively deletes all nodes by following branch pointers.
 */
template< typename T, int AS >
Tree<T,AS>::~Tree()
{
    deleteNode(&root_);
}

/**
    Swaps the octree's contents with another's. This is a cheap operation as only
    the root pointers are swapped, not the whole structure.
     */
template< typename T, int AS >
void Tree<T,AS>::swap( Tree<T,AS>& o )
{
    std::swap( emptyValue_, o.emptyValue_ );  // This can throw.
    std::swap( root_, o.root_ );
    std::swap( size_, o.size_ );
}


}
