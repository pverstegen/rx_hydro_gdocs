#-------------------------------------------------------------------------------
# Name:        RX_Hydro_GDOCS
# Purpose:     Pulls serial data from Arduino (rx_hydro_RPI) and logs to Google Docs
#
# Author:      Paul Verstegen <verstegen.paul@gmail.com>
#
# Created:     04/11/2013
# Copyright:   (c) Paul 2013
# Licence:     GPL v2
# Thank you to Adafruit for a great gspread tutorial!
# Thank you to Anton Burnashev for the gspread library!
#-------------------------------------------------------------------------------

import subprocess
import re
import sys
import time
import datetime
import gspread
import serial
import os

# Setup timing and how long it will run

time_between = 300 # in seconds, 300 = 5 minutes
cycles = 24 # 2 hours @ 5 minutes
passed = 0 # holds number of completed cycles

# ===========================================================================
# Google Account Details
# ===========================================================================

# Account details for google docs
email       = 'emailgoeshere'
password    = 'passwordgoeshere'
spreadsheet = 'spreadsheetgoeshere'

# Login with your Google account
try:
  gc = gspread.login(email, password)
except:
  print "Unable to log in.  Check your email address/password"
  sys.exit()

# Open a worksheet from your spreadsheet using the filename
try:
  worksheet = gc.open(spreadsheet).sheet1

except:
  print "Unable to open the spreadsheet"
  sys.exit()

s = serial.Serial('/dev/ttyUSB0', 57600, timeout = 0.05)


def gethydro():
    s.flushInput()
    s.write('66/')
    #time.sleep(2)
    hydro = s.read(6)
    s.flushInput()
    return hydro

def getlight():
    s.flushInput()
    s.write('67/')
    #time.sleep(2)
    light = s.read(6)
    s.flushInput()
    return light

def gethumid():
    s.flushInput()
    s.write('68/')
    #time.sleep(2)
    humid = s.read(6)
    s.flushInput()
    return humid

def gettemp():
    s.flushInput()
    s.write('69/')
    #time.sleep(2)
    temp = s.read(6)
    s.flushInput()
    return temp

while (passed <= cycles):
# Update values
    hydro = gethydro()
    light = getlight()
    humid = gethumid()
    temp = gettemp()
# Print for debugging
    print "Soil humidity: "
    print hydro
    print "Light sensor: "
    print light
    print "Air humidity: "
    print humid
    print "Air temp: "
    print temp
    time.sleep(time_between)
    passed += 1
# Append the data in the spreadsheet, including a timestamp
    try:
        values = [datetime.datetime.now(), hydro, light, humid, temp]
        worksheet.append_row(values)
        print "Data written to spreadsheet"
    except:
        print "Unable to append data.  Check your connection?"





