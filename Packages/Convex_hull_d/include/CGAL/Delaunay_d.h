// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
//---------------------------------------------------------------------
// file generated by notangle from delaunay.lw
// please debug or modify LEDA web file
// mails and bugs: Michael.Seel@mpi-sb.mpg.de
// based on LEDA architecture by S. Naeher, C. Uhrig
// coding: K. Mehlhorn, M. Seel
// debugging and templatization: M. Seel
//---------------------------------------------------------------------

#ifndef CGAL_DELAUNAY_D_H
#define CGAL_DELAUNAY_D_H

/*{\Manpage {Delaunay_d}{R,Lifted_R}{Delaunay Triangulations}{DT}}*/
/*{\Mdefinition 

An instance |\Mvar| of type |\Mname| is the nearest and furthest
site Delaunay triangulation of a set |S| of points in some
$d$-dimensional space. We call |S| the underlying point set and $d$ or
|dim| the dimension of the underlying space.  We use |dcur| to denote
the affine dimension of |S|.  The data type supports incremental
construction of Delaunay triangulations and various kind of query
operations (in particular, nearest and furthest neighbor queries and
range queries with spheres and simplices).

A Delaunay triangulation is a simplicial complex. All simplices in
the Delaunay triangulation have dimension |dcur|.  In the nearest site
Delaunay triangulation the circumsphere of any simplex in the
triangulation contains no point of $S$ in its interior. In the
furthest site Delaunay triangulation the circumsphere of any simplex
contains no point of $S$ in its exterior. If the points in $S$ are
co-circular then any triangulation of $S$ is a nearest as well as a
furthest site Delaunay triangulation of $S$. If the points in $S$ are
not co-circular then no simplex can be a simplex of both
triangulations. Accordingly, we view |\Mvar| as either one or two
collection(s) of simplices. If the points in $S$ are co-circular there
is just one collection: the set of simplices of some triangulation.
If the points in $S$ are not co-circular there are two
collections. One collection consists of the simplices of a nearest
site Delaunay triangulation and the other collection consists of the
simplices of a furthest site Delaunay triangulation.

For each simplex of maximal dimension there is a handle of type
|Simplex_handle| and for each vertex of the triangulation there is a
handle of type |Vertex_handle|. Each simplex has |1 + dcur| vertices
indexed from $0$ to |dcur|.  For any simplex $s$ and any index $i$,
|DT.vertex_of(s,i)| returns the $i$-th vertex of $s$. There may or may
not be a simplex $t$ opposite to the vertex of $s$ with index $i$.
The function |DT.opposite_simplex(s,i)| returns $t$ if it exists and
returns |Simplex_handle()| otherwise. If $t$ exists then $s$ and $t$
share |dcur| vertices, namely all but the vertex with index $i$ of $s$
and the vertex with index
|DT.index_of_vertex_in_opposite_simplex(s,i)| of $t$.  Assume that $t
= |DT.opposite_simplex(s,i)|$ exists and let $j =
|DT.index_of_vertex_in_opposite_simplex(s,i)|$. Then |s =
DT.opposite_simplex(t,j)| and |i =
DT.index_of_vertex_in_opposite_simplex(t,j)|.  In general, a vertex
belongs to many simplices.

Any simplex of |\Mvar| belongs either to the nearest or to the
furthest site Delaunay triangulation or both. The test
|DT.simplex_of_nearest(dt_simplex s)| returns true if |s| belongs to
the nearest site triangulation and the test
|DT.simplex_of_furthest(dt_simplex s)| returns true if |s| belongs to
the furthest site triangulation.
}*/

#include <CGAL/Unique_hash_map.h>
#include <CGAL/Convex_hull_d.h>

CGAL_BEGIN_NAMESPACE

template <typename R_, typename Lifted_R_ = R_>
class Delaunay_d : public Convex_hull_d<Lifted_R_>
{ 
typedef Delaunay_d<R_,Lifted_R_> Self;
typedef Convex_hull_d<Lifted_R_> Base;

using Base::origin_simplex_;

public:

using Base::dcur;

/*{\Mgeneralization Convex_hull_d<Lifted_R>}*/

/*{\Mtypes 7}*/
typedef R_ R;
typedef Lifted_R_ Lifted_R;

typedef typename Base::Simplex_handle Simplex_handle;
/*{\Mtypemember handles to the simplices of the complex.}*/

typedef typename Base::Vertex_handle Vertex_handle;
/*{\Mtypemember handles to vertices of the complex.}*/

typedef typename Base::Simplex_const_handle Simplex_const_handle;
typedef typename Base::Vertex_const_handle Vertex_const_handle;

class Simplex_iterator;
class Simplex_const_iterator;
friend class Simplex_iterator;
friend class Simplex_const_iterator;

typedef typename R::Point_d Point_d;
/*{\Mtypemember the point type}*/
typedef typename R::Sphere_d Sphere_d;
/*{\Mtypemember the sphere type}*/
typedef typename R::FT FT;

typedef typename Lifted_R::Point_d Lifted_point_d;
typedef typename Lifted_R::Vector_d Lifted_vector_d;
typedef typename Lifted_R::Hyperplane_d Lifted_hyperplane_d;
typedef typename Lifted_R::RT RT;

enum Delaunay_voronoi_kind { NEAREST, FURTHEST };
/*{\Menum interface flags}*/

/*{\Mtext To use these types you can typedef them into the global
scope after instantiation of the class. We use |Vertex_handle| instead
of |\Mname::Vertex_handle| from now on. Similarly we use
|Simplex_handle|.}*/
private:
  enum type_of_S { unknown, non_cocircular, cocircular };  
  type_of_S ts;

