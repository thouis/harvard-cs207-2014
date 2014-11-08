#pragma once
/** @file Mesh.hpp
 * @brief A Mesh is composed of nodes, edges, and triangles such that:
 *  -- All triangles have three nodes and three edges.
 *  -- All edges belong to at least one triangle and at most two triangles.
 */

/** @class Mesh
 * @brief A template for 3D triangular meshes.
 *
 * Users can add triangles and retrieve nodes, edges, and triangles.
 */
template <typename N, typename E, typename T>
class Mesh {
  // HW3: YOUR CODE HERE
  // Write all typedefs, public member functions, private member functions,
  //   inner classes, private member data, etc.
 public:
  /** Type of indexes and sizes. Return type of Mesh::num_nodes(). */
  typedef unsigned size_type;

  /** Return the number of nodes in the mesh. */
  size_type num_nodes() const {
    return 0;
  }

  /** Return the number of edges in the mesh. */
  size_type num_edges() const {
    return 0;
  }

  /** Return the number of triangles in the mesh. */
  size_type num_triangles() const {
    return 0;
  }
};
