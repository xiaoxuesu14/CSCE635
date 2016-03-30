import sys
import numpy as np
import math

import gpsParser

import scipy.interpolate
import matplotlib.pyplot as plt
import scipy.optimize

def circleFit(x,x1,x2,x3):
    R = x[2]
    x0 = x[0]
    y0 = x[1]
    f1 = math.pow(x1[0]-x0,2.0) + math.pow(x1[1]-y0,2.0) - math.pow(R,2.0)
    f2 = math.pow(x2[0]-x0,2.0) + math.pow(x2[1]-y0,2.0) - math.pow(R,2.0)
    f3 = math.pow(x3[0]-x0,2.0) + math.pow(x3[1]-y0,2.0) - math.pow(R,2.0)
    #f = f1*f1+f2*f2+f3*f3
    f = np.array([f1,f2,f3])
    return f

## @file circleFit.py
## @package circleFit Curve fit to GPS data assuming constant-radius turns between successive GPS points. Seems to work reasonably well.
# @brief Approximates a curve fit for the minimum turn radius as a function of time. More conservative results at low speeds using 20160328172331 data set.
#
# Dependencies: gpsParser package, numpy, scipy, matplotlib, python2

file = '20160328172944.csv'
#file = '20160328172331.csv'
dat = gpsParser.parseFile(file)

# shorten to relevant interval
if file == '20160328172944.csv':
    k1 = np.nonzero((dat[:,0]-dat[0,0] > 322))
    k2 = np.nonzero((dat[:,0]-dat[0,0] < 375))
    kp = np.intersect1d(k1[0],k2[0])
    dat = dat[kp,:]
if file == '20160328172331.csv':
    k1 = np.nonzero((dat[:,0]-dat[0,0] > 170))
    k2 = np.nonzero((dat[:,0]-dat[0,0] < 300))
    kp = np.intersect1d(k1[0],k2[0])
    dat = dat[kp,:]

# extract variables
t = dat[:,0]-dat[0,0]
vel = dat[:,5]
x = dat[:,6]-dat[0,6]
y = dat[:,7]-dat[0,7]

# solve for the turn radius and circle center
xSol = np.zeros((len(t),3))
for k in range(1,len(t)-1):
    x0 = np.array([0.0,0.0,1.0])
    x1 = np.array([x[k-1],y[k-1]])
    x2 = np.array([x[k],y[k]])
    x3 = np.array([x[k+1],y[k+1]])
    xs = scipy.optimize.fsolve(circleFit,x0,args=(x1,x2,x3,))
    #print(k,xs)
    xSol[k,:] = xs

# interpolate for plotting
desn = 50
xyhr = np.zeros(((len(t)-1)*desn,2))
for k in range(1,4):#len(t)-1):
    th2 = math.atan2(y[k+1]-xSol[k,1],x[k+1]-xSol[k,0])
    th1 = math.atan2(y[k-1]-xSol[k,1],x[k-1]-xSol[k,0])
    th = np.linspace(th1,th2,desn)
    #th = np.linspace(0,2.0*math.pi,desn)
    xyhr[ (k*desn):((k+1)*desn),0 ] = xSol[k,2]*np.cos(th) + xSol[k,0]
    xyhr[ (k*desn):((k+1)*desn),1 ] = xSol[k,2]*np.sin(th) + xSol[k,1]

fig1 = plt.figure()
ax1 = fig1.add_subplot(1,1,1,ylabel='y(m)',xlabel='x(m)')
ax1.plot(x,y,'b-d',lw=2)
ax1.plot(xyhr[:,0],xyhr[:,1],'r--',lw=2)
ax1.grid()
fig1.show()


# simple curve fit for turning radius(vel):
# bin velocities every m/s
bins = [2,4,6,8]
minr = [1000.0,1000.0,1000.0,1000.0]
for k in range(len(vel)):
    for j in range(len(bins)):
        #print(k,j,vel[k],math.fabs(xSol[k,2]),bins[j])
        if vel[k] < bins[j]:
            rad = math.fabs(xSol[k,2])
            if minr[j] > rad and rad > 1e-2:# tolerance for reasonableness
                minr[j] = rad
            break
print(bins,minr)
# compute the best-fit
A = np.zeros((len(bins),2))
Yf = np.zeros((len(bins),))
for k in range(len(bins)):
    A[k,0] = float(bins[k])-1.0
    A[k,1] = 1.0
    Yf[k] = math.log10(minr[k])
Xf = np.dot(np.dot(np.linalg.inv( np.dot(A.transpose(),A)),A.transpose()),Yf)
print('log10(radius) = %g*V+%g' % (Xf[0],Xf[1]))
Yp = np.dot(A,Xf)
Ype = np.power(10.0,Yp)
print('Y_pred = %g,%g,%g,%g' % (Ype[0],Ype[1],Ype[2],Ype[3]))
xe = np.linspace(1.0,9.0,10)

fig = plt.figure()
ax = []
ax.append( fig.add_subplot(3,1,1,ylabel='radius(m)') )
ax[0].plot(t,np.abs(xSol[:,2]),lw=2)
ax[0].grid()
#
ax.append( fig.add_subplot(3,1,2,ylabel='log10(radius)(m)',xlabel='V(m/s)') )
ax[1].plot(vel,np.log10(np.abs(xSol[:,2])),'bx',lw=2)
# plot curve fit
ax[1].plot(xe,xe*Xf[0]+Xf[1],'r-',lw=2)
ax[1].grid()
ax.append( fig.add_subplot(3,1,3,ylabel='V(m/s)') )
ax[2].plot(t,vel,lw=2)
ax[2].grid()

fig.set_size_inches((6.25,9),forward=True)
fig1.set_size_inches((6.25,9),forward=True)
fig.show()

raw_input('Return to exit')
plt.close(fig)
plt.close(fig1)