  const R& Delaunay_kernel_;

  enum type_of_facet { lower_hull, upper_hull, vertical };

  type_of_facet type_of(typename Base::Facet_const_handle f) const
  /*{\Xop returns the type of the facet $f$.}*/
  { typename Lifted_R::Orthogonal_vector_d ortho =
      lifted_kernel().orthogonal_vector_d_object();
    Lifted_vector_d normal = ortho(hyperplane_supporting(f));
    typename Lifted_R::Component_accessor_d access =
      lifted_kernel().component_accessor_d_object();
    int d = CGAL_NTS sign( 
      access.homogeneous(normal,access.dimension(normal)-1));
    if (d > 0) return upper_hull;
    if (d < 0) return lower_hull;
    return vertical;
  }

  type_of_facet type_of(typename Base::Facet_handle f) const
  { return type_of(static_cast<typename Base::Facet_const_handle>(f)); }


  bool incident_simplex_search(Vertex_handle v, Simplex_handle s) const;

public:
  
  typedef typename Base::Point_const_iterator Point_const_iterator;
  /*{\Mtypemember the iterator for points.}*/

  typedef typename Base::Vertex_iterator Vertex_iterator;
  /*{\Mtypemember the iterator for vertices.}*/

  typedef typename Base::Simplex_iterator CH_simplex_iterator;
  typedef typename Base::Simplex_const_iterator CH_simplex_const_iterator;

  class Simplex_iterator 
  /*{\Mtypemember the iterator for simplices.}*/ 
    : public CH_simplex_iterator 
  {
    typedef Delaunay_d<R,Lifted_R> Delaunay;
    typedef CH_simplex_iterator Base_iterator;

    Delaunay* DT;
    type_of_facet tf;
    Base_iterator base() { return Base_iterator(*this); }
  public:
    Simplex_iterator(Base_iterator y = Base_iterator()) : 
      Base_iterator(y) {}

    Simplex_iterator(Delaunay* x,  Base_iterator y, 
      Delaunay_voronoi_kind z = NEAREST) : Base_iterator(y), DT(x) 
    /* if the facet is not nil we set the current marker to
       the facet and insert all it's neighbors into the
       candidates stack */
    { CGAL_assertion(base() != Base_iterator());
      tf = (z == NEAREST ? lower_hull : upper_hull); 
      bool cocirc = DT->is_S_cocircular();
      while ( base() != DT->simplices_end() &&
              !( cocirc && DT->is_bounded_simplex(base()) ||
                 !cocirc && DT->is_unbounded_simplex(base()) && 
                 DT->type_of(base()) == tf ) ) {
         Base_iterator::operator++();
      }
    }

    Simplex_iterator(const Simplex_iterator& it) : Base_iterator(it) {}

    Simplex_iterator& operator++()
    /* here we get a new candidate from the stack
       and insert all its not-visited neighbors */
    { 
      bool cocirc = DT->is_S_cocircular();
      do {
        Base_iterator::operator++();
      } while ( base() != DT->simplices_end() &&
                !( cocirc && DT->is_bounded_simplex(base()) ||
                   !cocirc && DT->is_unbounded_simplex(base()) && 
                   DT->type_of(base()) == tf ) );
      return *this; 
    }
    Simplex_iterator  operator++(int) 
    { Simplex_iterator tmp = *this; ++(*this); return tmp; }

    // change modus:
    typedef std::forward_iterator_tag iterator_category;
  private:
    Simplex_iterator  operator--(int);
    Simplex_iterator& operator--();

  }; // Simplex_iterator


  class Simplex_const_iterator : public CH_simplex_const_iterator {
    typedef Delaunay_d<R,Lifted_R> Delaunay;
    typedef CH_simplex_const_iterator Base_iterator;

    const Delaunay* DT;
    type_of_facet tf;
    Base_iterator base() { return Base_iterator(*this); }
  public:
    Simplex_const_iterator(Base_iterator y = Base_iterator()) : 
      Base_iterator(y) {}

    Simplex_const_iterator(const Delaunay* x,  Base_iterator y, 
      Delaunay_voronoi_kind z = NEAREST) : Base_iterator(y), DT(x) 
    /* if the facet is not nil we set the current marker to
       the facet and insert all it's neighbors into the
       candidates stack */
    { CGAL_assertion(base() != Base_iterator());
      tf = (z == NEAREST ? lower_hull : upper_hull); 
      bool cocirc = const_cast<Delaunay*>(DT)->is_S_cocircular();
      while ( base() != DT->simplices_end() &&
              !( cocirc && DT->is_bounded_simplex(base()) ||
                 !cocirc && DT->is_unbounded_simplex(base()) && 
                 DT->type_of(base()) == tf ) ) {
         Base_iterator::operator++();
      }
    }

    Simplex_const_iterator(const Simplex_const_iterator& it) : 
      Base_iterator(it) {}

