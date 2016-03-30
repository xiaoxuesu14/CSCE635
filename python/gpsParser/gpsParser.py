import csv
import numpy as np
import matplotlib.pyplot as plt
import math
import sys

## @package gpsParser Functions for reading GPS CSV logs from GPSLogger Android App
# Dependencies: numpy, matplotlib, python2

Re = 6378100 # earth radius in meters
d2r = math.pi/180.0 # convert degrees to radians

def str2time(strObj):
    if type(strObj) is list:
        str2time(strObj[0])
    elif type(strObj) is str:
        if len(strObj) >= 20:
            hr = int(strObj[11:13])*3600
            mn = int(strObj[14:16])*60
            se = int(strObj[17:19])
            return (hr+mn+se)
        else:
            print(strObj)
            return

## convert GPS time, latitude (deg), longitude (deg) to coordinates in the Earth-fixed earth-centered reference frame
#
# Assume that we are at the mean equatorial radius of Earth for all calcs
# @param[out] R Numpy array size [N x 3], each column is a component of the position in the ECEF frame
#
def gps2ecef(lat,lon):
    R = np.zeros([len(lat),3])
    for k in range(len(lat)):
        lam = d2r*lat[k]
        phi = d2r*lon[k]
        R[k,:] = Re*[math.cos(lam),-math.sin(lam)*math.sin(phi),math.sin(lam)*math.cos(phi)]
    return R

## convert GPS history to position in a local NED frame centered at the first measurement.
def gps2ned(lat,lon):
    lam0 = d2r*lat[0]
    phi0 = d2r*lon[0]
    return -1

## parse a logged GPS file and return the log history.
#
# @param[in] filename string giving the path to CSV file
# @param[out] dat numpy [N x 9] array: [time (sec),lat (deg), lon(deg), GPS bearing(deg),GPS speed (m/s), X (m), Y(m)]
#
def parseFile(filename):
    Re = 6378100 # earth radius in meters
    d2r = math.pi/180.0 # convert degrees to radians

    # size of the file is N x 9
    # columns: [time, lat, lon, elevation, accuracy, bearing, speed, satellites, provider]

    # save to format: [time lat lon bearing speed X Y <> <>]
    with open(filename,'rb') as csvfile:
        reader = csv.reader(csvfile,delimiter=',')
        dat = np.zeros([1e4,9])
        datLen = 0
        for row in reader:
            # make sure it's not the first row
            if not (row[0] == 'time'):
                # make sure the source is GPS
                source = row[8]
                if source == 'gps':
                    dat[datLen,0] = str2time(row[0])# time (sec)
                    dat[datLen,1] = float(row[1])# lat (deg)
                    dat[datLen,2] = float(row[2])# lon (deg)
                    dat[datLen,3] = float(row[4])# acc (m)
                    dat[datLen,4] = float(row[5])# bearing (deg)
                    dat[datLen,5] = float(row[6])# speed (m/s)
                    dat[datLen,6] = Re*d2r*dat[datLen,2] # X (m)
                    dat[datLen,7] = Re*d2r*dat[datLen,1] # Y (m)
                    datLen = datLen + 1
        print ("Read %d lines" % datLen)
    if not ('dat' in locals()):
        print('File not read')
        return -1
    # remove unused zeros
    dat = dat[0:datLen,:]
    return dat

## command-line execution to generate plots from a data set
# @brief usage: '-f filename' followed by optional arguments '-plot' and '-map'
#
#  '-plot' generates a time history plot, -map generates a 2D X-Y flat-earth plot. example use: 'python gpsParser -f 20160328172944.csv -plot -map'
def main():
    # current index
    kst = 1
    if len(sys.argv) > 1:
        # parse input arguments
        if sys.argv[1] == '-f':
            filename = sys.argv[2]
            kst = 3
        else:
            filename = '20160326175432.csv' # default
            kst = 1
    dat = parseFile(filename)
    if kst == len(sys.argv):
        plotFunction(dat)
    else:
        for jk in range(kst,len(sys.argv)):
            if sys.argv[jk] == '-plot':
                plotFunction(dat)
            if sys.argv[jk] == '-map':
                plot2D(dat)

def plot2D(dat):
    print("Plot 2D")
    # plot lat/lon as 2D
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1,ylabel='X(m)',xlabel='Y(m)')
    ax.plot(dat[:,6],dat[:,7])
    fig.show()
    raw_input('Return to continue')
    plt.close(fig)
    return

def plotFunction(dat):
    print("Plot histories")
    # plot figure
    fig = plt.figure()
    ax = []
    plotks = [1,2,4,5,6,7]#indicaes to plot
    lbls = ['lat(deg)','lon(deg)','bearing(deg)','speed(m/s)','X(m)','Y(m)']
    for k in range(6):
        j = plotks[k]
        ax.append( fig.add_subplot(2,3,k+1,ylabel=lbls[k]) )
        ax[k].plot(dat[:,0],dat[:,j])
    fig.show()
    raw_input('Return to exit')
    plt.close(fig)

if __name__ == "__main__":
    main()
