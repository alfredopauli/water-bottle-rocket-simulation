from math import pi
from math_util import interpolate


# PHYSICS ----------------
# Define physical constants
physicsconst = dict()
physicsconst["atm_pressure"] = 1.01e5      # Pa
physicsconst["gamma"] = 7/5                # Rat
physicsconst["gravity_acc"] = 9.8          # m/s^2
physicsconst["water_density"] = 997        # kg/m^3
physicsconst["air_density"] = 1.225        # kg/m^3
physicsconst["temperature"] = 273 + 20     # K
physicsconst["gas_constant"] = 8.314/0.028 


# BOTTLE ----------------
# Define bottle cosntants
bottleconst = dict()
bottleconst["drag_coeficient"] = 0.5
bottleconst["initial_pressure"] = 7e5
bottleconst["volume_launch_tube"] = 0.0005
bottleconst["volume_bottle"] = 2.1 / 1000
bottleconst["volume_water0"] = 0.5 / 1000
bottleconst["dry_mass"] = 0.1
bottleconst["mass"] = bottleconst["dry_mass"] + bottleconst["volume_water0"] * physicsconst["water_density"]
bottleconst["launch_tube_size"] = 0.15
bottleconst["external_xsection_launch_tube"] = 0.0045**2 * pi
bottleconst["internal_xsection_launch_tube"] = 0.0045**2 * pi
bottleconst["bottle_radius_1"] = 0.0022
bottleconst["bottle_radius_2"] = 0.0525
bottleconst["bottle_height"] = 0.27
bottleconst["nozzle_size_1"] = 0.015
bottleconst["nozzle_size_2"] = 0.075
bottleconst["sec_area"] = bottleconst["bottle_radius_1"]**2 * pi

# Define bottle countour
def getBottleRadius(y):
    if 0 <= y and y < bottleconst["nozzle_size_1"]:
        return bottleconst["bottle_radius_1"]
    
    if bottleconst["nozzle_size_1"] <= y and y < bottleconst["nozzle_size_2"]:
        return interpolate(y, 
            bottleconst["nozzle_size_1"],
            bottleconst["nozzle_size_2"],
            bottleconst["bottle_radius_1"],
            bottleconst["bottle_radius_2"])

    if bottleconst["nozzle_size_2"] <= y and y <= bottleconst["bottle_height"]:
        return bottleconst["bottle_radius_2"]
    
    else:
        print("[WARNING]: y is outside the bottle.")
        return 0