    Simplex_const_iterator& operator++()
    /* here we get a new candidate from the stack
       and insert all its not-visited neighbors */
    { 
      bool cocirc = const_cast<Delaunay*>(DT)->is_S_cocircular();
      do {
        Base_iterator::operator++();
      } while ( base() != DT->simplices_end() &&
                !( cocirc && DT->is_bounded_simplex(base()) ||
                   !cocirc && DT->is_unbounded_simplex(base()) && 
                   DT->type_of(base()) == tf ) ); 
      return *this; 
    }
    Simplex_const_iterator  operator++(int) 
    { Simplex_iterator tmp = *this; ++(*this); return tmp; }

    // change modus:
    typedef std::forward_iterator_tag iterator_category;
  private:
    Simplex_const_iterator  operator--(int);
    Simplex_const_iterator& operator--();

  }; // Simplex_iterator




  void project(Regular_complex_d<R>& RC, int which = -1) const;
  /*{\Xop projects the upper (|which = 1|) or lower (|which = -1|) hull
  into regular complex |RC|. }*/

    
  bool is_S_cocircular();
  /*{\Xop returns |true| if the points of |S| are cocircular and returns
  |false| otherwise}*/

  /*{\Mcreation 3}*/

  Delaunay_d(int d, const R& k1 = R(), const Lifted_R& k2 = Lifted_R())
  /*{\Mcreate creates an instance |\Mvar| of type |\Mtype|. The
  dimension of the underlying space is $d$ and |S| is initialized to the
  empty point set. The traits class |R| specifies the models of
  all types and the implementations of all geometric primitives used by
  the Delaunay class. The traits class |Lifted_R| specifies the models of
  all types and the implementations of all geometric primitives used by
  the base class of |\Mname|. The second template parameter defaults to
  the first: |Delaunay_d<R> = Delaunay_d<R, Lifted_R = R >|.}*/
   : Base(d+1,k2), Delaunay_kernel_(k1) { ts = unknown; }

  /*{\Mtext Both template arguments have to be models that fit a
  subset of requirements of the d-dimensional kernel. We list them at
  the end of this manual page.}*/

  const R& kernel() const { return Delaunay_kernel_; }
  const R& lifted_kernel() const { return Base::kernel(); }

  private:
  /*{\Mtext The data type |\Mtype| offers neither copy constructor nor
  assignment operator.}*/
  Delaunay_d(const Self&); 
  Self& operator=(const Self&);
  public:


  /*{\Moperations 3 3}*/
  /*{\Mtext All operations below that take a point |x| as an argument
  have the common precondition that $|x.dimension()| = |\Mvar.dimension()|$.}*/

  int dimension() const 
  /*{\Mop returns the dimension of ambient space}*/
  { return (Base::dimension() - 1); } 

  int current_dimension() const
  /*{\Mop returns the affine dimension of the current point set, i.e.,
  $-1$ is $S$ is empty, $0$ if $S$ consists of a single point,
  $1$ if all points of $S$ lie on a common line, etcetera.}*/
  { int d = Base::current_dimension();
    if (d == -1) return d;
    return ( const_cast<Self*>(this)->is_S_cocircular() ? d : d-1 );
  }

  bool is_simplex_of_nearest(Simplex_handle s) const
  /*{\Mop returns true if |s| is a simplex of the nearest site 
  triangulation.}*/
  { if ( const_cast<Self*>(this)->is_S_cocircular() ) return true;
    return ( type_of(s) == lower_hull );
  }

  bool is_simplex_of_furthest(Simplex_handle s) const
  /*{\Mop returns true if |s| is a simplex of the furthest site 
  triangulation.}*/
  { if ( const_cast<Self*>(this)->is_S_cocircular() ) return true;
    return (type_of(s) == upper_hull);
  }  

  bool is_simplex_of_nearest(Simplex_const_handle s) const
  { if ( const_cast<Self*>(this)->is_S_cocircular() ) return true;
    return ( type_of(s) == lower_hull );
  }
  bool is_simplex_of_furthest(Simplex_const_handle s) const
  { if ( const_cast<Self*>(this)->is_S_cocircular() ) return true;
    return (type_of(s) == upper_hull);
  }  

     
  Vertex_handle vertex_of_simplex(Simplex_handle s, int i) const
  /*{\Mop returns the vertex associated with the $i$-th node of $s$.
  \precond $0 \leq i \leq |dcur|$. }*/
  { if ( const_cast<Self*>(this)->is_S_cocircular() )
      return Base::vertex_of_simplex(s,i);
    else 
     return Base::vertex_of_simplex(s,i+1);
  }

  Vertex_const_handle vertex_of_simplex(Simplex_const_handle s, 
                                        int i) const
  { if ( const_cast<Self*>(this)->is_S_cocircular() )
      return Base::vertex_of_simplex(s,i);
    else 
      return Base::vertex_of_simplex(s,i+1);
  }

  Point_d associated_point(Vertex_handle v) const
  /*{\Mop returns the point associated with vertex $v$.}*/
  { typename Lifted_R::Project_along_d_axis_d project =
      lifted_kernel().project_along_d_axis_d_object();
    return project(Base::associated_point(v)); }

