import sys
import numpy as np
import math
import matplotlib.pyplot as plt
import scipy.optimize
import scipy.linalg

sys.path.append('../../../estimation/filters/python/ekf')
import ekf

import gpsParser

## Governing analytical equation of motion
#
# @param x: [x,y,v,hdg]
def stateDerivative(x,t,u):
    f = np.zeros(4,)
    # d/dt(x) = v*cos(hdg)
    f[0] = x[2]*math.cos(x[3])
    # d/dt(y) = v*sin(hdg)
    f[1] = x[2]*math.sin(x[3])
    # d/dt(v) = Gaussian noise
    f[2] = 0.0
    # d/dt(hdg) = Gaussian noise
    f[3] = 0.0
    return f

def stateGradient(x,t,u):
    F = np.zeros((4,4))
    F[0:2,2:4] = [ [math.cos(x[3]),-x[2]*math.sin(x[3])],[math.sin(x[3]),x[2]*math.cos(x[3])] ]
    return F

def processInfluence(x,t,u):
    G = np.zeros((4,2))
    G[2,0] = 1.0
    G[3,1] = 1.0
    return G

def measurementFunction(x,t):
    y = np.zeros(2,)
    y[0] = x[0]
    y[1] = x[1]
    return y

def measurementGradient(x,t):
    H = np.zeros((2,4))
    H[0:2,0:2] = [ [1.0,0.0],[0.0,1.0] ]
    return H

def initFunction(yk):
    xk = np.array([ yk[0],yk[1],1,0 ])
    Pk = np.diag([math.pow(5.0/3.0,2.0),math.pow(5.0/3.0,2.0),1.0,1.0])
    return(xk,Pk)

def main():
    # load data
    filename = '20160328172944.csv'
    dat = gpsParser.parseFile(filename)
    # shorten the data vector
    tst = 186
    a0 = np.nonzero((dat[:,0] - dat[0,0]) > tst)
    dat = dat[a0[0][0]:,:]
    # time vector
    tsim = dat[:,0]-dat[0,0]
    # X-Y data
    Y = dat[:,6:8]
    # set the first point to the origin
    Y[:,0] = Y[:,0] - Y[0,0]
    Y[:,1] = Y[:,1] - Y[0,1]
    # measurement noise estimate
    Rk = np.diag(math.pow(5.0/3.0,2.0)*np.ones(2,))# metres
    # process noise estimate
    Qk = np.diag([10.0,10.0])
    # measurement interval
    dt = 1.0
    EKF = ekf.ekf(4,0,stateDerivative,stateGradient,processInfluence,Qk=Qk)
    # initialize estimate state
    #x0 = np.array([Y[0,0],Y[0,1],1.0,0.0])
    x0 = np.array([Y[0,0],Y[0,1],dat[0,5],dat[0,4]*math.pi/180.0])
    # initial covariance
    P0 = np.diag([Rk[0,0],Rk[1,1],1.0,1.0])
    # initialize the EKF state
    EKF.init_P(x0,P0,0.0)
    # number of steps to propagate
    nSteps = 100
    # log values
    xAll = np.zeros((2*nSteps-1,4))
    tAll = np.zeros(2*nSteps-1,)
    # log posterior values
    xk = np.zeros((nSteps,4))
    Pk = np.zeros((nSteps,4))
    allCounter = 0
    xAll[allCounter,:] = x0.copy()
    tAll[allCounter] = tsim[0]
    for k in range(0,nSteps):
        if k > 0:
            dt = tsim[k]-tsim[k-1]
            # propagate
            EKF.propagateOde(dt)
            # log
            allCounter = allCounter+1
            xAll[allCounter,:] = EKF.xhat.copy()
            tAll[allCounter] = tsim[k]
            # update
            EKF.update(tsim[k],Y[k,:],measurementFunction,measurementGradient,Rk)
            # log all
            allCounter = allCounter+1
            xAll[allCounter,:] = EKF.xhat.copy()
            tAll[allCounter] = tsim[k]
            print(allCounter)
        # log
        xk[k,:] = EKF.xhat.copy()
        Pk[k,:] = np.diag(EKF.Pk.copy())

    # compute propagataion residuals

    # plot results
    fig1 = plt.figure()
    lbls = ['x(m)','y(m)','v(m/s)','hdg(rad)']
    tplot = tsim[:nSteps]
    ax = []
    for k in range(4):
        ax.append(fig1.add_subplot(2,2,k+1,ylabel=lbls[k]))
        #ax[k].plot(tplot,xk[:,k])
        ax[k].plot(tAll,xAll[:,k])
        if k < 2:
            ax[k].plot(tsim,Y[:,k],'r--')
        elif k == 2:
            ax[k].plot(tsim,dat[:,5],'r--')
        elif k == 3:
            ax[k].plot(tsim,dat[:,4]*math.pi/180.0,'r--')
    fig1.show()

    fig2 = plt.figure()
    lbls = ['x(m)','y(m)','v(m/s)','hdg(rad)']
    tplot = tsim[:nSteps]
    # residuals
    e = np.zeros((nSteps,4))
    e[:,0] = xk[:,0] - Y[:nSteps,0]
    e[:,1] = xk[:,1] - Y[:nSteps,1]
    e[:,2] = xk[:,2] - dat[:nSteps,5]
    e[:,3] = xk[:,3] - dat[:nSteps,4]*math.pi/180.0
    ax = []
    for k in range(4):
        ax.append(fig2.add_subplot(2,2,k+1,ylabel=lbls[k]))
        ax[k].plot(tplot,e[:,k])
        ax[k].plot(tplot,3.0*np.sqrt(Pk[:,k]),'r--')
        ax[k].plot(tplot,-3.0*np.sqrt(Pk[:,k]),'r--')
    fig2.show()

    raw_input('Return to exit')
    plt.close(fig1)

if __name__ == '__main__':
    main()
