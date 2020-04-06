#!/usr/bin/env python

import pyproj
import numpy

Y_AZIMUTH = 330.0 / 180.0 * numpy.pi
ORIGIN_X = 200000.0
ORIGIN_Y = -400000.0

POINTS = numpy.array([
    [34.7, -117.8, 10.0],
    [34.7, -117.8, 1.0e+5],

    [35.0, -117.6, -45.0e+3],
    [35.0, -117.6, -45.1e+3],

    [35.1, -117.8, -3.0e+3],
    [34.3, -117.8, -3.0e+3],

    [35.0, -117.5, -15.0e+3],
    [35.0, -113.0, -15.0e+3],

    [35.0, -118.2, -25.0e+3],
    [42.0, -117.8, -25.0e+3],
])

csSrc = pyproj.Proj("EPSG:4326")
csDst = pyproj.Proj("EPSG:3311")
xG, yG = pyproj.transform(csSrc, csDst, POINTS[:, 0], POINTS[:, 1])
x = xG - ORIGIN_X
y = yG - ORIGIN_Y

xR = x * numpy.cos(Y_AZIMUTH) - y * numpy.sin(Y_AZIMUTH)
yR = x * numpy.sin(Y_AZIMUTH) + y * numpy.cos(Y_AZIMUTH)

zG = 1.5e+2 + 0.2 * xR - 0.1 * yR + 0.05e-3 * xR * yR

print(zG)

zBot = -45.0e+3
zR = zBot * (zG - POINTS[:, 2]) / (zG - zBot)

for xi, yi, zi in zip(xR, yR, zR):
    print(xi, yi, zi)