  Point_d associated_point(Vertex_const_handle v) const
  { typename Lifted_R::Project_along_d_axis_d project =
      lifted_kernel().project_along_d_axis_d_object();
    return project(Base::associated_point(v)); }

  Point_d point_of_simplex(Simplex_handle s,int i) const
  /*{\Mop returns the point associated with the $i$-th vertex of $s$.
  \precond $0 \leq i \leq |dcur|$. }*/
  { return associated_point(vertex_of_simplex(s,i)); }

  Point_d point_of_simplex(Simplex_const_handle s,int i) const
  { return associated_point(vertex_of_simplex(s,i)); }


  Simplex_handle opposite_simplex(Simplex_handle s, int i) const
  /*{\Mop returns the simplex opposite to the $i$-th vertex of $s$
  (|Simplex_handle()| if there is no such simplex).
  \precond $0 \leq i \leq |dcur|$. }*/
  { 
    if ( const_cast<Self*>(this)->is_S_cocircular() ) {
      Simplex_handle f = Base::opposite_simplex(s,i);
      return ( Base::is_unbounded_simplex(f) ? 
	       Simplex_handle() : f );    
    } else {
      Simplex_handle f = Base::opposite_simplex(s,i+1);
      return ( type_of(f) == type_of(s) ? f : Simplex_handle() );
    }
  }  

  Simplex_const_handle opposite_simplex(Simplex_const_handle s, 
                                        int i) const
  { 
    if ( const_cast<Self*>(this)->is_S_cocircular() ) {
      Simplex_const_handle f = Base::opposite_simplex(s,i);
      return ( Base::is_unbounded_simplex(f) ? 
	       Simplex_const_handle() : f );    
    } else {
      Simplex_const_handle f = Base::opposite_simplex(s,i+1);
      return ( type_of(f) == type_of(s) ? f : Simplex_const_handle() );
    }
  }  
   
  int index_of_vertex_in_opposite_simplex(Simplex_handle s,int i) const
  /*{\Mop returns the index of the vertex opposite to the $i$-th vertex 
  of $s$. \precond $0 \leq i \leq |dcur|$.}*/
  { 
    if ( const_cast<Self*>(this)->is_S_cocircular() ) 
      return Base::index_of_vertex_in_opposite_simplex(s,i);
    else
      return Base::index_of_vertex_in_opposite_simplex(s,i+1) - 1; 
  }  

  int index_of_vertex_in_opposite_simplex(Simplex_const_handle s,
                                          int i) const
  { 
    if ( const_cast<Self*>(this)->is_S_cocircular() ) 
      return Base::index_of_vertex_in_opposite_simplex(s,i);
    else
      return Base::index_of_vertex_in_opposite_simplex(s,i+1) - 1; 
  }  

  Simplex_handle simplex(Vertex_handle v) const;
  /*{\Mop returns a simplex of the nearest site triangulation incident 
  to $v$.}*/

  int index(Vertex_handle v) const;
  /*{\Mop returns the index of $v$ in |\Mvar.simplex(v)|.}*/

  bool  contains(Simplex_handle s, const Point_d& x) const;
  /*{\Mop returns true if |x| is contained in the closure of simplex |s|.}*/

  bool empty() const
  /*{\Mop  decides whether |\Mvar| is empty.}*/
  { return (current_dimension() == -1); }

  void clear()
  /*{\Mop reinitializes |\Mvar| to the empty Delaunay triangulation.}*/
  { int d = dimension(); Base::clear(d + 1);
    ts = unknown;
  }


  Vertex_handle insert(const Point_d& x)
  /*{\Mop inserts point $x$ into |\Mvar| and returns the corresponding
  |Vertex_handle|.  More precisely, if there is already a vertex |v| in
  |\Mvar| positioned at $x$ (i.e., |associated_point(v)| is equal to
  |x|) then |associated_point(v)| is changed to |x| (i.e.,
  |associated_point(v)| is made identical to |x|) and if there is no
  such vertex then a new vertex $v$ with |associated_point(v) = x| is
  added to |\Mvar|.  In either case, $v$ is returned.}*/
  { ts = unknown;
    typename Lifted_R::Lift_to_paraboloid_d lift =
      lifted_kernel().lift_to_paraboloid_d_object();
    return Base::insert(lift(x));
  }


  Simplex_handle locate(const Point_d& x) const;
  /*{\Mop returns a simplex of the nearest site triangulation
  containing |x| in its closure (returns |Simplex_handle()| if |x| lies 
  outside the convex hull of $S$).}*/


  Vertex_handle lookup(const Point_d& x) const
  /*{\Mop if |\Mvar| contains a vertex $v$ with |associated_point(v) = x| 
  the result is $v$ otherwise the result is |Vertex_handle()|. }*/
  { 
    Simplex_handle s = locate(x);
    if ( s == Simplex_handle() ) return Vertex_handle();
    for (int i = 0; i <= current_dimension(); i++) {
      Vertex_handle v = vertex_of_simplex(s,i);
      if ( x == associated_point(v) ) return v;
    }
    return Vertex_handle();
  }


  Vertex_handle nearest_neighbor(const Point_d& x) const;
  /*{\Mop computes a vertex $v$ of |\Mvar| that is closest to $x$,
  i.e.,\\ $|dist(x,associated_point(v))| = \min \{ 
  |dist(x, associated_point(u))| \mid u \in S\ \}$.}*/

