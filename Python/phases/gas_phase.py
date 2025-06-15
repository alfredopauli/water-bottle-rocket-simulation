import setup
from math import sqrt


def getMass(rho):
    mb = setup.bottleconst["dry_mass"]
    vb = setup.bottleconst["volume_bottle"]
    return mb + rho * vb


def simulateGasPhase(ypos_0, yvel_0, p, t, dt):
    patm = setup.physicsconst["atm_pressure"]
    gamma = setup.physicsconst["gamma"]
    rm = setup.physicsconst["gas_constant"]
    rho_air = setup.physicsconst["air_density"]
    g = setup.physicsconst["gravity_acc"]

    vw0 = setup.bottleconst["volume_water0"]
    vb = setup.bottleconst["volume_bottle"]
    sec_area0 = setup.bottleconst["sec_area0"]
    dc = setup.bottleconst["drag_coeficient"]
    ab = setup.bottleconst["sec_area"]
    
    tau = vb/(sec_area0*sqrt(gamma*rm*t))\
          *(2/(gamma-1))\
          *((gamma+1)/2)**((gamma+1)/(2*(gamma-1)))

    time = 0
    ypos = ypos_0
    yvel = yvel_0
    rho0 = rho_air*(vb - vw0)/(vb)
    pressure = p
    
    time_points = list()
    ypos_points = list()
    yvel_points = list()

    while (pressure > patm):
        pressure = p * (1 + time/tau)**((2*gamma)/(1-gamma)) 
        rho = (pressure / p)**(1/gamma)*rho0

        thrust_force = 2*pressure*sec_area0*(2/(gamma + 1))**(1/(gamma - 1)) - pressure*sec_area0
        drag_force = -1/2 * dc * rho_air * ab * yvel * abs(yvel)

        yacc = (thrust_force + drag_force) / getMass(rho) - g
        yvel += yacc * dt
        ypos += yvel * dt
        time += dt
        
        time_points.append(time)
        ypos_points.append(ypos)
        yvel_points.append(yvel)
    
    return {
        "time": time_points,
        "ypos": ypos_points,
        "yvel": yvel_points,
    }

