#!/usr/bin/env python
# coding: utf-8
# npm install -g firebase-tools // Run in cmd in windows
#pip install firebase
#pip install gcloud
#pip install python_jwt
#pip install sseclient
#pip install pycryptodome
#pip install requests-toolbelt
#pip install simplekml

from firebase import Firebase
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from datetime import datetime
import time
import csv
import pandas as pd
import os
import simplekml


#configuration to the database Firebase
config = {
  "apiKey": "AIzaSyA4fHNEMAnGEEJV87kFC0GHzMMRc0TatuA",
  "authDomain": "proberequest-finalproject.firebaseapp.com",
  "databaseURL": "https://proberequest-finalproject-default-rtdb.firebaseio.com",
  "storageBucket": "proberequest-finalproject.appspot.com"
}



firebase = Firebase(config)
db = firebase.database()
usersFrom = dict(db.child("Users").get().val())
listOfLocations = {}

for sensor in usersFrom.keys():    
    location = usersFrom[sensor]["Location"]
    usersFrom[sensor].pop("Location")
    listOfLocations[sensor] = {"location":location}
    print(location)

while(True):
    dictionaryForPlot ={}
    db = firebase.database()
    usersFrom = dict(db.child("Users").get().val())
    usersFrom[sensor].pop("Location")
    print(usersFrom)
    db.child("Users").remove()
    now = datetime.now()
    current_time = (now.strftime("%m-%d-%y-%H-%M-%S"))
    kml = simplekml.Kml(open=1) # the folder will be open in the table of contents
    for sensor in usersFrom.keys():
        numofprobes = 0
        pd.DataFrame.from_dict(usersFrom[sensor], orient ='index').to_csv("DATA/probeRequest"+str(current_time)+".csv",mode='a')
        print(usersFrom[sensor])
        listOfLocations[sensor]["numofprobes"] = len(usersFrom[sensor].values())
        print(listOfLocations)


        #creating kml file 
        temp_location = tuple(float(x) for x in listOfLocations[sensor]["location"].split(","))
        print(temp_location)
        single_point = kml.newpoint(name=sensor, description = "The number of probe requests:" + str(listOfLocations[sensor]["numofprobes"]), coords=[temp_location[::-1]])
    kml.save("DATA/00 Points.kml")
        
    time.sleep(60*60)