  /*{\Mtext \setopdims{5cm}{1cm}}*/
  std::list<Vertex_handle> 
  range_search(const Sphere_d& C) const;
  /*{\Mop returns the list of all vertices contained in the closure of 
  sphere $C$.}*/

  std::list<Vertex_handle>
  range_search(const std::vector<Point_d>& A) const;
  /*{\Mop returns the list of all vertices contained in the closure of
  the simplex whose corners are given by |A|.
  \precond |A| must consist of $d+1$ affinely independent points
  in base space.}*/


  void all_vertices_below(const Lifted_hyperplane_d& h, 
                          Simplex_handle s, 
                          std::list<Vertex_handle>& result,
                          Unique_hash_map<Vertex_handle,bool>& is_new,
                          bool is_cocircular) const;


  std::list<Simplex_handle> 
  all_simplices(Delaunay_voronoi_kind k = NEAREST) const;
  /*{\Mop returns a list of all simplices of either the nearest or the
          furthest site Delaunay triangulation of |S|.}*/


  std::list<Vertex_handle> 
  all_vertices(Delaunay_voronoi_kind k = NEAREST) const;
  /*{\Mop returns a list of all vertices of either the nearest or the
  furthest site Delaunay triangulation of |S|.}*/

  std::list<Point_d> all_points() const;
  /*{\Mop returns $S$. }*/

  Point_const_iterator points_begin() const
  /*{\Mop returns the start iterator for points in |\Mvar|.}*/
  { return Point_const_iterator(Base::points_begin()); }

  Point_const_iterator points_end() const
  /*{\Mop returns the past the end iterator for points in |\Mvar|.}*/
  { return Point_const_iterator(Base::points_end()); }

  Simplex_iterator simplices_begin(Delaunay_voronoi_kind k = NEAREST)
  /*{\Mop returns the start iterator for simplices of |\Mvar|.}*/
  { return Simplex_iterator(this,Base::simplices_begin(),k); }


  Simplex_iterator simplices_end()
  /*{\Mop returns the past the end iterator for simplices of |\Mvar|.}*/
  { return Simplex_iterator(Base::simplices_end()); }


  Simplex_const_iterator 
  simplices_begin(Delaunay_voronoi_kind k = NEAREST) const
  { return Simplex_const_iterator(this,Base::simplices_begin(),k); }

  Simplex_const_iterator simplices_end() const
  { return Simplex_const_iterator(Base::simplices_end()); }


  /*{\Mimplementation The data type is derived from |Convex_hull_d| via
  the lifting map. For a point $x$ in $d$-dimensional space let
  |lift(x)| be its lifting to the unit paraboloid of revolution. There
  is an intimate relationship between the Delaunay triangulation of a
  point set $S$ and the convex hull of |lift(S)|: The nearest site
  Delaunay triangulation is the projection of the lower hull and the
  furthest site Delaunay triangulation is the upper hull.  For
  implementation details we refer the reader to the implementation
  report available from the CGAL server.

  The space requirement is the same as for convex hulls. The time
  requirement for an insert is the time to insert the lifted point
  into the convex hull of the lifted points.}*/

  /*{\Mexample 

  The abstract data type |Delaunay_d| has a default instantiation by
  means of the $d$-dimensional geometric kernel.

  \begin{Mverb}
  #include <CGAL/Homogeneous_d.h>
  #include <CGAL/leda_integer.h>
  #include <CGAL/Delaunay_d.h>

  typedef leda_integer RT;
  typedef CGAL::Homogeneous_d<RT> Kernel;
  typedef CGAL::Delaunay_d<Kernel> Delaunay_d;
  typedef Delaunay_d::Point_d Point;
  typedef Delaunay_d::Simplex_handle Simplex_handle;
  typedef Delaunay_d::Vertex_handle Vertex_handle;

  int main()
  {
    Delaunay_d T(2);
    Vertex_handle v1 = T.insert(Point_d(2,11));
    ...
  }
  \end{Mverb}
  }*/

  /*{\Mtext\headerline{Traits requirements}

  |\Mname| requires the following types from the kernel traits |Lifted_R|:
  \begin{Mverb}
    RT Point_d Vector_d Ray_d Hyperplane_d 
  \end{Mverb}
  and uses the following function objects from the kernel traits:
  \begin{Mverb}
    Construct_hyperplane_d
    Construct_vector_d
    Vector_to_point_d / Point_to_vector_d
    Orientation_d
    Orthogonal_vector_d
    Oriented_side_d / Has_on_positive_side_d
    Affinely_independent_d
    Contained_in_simplex_d
    Contained_in_affine_hull_d
    Intersect_d
    Lift_to_paraboloid_d / Project_along_d_axis_d
    Component_accessor_d
  \end{Mverb}
  |\Mname| requires the following types from the kernel traits |R|:
  \begin{Mverb}
    FT Point_d Sphere_d 
  \end{Mverb}
  and uses the following function objects from the kernel traits |R|:
  \begin{Mverb}
    Construct_sphere_d
    Squared_distance_d
    Point_of_sphere_d
    Affinely_independent_d
    Contained_in_simplex_d
  \end{Mverb}
  }*/



}; // Delaunay_d<R,Lifted_R>

