import numpy as np
import matplotlib.pyplot as plt
import subprocess
import re

# feature ranking
# - it's easier to make it hard coded...
rank = [3, 4, 8, 7, 1, 6, 2, 5, 10, 9]
number_of_features = 13
number_of_classes = 3
training_file = 'input/Wine/trn2.dat'
testing_file = ' input/Wine/tst2.dat'

error_list = []
number_of_features_list = []
nof_features = 0

selected_features = [0] * 13
for feature in rank:
    selected_features[feature - 1] = 1
    nof_features += 1
    result = subprocess.check_output (["perl", "bin/ncm_validation.pl", 
        training_file, testing_file, str (number_of_features),
        str (number_of_classes),  
        ''.join (map (str, selected_features))])
    result = result.decode ()

    # validation error: 0.115384615384615
    m = re.search ('error: (\d+.?\d+)', result)
    error_list.append (m.group (1))
    number_of_features_list.append (nof_features)



plt.plot(number_of_features_list, error_list)
axes = plt.gca ()
axes.set_ylim([0, 1])

plt.xlabel ("Number of features")
plt.ylabel ("Validation error")
plt.title ('')
plt.show()