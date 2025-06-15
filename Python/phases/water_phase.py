import setup


def B1(h, points=100):
    z = 0
    dz = (h - z) / points
    value = 0

    for _ in range(points):
        value += (setup.getBottleSection(0) / setup.getBottleSection(z)) * dz
        z += dz
    
    return value

def B2(h):
    return 1/2 * ((setup.getBottleSection(0) / setup.getBottleSection(h))**2 - 1)

def B3(h):
    gamma = setup.physicsconst["gamma"]
    vb = setup.bottleconst["volume_bottle"]
    vw0 = setup.bottleconst["volume_water0"]
    return ((vb - vw0) / (vb - setup.getWaterVolume(h)))**gamma

def getMass(h):
    rho_air = setup.physicsconst["air_density"]
    rho_water = setup.physicsconst["water_density"]
    mb = setup.bottleconst["dry_mass"]
    vb = setup.bottleconst["volume_bottle"]
    vw0 = setup.bottleconst["volume_water0"]
    return mb + rho_air * (vb - vw0) + rho_water * setup.getWaterVolume(h)

def simulateWaterPhase(ypos_0, yvel_0, p, t, dt):
    rho_water = setup.physicsconst["water_density"]
    rho_air = setup.physicsconst["air_density"]
    g = setup.physicsconst["gravity_acc"]
    patm = setup.physicsconst["atm_pressure"]
    gamma = setup.physicsconst["gamma"]
    
    dc = setup.bottleconst["drag_coeficient"]
    ab = setup.bottleconst["sec_area"]
    vb = setup.bottleconst["volume_bottle"]
    vw0 = setup.bottleconst["volume_water0"]

    ypos = ypos_0
    yvel = yvel_0
    yacc = 0
    uout = 0
    duout = 0
    h = setup.getWaterLevel()
    dh = 0
    
    time = 0
    
    time_points = list()
    ypos_points = list()
    yvel_points = list()
    h_points = list()
    uout_points = list()

    while (h > 0):
        time_points.append(time)
        ypos_points.append(ypos)
        yvel_points.append(yvel)
        h_points.append(h)
        uout_points.append(uout)
        
        thrust_force = rho_water * setup.getBottleSection(0) * uout**2
        intern_force = -rho_water * setup.getBottleSection(0) * \
            (h*duout + (setup.getBottleSection(0)/setup.getBottleSection(h))*uout**2)
        drag_force = -1/2 * dc * rho_air * ab * yvel * abs(yvel)
        
        # Update Y
        yacc = (thrust_force + intern_force + drag_force)/getMass(h) - g
        yvel += yacc * dt
        ypos += yvel * dt
        
        # Update Uout
        duout = -B2(h) * uout**2 - B3(h) * p / rho_water + patm / rho_water - (yacc + g) * h
        duout /= B1(h)
        uout += duout * dt
        
        # Update H
        dh = setup.getBottleSection(0) * uout / setup.getBottleSection(h)
        h += dh * dt
        
        time += dt
    
    pressure = p*((vb - vw0)/vb)**gamma
    temperature = t*((vb - vw0)/vb)**(1 - gamma)

    return {
        "time": time_points,
        "ypos": ypos_points,
        "yvel": yvel_points,
        "h": h_points,
        "uout": uout_points,
        "pressure": pressure,
        "temperature": temperature
    }