template <typename R, typename Lifted_R>
void Delaunay_d<R,Lifted_R>::project(Regular_complex_d<R>& RC, int which) const
{
  RC.clear(dimension());
  Delaunay_voronoi_kind k = (which == -1 ? NEAREST : FURTHEST);
  Unique_hash_map<Simplex_const_handle, Simplex_handle > project_simps;
  Unique_hash_map<Vertex_const_handle,  Vertex_handle >  project_verts;
  int dc = current_dimension();
  RC.set_current_dimension(dc);

  Simplex_const_iterator f;
  for(f =  simplices_begin(k); f != simplices_end(); ++f) {
    Simplex_handle s = project_simps[f] = RC.new_simplex();
    for (int i = 0; i <= dc; i++) {
      Vertex_const_handle v = vertex_of_simplex(f,i);
      Vertex_handle pv = project_verts[v];
      if ( pv == Vertex_handle() ) {
        Point_d x = associated_point(v);
        pv = project_verts[v] = RC.new_vertex(x);
      }
      RC.associate_vertex_with_simplex(s,i,pv);
    }
  }

  /* in a second pass we set up neighbor connections */
  Simplex_iterator s,t;
  for(f =  simplices_begin(k); f != simplices_end(); ++f) {
    s = project_simps[f];
    if ( s != Simplex_handle() ) {
      for (int i = 0; i <= dc; i++) {
        t = project_simps[opposite_simplex(f,i)];
        if ( dc > 0 && t != Simplex_handle() ) {
          RC.set_neighbor(s,i,t,
                          index_of_vertex_in_opposite_simplex(f,i)); 
        }
      }
    }
  }
}


template <typename R, typename Lifted_R>
bool Delaunay_d<R,Lifted_R>::is_S_cocircular() 
{ 
  if (ts == unknown) {
    int d = Base::current_dimension();
    std::vector<Lifted_point_d> A(d + 1);
    typename Lifted_R::Project_along_d_axis_d project =
      lifted_kernel().project_along_d_axis_d_object();
    for (int i = 0; i <= d; i++)
      A[i] = project( Base::point_of_simplex(origin_simplex_,i));

    typename Lifted_R::Affinely_independent_d affinely_independent =
      lifted_kernel().affinely_independent_d_object();
    ts = ( affinely_independent(A.begin(),A.end()) ? 
           cocircular : non_cocircular );
    if ( d == -1 && ts != cocircular )
      CGAL_assertion_msg(0,
        "affinely independent works incorrectly for empty set");
  }
  return (ts == cocircular);
}


template <typename R, typename Lifted_R> 
bool Delaunay_d<R,Lifted_R>::
incident_simplex_search(Vertex_handle v, Simplex_handle s) const
{ 
  visited_mark(s) = true;
  if ( const_cast<Self*>(this)->is_S_cocircular() == 
       is_bounded_simplex(s) ) {
    // we have found a simplex of the desired kind 
    int low = ( is_unbounded_simplex(s) ? 1 : 0 );
    for ( int i = low; i <= Base::current_dimension(); i++) {
      if ( v == Base::vertex_of_simplex(s,i) ) {
        const_cast<Self*>(this)->associate_vertex_with_simplex(s,i,v); 
        return true;
      }
    }
    CGAL_assertion_msg(0,
      "Delaunay_d::incident_simplex_search: unreachable point.");
  }

  /* s does not have the desired kind; we visit all neighbors except
     the one opposite v */

  bool incident = false;
  register int j;
  for (j = 0; j <= dcur; j++)
    if ( Base::vertex_of_simplex(s,j) == v ) incident = true;
  if ( !incident ) 
    CGAL_assertion_msg(0,"reached a simplex that is not incident to v");

  for (j = 0; j <= Base::current_dimension(); j++) {
    Simplex_handle t = Base::opposite_simplex(s,j);
    if ( Base::vertex_of_simplex(s,j) != v && !visited_mark(t)  &&
         incident_simplex_search(v,t) ) 
      return true;    
  }
  return false;
}

template <typename R, typename Lifted_R> 
typename Delaunay_d<R,Lifted_R>::Simplex_handle 
Delaunay_d<R,Lifted_R>::simplex(Vertex_handle v) const
{ 
  Simplex_handle s = Base::simplex(v);
  if ( Base::vertex_of_simplex(s,Base::index(v)) != v )
    CGAL_assertion_msg(0,"Delaunay_d::simplex: s is not incident to v.");
  incident_simplex_search(v,s);
  clear_visited_marks(s);
  return Base::simplex(v);
}

template <typename R, typename Lifted_R>
int Delaunay_d<R,Lifted_R>::index(Vertex_handle v) const
{ simplex(v);
  int i = Base::index(v);
  return ( const_cast<Self*>(this)->is_S_cocircular() ?  i : i-1);
}

template <typename R, typename Lifted_R>
bool Delaunay_d<R,Lifted_R>::
contains(Simplex_handle s, const Point_d& x) const
{ int d = current_dimension();
  if (d < 0) return false;
  std::vector<Point_d> A(d + 1);
  for (int i = 0; i <= d; i++) 
    A[i] = point_of_simplex(s,i);
  typename R::Contained_in_simplex_d contained_in_simplex =
    kernel().contained_in_simplex_d_object();
  return contained_in_simplex(A.begin(),A.end(),x);
}


