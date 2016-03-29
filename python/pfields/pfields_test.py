import numpy as np
from math import pi
from scipy.integrate import odeint
import matplotlib.pyplot as plt
import pfields

def test_uni_gain(yin):
    return 1

def test_perp_gain(xyin):
    return xyin[1]

def test_uniform_perp_fields():
    uni = pfields.pfield_uniform(pi*0.25,test_uni_gain,(1,0))
    perp = pfields.pfield_perpendicular(pi*0.25,test_perp_gain,(1,0))
    init = (0,0)
    # simulate
    tint = np.linspace(0.0,10.0,100)
    y = odeint(pfields.eqom_field_vel,init,tint,args=((uni,perp),))

    # figure
    fig = plt.figure()
    ax = fig.add_subplot(111,ylabel='y',xlabel='x',title='Uniform and perpindicular field tests')
    ax.plot(y[:,0],y[:,1])
    ax.plot([1,y[-1,0]],[0,y[-1,0]-1],'r--')
    ax.grid()
    
    fig.show()
    raw_input("Return to exit")
    plt.close(fig)

def test_attractive_gain(r,th):
    if r > 1.0:
        g = -(r-1.0)
    else:
        g = 0
    return g
def test_tangential_gain(r,th):
    g = 1/r
    return g

def test_radial_tangential_fields():
    orig = (1,1)
    att = pfields.pfield_radial(test_attractive_gain,orig)
    tan = pfields.pfield_tangential(test_tangential_gain,orig)

    init = (0,0)
    # simulate
    tint = np.linspace(0.0,5.0,100)
    y = odeint(pfields.eqom_field_vel,init,tint,args=((att,tan),))

    # figure
    fig = plt.figure()
    ax = fig.add_subplot(111,ylabel='y',xlabel='x',title='Attractive and tangential field tests')
    ax.plot(y[:,0],y[:,1])
    # draw unit circle at (1,1)
    th = np.linspace(0,2*pi,20)
    circ = np.zeros((20,2))
    circ[:,0] = np.cos(th) + orig[0]
    circ[:,1] = np.sin(th) + orig[1]
    ax.plot(circ[:,0],circ[:,1],'r--')
    ax.grid()
    
    fig.show()
    raw_input("Return to exit")
    plt.close(fig)

def test_radial_fields():

    orig1 = (2,1)
    att1 = pfields.pfield_radial(test_attractive_gain,orig1,2)
    orig2 = (-2,1)
    att2 = pfields.pfield_radial(test_attractive_gain,orig2,1)

    init = (0,0)
    # simulate
    tint = np.linspace(0.0,5.0,100)
    y = odeint(pfields.eqom_field_vel,init,tint,args=((att1,att2),))

    # figure
    fig = plt.figure()
    ax = fig.add_subplot(111,ylabel='y',xlabel='x',title='Competing attractive fields test')
    ax.plot(y[:,0],y[:,1])
    # draw unit circle at (1,1)
    th = np.linspace(0,2*pi,20)
    circ = np.zeros((20,2))
    circ[:,0] = np.cos(th) + orig1[0]
    circ[:,1] = np.sin(th) + orig1[1]
    ax.plot(circ[:,0],circ[:,1],'r--')
    # draw unit circle at (1,1)
    th = np.linspace(0,2*pi,20)
    circ = np.zeros((20,2))
    circ[:,0] = np.cos(th) + orig2[0]
    circ[:,1] = np.sin(th) + orig2[1]
    ax.plot(circ[:,0],circ[:,1],'r--')
    ax.grid()
    
    fig.show()
    raw_input("Return to exit")
    plt.close(fig)
    
