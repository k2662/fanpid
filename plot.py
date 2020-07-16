#/usr/bin/env python

import sys

from matplotlib import pyplot as plt

def read_csv(path):
    with open(path) as f:
        lines = f.readlines()

    data = list()
    for line in lines:
        cols = line.split(',')
        numeric_cols = [float(c) for c in cols[:-1]]
        cols = [*numeric_cols, cols[-1]]
        data.append(cols)
    return list(zip(*data))

if __name__ == '__main__':
    path = sys.argv[1]
    data = read_csv(path)

    controlled = [0 if d == 'auto' else 1 for d in data[-1]]

    fig, axes = plt.subplots(nrows=4)
    temp, fans_actual, fans_target, ctl = axes

    plt.sca(temp)
    plt.plot(data[0])

    plt.sca(fans_target)
    plt.plot(data[1])
    plt.plot(data[2])

    plt.sca(fans_actual)
    plt.plot(data[3])
    plt.plot(data[4])

    plt.sca(ctl)
    plt.plot(controlled)

    plt.show()