template <typename R, typename Lifted_R>
typename Delaunay_d<R,Lifted_R>::Simplex_handle
Delaunay_d<R,Lifted_R>::
locate(const Point_d& x) const
{ 
  int d = current_dimension();
  if (d < 0) return Simplex_handle();
  if ( d == 0 ) {
    if ( x == point_of_simplex(origin_simplex_,0) )
      return origin_simplex_;
    else
      return Simplex_handle();
  }
  typename Lifted_R::Lift_to_paraboloid_d lift =
    lifted_kernel().lift_to_paraboloid_d_object();;
  Lifted_point_d lp = lift(x);
  if ( is_dimension_jump(lp) ) {
    Simplex_iterator s;
    for (s = const_cast<Self*>(this)->simplices_begin(NEAREST); 
         s != const_cast<Self*>(this)->simplices_end(); ++s) 
      if ( contains(s,x) ) return s;
    return Simplex_handle();
  }
  // lift(p) is not a dimension jump
  std::list<Simplex_handle> candidates;
  int dummy1 = 0; 
  int loc = -1; // intialization is important
  Simplex_handle f;
  this -> visibility_search(origin_simplex_,lp,candidates,dummy1,loc,f);
  this -> clear_visited_marks(origin_simplex_);
  if ( f != Simplex_handle() ) return f;
  typename std::list<Simplex_handle>::iterator it;
  for(it = candidates.begin(); it != candidates.end(); ++it)
    if ( contains(*it,x) ) return *it;
  return Simplex_handle();
}



template <typename R, typename Lifted_R>
typename Delaunay_d<R,Lifted_R>::Vertex_handle 
Delaunay_d<R,Lifted_R>::
nearest_neighbor(const Point_d& x) const
{ 
  int d = current_dimension();
  if (d < 0) return Vertex_handle();
  if (d == 0) 
    return Base::vertex_of_simplex(origin_simplex_,0);
  
  typename Lifted_R::Lift_to_paraboloid_d lift =
    lifted_kernel().lift_to_paraboloid_d_object();;
  Lifted_point_d lp = lift(x);
  std::list<Simplex_handle> candidates;
 
  if ( is_dimension_jump(lp) )
    candidates = all_simplices(NEAREST);
  else {
    // lift(x) is not a dimension jump
    int dummy1 = 0; 
    int location = -1;
    typename Base::Facet_handle f;
    this -> visibility_search(origin_simplex_,lp,candidates,dummy1,location,f);
    this -> clear_visited_marks(origin_simplex_);
    CGAL_assertion_msg( location != -1,
      "Delaunay_d::nearest_neighbor: location cannot be -1");
    if (location == 0) {
      // x must be one of the corners of f
      for (int i = 0; i < Base::current_dimension(); i++) {
        if ( point_of_facet(f,i) == lp )  
          return vertex_of_facet(f,i);
      }
      CGAL_assertion_msg(0,"Delaunay_d::nearest_neighbor: \
      if loc = 1 then lp must be corner of f");
    }
  }

  /* search through the vertices of the candidate simplices */
  if ( candidates.empty() ) 
    CGAL_assertion_msg(0,"Delaunay_d::nearest_neighbor: candidates is empty");
  Vertex_handle nearest_v = 
    vertex_of_simplex(*candidates.begin(),0);
  typename R::Squared_distance_d sqr_dist =
    kernel().squared_distance_d_object();
  FT min_dist = sqr_dist(x,associated_point(nearest_v));

  typename std::list<Simplex_handle>::iterator it;
  for(it=candidates.begin(); it!=candidates.end(); ++it) {
    for (int i = 0; i <= d ; i++) {
      typename R::Squared_distance_d sqr_dist =
        kernel().squared_distance_d_object();
      FT sidist = sqr_dist(x,point_of_simplex(*it,i));
      if ( sidist < min_dist ) {
        min_dist = sidist ;
        nearest_v = vertex_of_simplex(*it,i);
      }
    }
  }
  return nearest_v;
}

template <typename R, typename Lifted_R>
void Delaunay_d<R,Lifted_R>::
all_vertices_below(const Lifted_hyperplane_d& h, 
                   Simplex_handle s, 
                   std::list< Vertex_handle >& result,
                   Unique_hash_map<Vertex_handle,bool>& is_new,
                   bool is_cocircular) const
{ 
  visited_mark(s) = true;
  bool some_vertex_on_or_below_h = false;
  register int i;
  int low = (is_cocircular ? 0 : 1);
  for (i = low; i <= Base::current_dimension(); i++) {
    Vertex_handle v = Base::vertex_of_simplex(s,i);
    typename Lifted_R::Oriented_side_d side =
      lifted_kernel().oriented_side_d_object();
    if ( !(side(h, Base::associated_point(v)) == ON_POSITIVE_SIDE) ) {
      some_vertex_on_or_below_h = true;
      if ( is_new[v] ) {
        result.push_back(v);
        is_new[v] = false;
      }
    }
  }
        
  if ( !some_vertex_on_or_below_h ) return;
  for (i = low; i <= Base::current_dimension(); i++) {
    Simplex_handle t = Base::opposite_simplex(s,i);
    if ( !visited_mark(t) && 
         (!is_cocircular || is_bounded_simplex(t)) )
      all_vertices_below(h,t,result,is_new,is_cocircular);
  }
}

