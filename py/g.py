import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import glob
import pathlib

def plotGraph(data, title):

    fig = plt.figure(figsize=(4,2), dpi=200)
    ax = fig.add_subplot(1, 1, 1)

    ax.barh(
        [e[0] for e in data],
        [e[1] for e in data]    )
    ax.set_title('Error ('+title+")")
    fig.subplots_adjust(left=0.3, bottom=0.25, top=0.8)
    plt.savefig(title) # -----(2)

def main():

    for fn in glob.glob("./*.csv"):
        with open(fn) as f:
            lines = reversed(f.readlines())
            items = [ x.split(",") for x in lines ]
            data = [ [name.strip(), float(val)] for name,val in items]
            plotGraph( data, pathlib.Path(fn).stem)

main()