import pyximport; pyximport.install()
import graph

g = graph.PyGraph()
n1 = g.add_node(1)
n2 =  g.add_node(2)
n3 = g.add_node(3)
n4 = g.add_node(4)
g.add_edge(n1, n2)
g.add_edge(n2, n3)
g.add_edge(n1, n3)
g.add_edge(n2, n4)
g.add_edge(n4, n3)

for n in g.nodes():
    print n, "has", n.degree(), "edges"
    for e in n.edges():
        print "  ", e
