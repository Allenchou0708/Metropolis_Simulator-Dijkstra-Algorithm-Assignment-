import matplotlib.pyplot as plt
import numpy as np

x_array= #wait for .cc to generate array
y_array=

side_x= # wait for .cc to generate side-sets
side_y=

plt.plot(x_array,y_array,"ob");

time=len(side_x)
for i in range(time):
  plt.plot([x_array[side_x[i]],x_array[side_y[i]]],[y_array[side_x[i]],y_array[side_y[i]]],"black");
plt.annotate("",(x_array[0],y_array[0]))
plt.show();
