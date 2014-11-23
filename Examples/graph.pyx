# x++ and *x are both disallowed in python syntax, so we need to use function
# equivalents.
from cython.operator cimport preincrement as inc
from cython.operator cimport dereference as deref

# Below, read "cdef ..." as "define a C object"

# Tell Cython about the C++ structure, only the elements we use.
cdef extern from "Graph.hpp":
    cdef cppclass Point:
        Point()
    cdef cppclass Graph[V]:  # [V] is the template argument
        # forward declarations
        cppclass Node
        cppclass Edge
        cppclass incident_iterator
        cppclass node_iterator

        Graph()
        Node add_node(Point, V)
        Edge add_edge(const Node &n1, const Node &n2)
        node_iterator node_begin()
        node_iterator node_end()

        cppclass Node:
           V value()
           int degree()
           incident_iterator edge_begin()
           incident_iterator edge_end()
        cppclass Edge:
           Node node1()
           Node node2()
        cppclass node_iterator:
           Node operator*()
           node_iterator &operator++()
           bint operator==(node_iterator)
        cppclass incident_iterator:
           Edge operator*()
           incident_iterator &operator++()
           bint operator==(incident_iterator)

# A python wrapper for the Graph class
cdef class PyGraph:
    cdef Graph[int] *g

    def __cinit__(self):
        # __cinit__ is called before __init__, and should make sure all of the
        # C++ data of the Python wrapper is set up.
        self.g = new Graph[int]()

    def __dealloc__(self):
        # __dealloc__ is a standard method.  It should handle any last-minute cleanup
        del self.g

    # Note the type information in the argument list (similar to cdef)
    def add_node(self, int v):
        cdef PyNode p = PyNode()
        p._node = self.g.add_node(Point(), v)  # This line is pure C++ after translation
        return p

    def add_edge(self, PyNode n1, PyNode n2):
        cdef PyEdge p = PyEdge()
        p._edge = self.g.add_edge(n1._node, n2._node)  # This line is pure C++ after translation
        return p

    def nodes(self):
        # Graph<int>::node_iterator in C++ == Graph[int].node_iterator in Cython
        cdef Graph[int].node_iterator n = self.g.node_begin()
        cdef PyNode p
        while not (n == self.g.node_end()):
            p = PyNode()  # Need to allocate a new PyNode for each yield
            p._node = deref(n)
            yield p
            inc(n)

# A python wrapper for the Graph::Node class
cdef class PyNode:
   # PyNode is a Python object, but we can add C++ slots, such as one to hold
   # the actual Node
   cdef Graph[int].Node _node

   def __repr__(self):
       # default print method
       return "<Node %d>" % (self._node.value())

   def degree(self):
       return self._node.degree()

   # Similar to the node_iterator above.
   def edges(self):
       # Graph<int>::incident_iterator in C++ == Graph[int].incident_iterator in Cython
       cdef Graph[int].incident_iterator e = self._node.edge_begin()
       cdef PyEdge p  # We need to cdef this to allow access to it's C++ slots
       while not (e == self._node.edge_end()):
           p = PyEdge()  # Need to allocate a new PyNode for each yield
           p._edge = deref(e)
           yield p
           inc(e)

# A python wrapper for the Graph::Node class
cdef class PyEdge:
   cdef Graph[int].Edge _edge
   def __repr__(self):
       p1, p2 = self.nodes()
       return "<Edge> between %s and %s" % (p1, p2)

   def nodes(self):
       cdef PyNode p1 = PyNode()
       cdef PyNode p2 = PyNode()
       p1._node = self._edge.node1()
       p2._node = self._edge.node2()
       return p1, p2
