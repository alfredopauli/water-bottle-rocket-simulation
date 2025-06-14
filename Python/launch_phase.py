import setup

# Return list(tuple(time, y_pos) ...)
def simulateLaunchPhase(dt, max_time):
    time = 0
    ypos = 0
    yvel = 0
    
    patm = setup.physicsconst["atm_pressure"]
    gamma = setup.physicsconst["gamma"]
    rho_air = setup.physicsconst["air_density"]
    g = setup.physicsconst["gravity_acc"]
    
    vl = setup.bottleconst["volume_launch_tube"]
    vb = setup.bottleconst["volume_bottle"]
    vw0 = setup.bottleconst["volume_water0"] = 0.5 / 1000
    lt = setup.bottleconst["launch_tube_size"]
    ato = setup.bottleconst["external_xsection_launch_tube"]
    ati = setup.bottleconst["internal_xsection_launch_tube"]
    dc = setup.bottleconst["drag_coeficient"]
    ab = setup.bottleconst["sec_area"]
    m0 = setup.bottleconst["mass"]
    p0 = setup.bottleconst["initial_pressure"] = 7e5

    v_init = vl + vb - vw0 - lt * (ato - ati)
    
    time_points = list()
    y_points = list()

    while (ypos < lt):
        rho = v_init / (v_init + ypos * ato)
        pressure = p0 * rho ** gamma

        impulse_force = (pressure - patm) * ato
        drag_force = -1/2 * dc * rho_air * ab * yvel * abs(yvel)
        
        yacc = (impulse_force + drag_force) / m0
        yvel += (yacc - g) * dt 
        ypos += yvel * dt

        time_points.append(time)
        y_points.append(ypos)

        time += dt
        if (time > max_time):
            print("[WARNING]: could not evaluate, exceed max time")
            break

    return (time_points, y_points)


