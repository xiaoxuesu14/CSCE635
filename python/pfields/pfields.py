import numpy as np
from math import cos, sin, pi, atan2
from scipy.integrate import odeint
import matplotlib.pyplot as plt

class pfield:
    def __init__(self,gainfun,origin,gain_static = 1):
        """ gainfun([xy]): multiplies the  output function, handles conditionals """
        self._gainfun = gainfun
        """ origin: 2 length numpy vector of either the center of the pfield or a point on a line it acts along """
        self.origin = np.array([origin[0],origin[1]])
        """ static gain: can be used as a baseline for competing fields of nonuniform strength """
        self._gain_static = gain_static
    def inertial2localframe(self,inertialxy):
        # convert to linear distance along and orthogonal to the line of action from the origin
        rxo_n = inertialxy - self.origin
        return rxo_n
    def inertial2polarframe(self,inertialxy):
        # convert to r, theta centered at the source
        rxo_n = self.inertial2localframe(inertialxy)
        rho = np.linalg.norm(rxo_n)
        theta = atan2(rxo_n[1],rxo_n[0])
        return (rho,theta)

class pfield_uniform(pfield):
    def __init__(self,direc,gainfun,origin,gain_static):
        """ direction along which we act """
        self.direc = direc
        """ standard init """
        pfield.__init__(self,gainfun,origin,gain_static)
    def inertial2localframe(self,inertialxy):
        # convert to linear distance along and orthogonal to the line of action from the origin
        rxo_n = inertialxy - self.origin
        # rotate to local coordinate
        rxo_l = np.zeros(2)
        rxo_l[0] = cos(self.direc)*rxo_n[0] + sin(self.direc)*rxo_n[1]
        rxo_l[1] = -sin(self.direc)*rxo_n[0] + cos(self.direc)*rxo_n[1]
        return rxo_l
    """ compute the potential field strength at a given point """
    def output(self,inertialxy):
        lclxy = self.inertial2localframe(inertialxy)
        G = self._gainfun(lclxy)
        # output is the gain times the direction of action
        y = self._gain_static*G*np.array([cos(self.direc),sin(self.direc)])
        return y

class pfield_perpendicular(pfield_uniform):
    """ compute the potential field strength at a given point """
    def output(self,inertialxy):
        lclxy = self.inertial2localframe(inertialxy)
        G = self._gainfun(lclxy)
        # if local Y is positive, point DOWN; else, point UP
        if lclxy[1] > 0:
            diract = -np.array([-sin(self.direc),cos(self.direc)])
        else:
            diract = np.array([-sin(self.direc),cos(self.direc)])
        y = self._gain_static*G*diract
        return y

class pfield_radial(pfield):
    """ standard init function """
    """ output the gain times the radial direction in the inertial frame """
    """ negative gain is ATTRACTIVE """
    def output(self,inertialxy):
        rxo = self.inertial2localframe(inertialxy)
        # normalize rxo
        rxo = rxo/np.linalg.norm(rxo)
        (r,th) = self.inertial2polarframe(inertialxy)
        G = self._gainfun(r,th)
        # output
        return self._gain_static*G*rxo

class pfield_tangential(pfield):
    """ output the gain times the tengential direction, which is counterclockwise by default """
    def output(self,inertialxy):
        rxo = self.inertial2localframe(inertialxy)
        # normalize rxo
        rxo = rxo/np.linalg.norm(rxo)
        # cross [0;0;1] with it to get the tangential vector
        rtan = np.array([-rxo[1],rxo[0]])
        (r,th) = self.inertial2polarframe(inertialxy)
        G = self._gainfun(r,th)
        # output
        return self._gain_static*G*rtan


""" governing equation of motion when the field is used to set the velocity """
def eqom_field_vel(y,t,fields):
    """ y[0,1]: inertial pos """
    vel = np.zeros(2)
    for k in range(len(fields)):
        vel = vel + fields[k].output(y)
    dy = vel
    return dy

def test():
    import pfields_test
    #pfields_test.test_uniform_perp_fields()
    #pfields_test.test_radial_tangential_fields()
    pfields_test.test_radial_fields()
    

if __name__ == '__main__':
    test()
