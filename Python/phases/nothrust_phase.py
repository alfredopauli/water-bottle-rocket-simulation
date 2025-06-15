import setup


def simulateNoThrustPhase(ypos_0, yvel_0, dt):
    time = 0
    ypos = ypos_0
    yvel = yvel_0
    m = setup.bottleconst["dry_mass"]
    
    rho_air = setup.physicsconst["air_density"]
    g = setup.physicsconst["gravity_acc"]
    
    dc = setup.bottleconst["drag_coeficient"]
    ab = setup.bottleconst["sec_area"]
    
    time_points = list()
    ypos_points = list()

    while (ypos > 0):
        drag_force = -1/2 * dc * rho_air * ab * yvel * abs(yvel)
        yvel += (drag_force / m - g) * dt
        ypos += yvel * dt
        time += dt
        
        time_points.append(time)
        ypos_points.append(ypos)
    
    return {
        "time": time_points,
        "ypos": ypos_points
    }

