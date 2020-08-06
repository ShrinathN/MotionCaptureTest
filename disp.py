#!/bin/python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import filter

data = pd.read_csv('out.csv')

z = list(data['accel_z'])
z_win = filter.window_filter(z)
z_lpf = filter.LPF(z)
z_lpf02 = filter.LPF(z, 0.2)

n = [i for i in range(0, len(z))]

figs, axes = plt.subplots(2,2)


axes[0, 0].plot(n,z)
axes[0, 0].set_title('Z')
axes[0, 1].plot(n,z_win)
axes[0, 1].set_title('Z Window')
axes[1, 0].plot(n,z_lpf)
axes[1, 0].set_title('Z LPF')
axes[1, 1].plot(n,z_lpf02)
axes[1, 1].set_title('Z LPF 0.2')

plt.show()