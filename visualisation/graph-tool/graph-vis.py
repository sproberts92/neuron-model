from graph_tool.all import *
import numpy as np
import argparse
import os

def arg_parse():
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('path', nargs='?', default='.')
    group.add_argument('-c', '--celeg', nargs='?', const=1, default=0)
    args = parser.parse_args()
    return args

def main():
    args = arg_parse()

    if args.celeg:
        g = collection.data["celegansneural"]
        outpath = "output/celegans.pdf"
    else:
        tail = os.path.split(args.path)[-1]
        name = os.path.splitext(tail)[0]
        outpath = os.path.join('output','{0}.pdf'.format(name))

        adj = np.loadtxt(args.path)

        g = Graph()
        g.add_vertex(adj.shape[0])

        for i, vv in enumerate(adj):
            for j, v in enumerate(vv):
                if v == 1:
                    g.add_edge(i, j)

    state = minimize_nested_blockmodel_dl(g, deg_corr=True)
    draw_hierarchy(state, output=outpath)

if __name__ == '__main__':
    main()