template <typename R, typename Lifted_R>
std::list< typename Delaunay_d<R,Lifted_R>::Vertex_handle > 
Delaunay_d<R,Lifted_R>::
range_search(const Sphere_d& C) const
{ 
  std::list<Vertex_handle> result;
  int dc = current_dimension();
  if ( dc < 0 ) 
    return result;
  Point_d c = C.center();
  Vertex_handle v = nearest_neighbor(c);
  if ( dc == 0 ) {
    if ( C.has_on_bounded_side(associated_point(v)) ) 
      result.push_back(v); return result;
  }
  Simplex_handle s = simplex(v);
  bool is_cocircular = const_cast<Self*>(this)->is_S_cocircular();
  Unique_hash_map<Vertex_handle,bool> is_new(true);
  int d = dimension();
  std::vector<Lifted_point_d> P(d + 1);
  typename Lifted_R::Lift_to_paraboloid_d lift =
    lifted_kernel().lift_to_paraboloid_d_object();
  typename R::Point_of_sphere_d point_of_sphere =
    kernel().point_of_sphere_d_object();
  for (int i = 0; i <= d; i++)  
    P[i] = lift(point_of_sphere(C,i));
  typedef typename Lifted_vector_d::Base_vector Base_vector;
  Lifted_point_d o = P[0] - 
    Lifted_vector_d(d+1,Base_vector(),d);
  typename Lifted_R::Construct_hyperplane_d hyperplane_trough =
    lifted_kernel().construct_hyperplane_d_object();
  Lifted_hyperplane_d h = 
    hyperplane_trough(P.begin(),P.end(),o,ON_NEGATIVE_SIDE);  
  // below is negative
  all_vertices_below(h,s,result,is_new,is_cocircular);
  clear_visited_marks(s);
  return result;
}
  

template <typename R, typename Lifted_R>
std::list< typename Delaunay_d<R,Lifted_R>::Vertex_handle > 
Delaunay_d<R,Lifted_R>::
range_search(const std::vector<Point_d>& A) const
{ 
  typename R::Affinely_independent_d affinely_independent =
    kernel().affinely_independent_d_object();
  CGAL_assertion_msg( affinely_independent(A.begin(),A.end()),
    "Delaunay_d::range_search: simplex must be affinely independent.");
  typename R::Construct_sphere_d sphere_through =
    kernel().construct_sphere_d_object();
  Sphere_d C = sphere_through(dimension(),A.begin(),A.end());
  std::list<Vertex_handle> result;
  std::list<Vertex_handle> candidates = range_search(C);
  typename R::Contained_in_simplex_d contained_in_simplex =
    kernel().contained_in_simplex_d_object();
  typename std::list<Vertex_handle>::iterator it;
  for(it = candidates.begin(); it != candidates.end(); ++it) {
    if ( contained_in_simplex(A.begin(),A.end(),associated_point(*it)) )
      result.push_back(*it);
  }
  return result;
}



template <typename R, typename Lifted_R>
std::list< typename Delaunay_d<R,Lifted_R>::Simplex_handle > 
Delaunay_d<R,Lifted_R>::
all_simplices(Delaunay_voronoi_kind k) const
{ 
  std::list<Simplex_handle> result;
  if ( dcur < 0 ) return result;
  Simplex_iterator s;
  for (s = const_cast<Self*>(this)->simplices_begin(k); 
       s != const_cast<Self*>(this)->simplices_end(); ++s) {
    result.push_back(s); 
  }
  return result;
}

template <typename R, typename Lifted_R>
std::list< typename Delaunay_d<R,Lifted_R>::Vertex_handle > 
Delaunay_d<R,Lifted_R>::
all_vertices(Delaunay_voronoi_kind k) const
{ 
  Unique_hash_map<Vertex_handle,bool> is_new_vertex(true);
  std::list<Vertex_handle> result;
  std::list<Simplex_handle> hull_simplices = all_simplices(k);
  typename std::list<Simplex_handle>::iterator it;
  for (it = hull_simplices.begin(); it != hull_simplices.end(); ++it) {
    for (int i = 0; i <= current_dimension(); i++) {
      Vertex_handle v = vertex_of_simplex(*it,i);
      if ( is_new_vertex[v] ) {
        is_new_vertex[v] = false;
        result.push_back(v);
      }
    }
  }
  return result;
}


template <typename R, typename Lifted_R>
std::list< typename Delaunay_d<R,Lifted_R>::Point_d > 
Delaunay_d<R,Lifted_R>::
all_points() const
{ 
  std::list<Point_d> result;
  std::list<Vertex_handle> all_nearest_verts = all_vertices(NEAREST);
  typename std::list<Vertex_handle>::iterator it;
  for(it = all_nearest_verts.begin(); 
      it != all_nearest_verts.end();
      ++it) 
    result.push_back(associated_point(*it));
  return result;
}


CGAL_END_NAMESPACE
#endif // CGAL_DELAUNAY_D_H


