import numpy as np
from matplotlib import pyplot as plt

import setup 
from phases import launch_phase
from phases import water_phase
from phases import gas_phase
from phases import nothrust_phase


if __name__ == "__main__":

    # TODO: Implement interface in webbrowser? Django?

    # Plot bottle contour
    X  = [i * setup.bottleconst["bottle_height"] / 100.0 for i in range(100)]
    Y  = [setup.getBottleRadius(x) for x in X]
    Yp = [-setup.getBottleRadius(x) for x in X]
    
    # Simulate launch phase
    launch_phase_solution = launch_phase.simulateLaunchPhase(0.0001, 1)
    
    # Simulate water phase
    water_phase_solution = water_phase.simulateWaterPhase(
        launch_phase_solution["ypos"][-1],
        launch_phase_solution["yvel"][-1],
        launch_phase_solution["pressure"],
        launch_phase_solution["temperature"],
        0.0001
    )
    
    # Simulate gas phase
    gas_phase_solution = gas_phase.simulateGasPhase(
        water_phase_solution["ypos"][-1],
        water_phase_solution["yvel"][-1],
        water_phase_solution["pressure"],
        water_phase_solution["temperature"],
        0.0001
    )

    # Simulate no thrust phase
    nothrust_phase_solution = nothrust_phase.simulateNoThrustPhase(
        gas_phase_solution["ypos"][-1],
        gas_phase_solution["yvel"][-1],
        0.0001
    )
    
    # Final
    final_time = list()
    final_ypos = list()
    
    final_time.extend(launch_phase_solution["time"])
    final_ypos.extend(launch_phase_solution["ypos"])
    
    last_time = final_time[-1]
    for (time, ypos) in zip(water_phase_solution["time"], water_phase_solution["ypos"]):
        final_time.append(last_time + time)
        final_ypos.append(ypos)
        pass
    
    last_time = final_time[-1]
    for (time, ypos) in zip(gas_phase_solution["time"], gas_phase_solution["ypos"]):
        final_time.append(last_time + time)
        final_ypos.append(ypos)
        pass
    
    last_time = final_time[-1]
    for (time, ypos) in zip(nothrust_phase_solution["time"], nothrust_phase_solution["ypos"]):
        final_time.append(last_time + time)
        final_ypos.append(ypos)
        pass
    
    ax_config = {"xlabel": "Time [s]", "ylabel": "Height [m]"}
    
    launch_ax = plt.subplot(2, 4, 1, title="Launch phase", **ax_config)
    water_ax = plt.subplot(2, 4, 2, title="Water phase", **ax_config)
    gas_ax = plt.subplot(2, 4, 5, title="Gas phase", **ax_config)
    nothrust_ax = plt.subplot(2, 4, 6, title="No Thrust phase", **ax_config)
    final_ax = plt.subplot(1, 2, 2, title="Final phase", **ax_config)
    
    launch_ax.plot(launch_phase_solution["time"], launch_phase_solution["ypos"])
    water_ax.plot(water_phase_solution["time"], water_phase_solution["ypos"])
    gas_ax.plot(gas_phase_solution["time"], gas_phase_solution["ypos"])
    nothrust_ax.plot(nothrust_phase_solution["time"], nothrust_phase_solution["ypos"])
    final_ax.plot(final_time, final_ypos)
    
    plt.tight_layout(w_pad=0, h_pad=0)
    plt.show()
    

