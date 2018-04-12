import os
import argparse
import numpy as np
import subprocess
import re
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

import plotly
import plotly.graph_objs as go


# Initial definitions
algorithms = ["pucs", "spec_cmi"]
alg_color = {"pucs" : "red", "spec_cmi" : "blue"}
z_order = {"pucs" : 1, "spec_cmi" : 2}
cost_functions = {"pucs" : "mce", "spec_cmi" : "mce"}
gen_parity_path = "/home/gustavo/cs/machine_learning_data/" + \
        "parity_problem/gen_parity_instance.py"


# Parsing parameters
parser = argparse.ArgumentParser (description = 'Compares feature' + \
        'selection algorithms on parity machine learning problems.')
parser.add_argument ("n0", type = int, 
        help = "Minimum number of features.")
parser.add_argument ("nf", type = int, 
        help = "Maximum number of features.")
parser.add_argument ("ns", type = int, 
        help = "Number of features step.")
parser.add_argument ("sample_size", type = int, 
        help = "Number of samples in each training set.")
parser.add_argument ("m", type = int, 
        help = "Number of repetitions.")
args = parser.parse_args()
min_n = args.n0
max_n = args.nf
n_step = args.ns
sample_size = args.sample_size
m = args.m

# Define the number of points in the graph (on each dimension)
n_points = int ((max_n - min_n) / n_step) + 1


# Defines k
min_k = int (.25 * min_n + 1)
max_k = int (.75 * max_n + 1)
k_step = int ((max_k - min_k) / n_points) + 1

x = np.arange (min_n, max_n + 1, n_step)
y = np.arange (min_k, max_k + 1, k_step)
mesh_Z = {}
mesh_X = []
mesh_Y = []
min_err = {}
for alg in algorithms:
    mesh_Z[alg] = []
    min_err[alg] = 1

for n in range (min_n, max_n + 1, n_step):
    for k in range (min_k, max_k + 1, k_step):

        if (k >= n):
            continue

        print ("n = " + str (n) + " and k = " + str (k))
        avg_error = {}
        for alg in algorithms:
            avg_error[alg] = 0.0

        for i in range (m):
            # generate problem input
            tst_size = int (sample_size * .5)
            dataset_name = 'parity_' + str (n) + "_" + str (k) + \
                "_" + str (i) + ".dat"
            dataset_file = "input/tmp/" + dataset_name
            trn_dataset_file = "input/tmp/trn_" + dataset_name
            tst_dataset_file = "input/tmp/tst_" + dataset_name
            sys_call = 'python ' + " ".join (str (x) for x in 
                [gen_parity_path, n, k, sample_size + tst_size]) + \
                " > " + dataset_file
            os.system (sys_call)
            os.system ("head -n " + str (sample_size) + " " + \
                dataset_file + " > " + tst_dataset_file)
            os.system ("tail -n " + str (tst_size) + " " + 
                dataset_file + " > " + trn_dataset_file)
            
            # validate with each algorithm
            for alg in algorithms:
                # Performs feature selection
                result = subprocess.check_output (["bin/featsel", 
                     "-f", trn_dataset_file,
                     "-n", str (n),
                     "-l", "2",
                     "-c", cost_functions[alg],
                     "-a", alg])
                result = result.decode ()
                matching = re.search ('<(\d+)\>\s+\:\s+(\S+)', result)
                selected_features = matching.group (1)
                
                # Validates 
                result = subprocess.check_output (["perl",
                    "bin/svm_validation.pl", trn_dataset_file,
                    tst_dataset_file, str (n), selected_features])
                result = result.decode ()
                matching = re.search ('.*:\s+(\d+\.?\d*).*', result)
                v_error = float (matching.group (1))
                avg_error[alg] += v_error

            # delete data 
            os.system ("rm -f " + tst_dataset_file)
            os.system ("rm -f " + trn_dataset_file)
            os.system ("rm -f " + dataset_file)

        
        mesh_X.append (n)
        mesh_Y.append (k)
        for alg in algorithms:
            print ("For n = " + str (n) + ", k = " + str (k) + " and alg = " + alg)
            avg_error[alg] /= m
            print (avg_error[alg])
            print ("\n")
            if (avg_error[alg] < min_err[alg]):
                min_err[alg] = avg_error[alg]
            mesh_Z[alg].append (avg_error[alg])


# Outputs results to a file
f = open ("output/parity_comp_data_m" + str (sample_size) + ".txt", "w")
for i in range (len (mesh_X)):
    n = str (mesh_X[i])
    k = str (mesh_Y[i])
    f.write (n + " " + k + " ")
    for alg in algorithms:
        f.write (str (mesh_Z[alg][i]) + " ")
    f.write ("\n")

# Calculates intersection surface between pucs and spec_cmi
inter_X = []
inter_Y = []
inter_Z = []
i = 0
while i < len (mesh_X):
    if (mesh_Z["pucs"][i] == mesh_Z["spec_cmi"][i]):
        inter_X.append (mesh_X[i] * 1.0001)
        inter_Y.append (mesh_Y[i] * 1.0001)
        inter_Z.append (mesh_Z["pucs"][i] * 1.0001)
    i += 1 

surf_data1 = [
    go.Mesh3d (x=mesh_X, y=mesh_Y, z=mesh_Z["pucs"], 
        color='rgb(200, 50, 50)',
        opacity=1),
     go.Mesh3d (x=mesh_X, y=mesh_Y, z=mesh_Z["spec_cmi"], 
        color='rgb(50, 50, 200)',
        opacity=1),
    go.Mesh3d (x=inter_X, y=inter_Y, z=inter_Z,
        color='#FF00FF',
        opacity=.5)
]

layout = go.Layout (
    title='Average error on Parity problem',
    scene=dict (
        xaxis=dict (
            title='Number of features.'
        ),
        yaxis=dict (
            title='Number of relevant features.',
        ),
        zaxis=dict (
            title='Average error.',
        )
    )
)

fig1 = go.Figure (data=surf_data1, layout=layout)
plotly.offline.plot(fig1, 
        filename='output/parity_plot_m' + str (sample_size) + '.html', 
        auto_open=False)
