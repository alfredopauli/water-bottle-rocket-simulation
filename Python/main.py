import numpy as np
from matplotlib import pyplot as plt

import setup 
import launch_phase


if __name__ == "__main__":

    # TODO: Implement interface in webbrowser? Django?

    # Plot bottle contour
    X  = [i * setup.bottleconst["bottle_height"] / 100.0 for i in range(100)]
    Y  = [setup.getBottleRadius(x) for x in X]
    Yp = [-setup.getBottleRadius(x) for x in X]

    plt.plot(X, Y)
    plt.plot(X, Yp)
    plt.show()
    
    # Simulate launch phase
    time, y = launch_phase.simulateLaunchPhase(0.0001, 1)
    plt.plot(time, y)
    plt.show()
    
    # Simulate water phase

