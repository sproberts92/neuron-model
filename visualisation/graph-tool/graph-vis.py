from graph_tool.all import *
import numpy as np

def main():
    adj = np.loadtxt('input/adj_300_12_5.dat')

    g = Graph()
    g.add_vertex(adj.shape[0])

    for i, vv in enumerate(adj):
        for j, v in enumerate(vv):
            if v == 1 and i > j:
                g.add_edge(i, j)

    # g = collection.data["celegansneural"]
    state = minimize_nested_blockmodel_dl(g, deg_corr=True)
    # draw_hierarchy(state, output="celegans_nested_mdl.pdf")
    draw_hierarchy(state, output="output/model_nested_mdl.pdf")

if __name__ == '__main__':
    main()