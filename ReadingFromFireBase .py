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
listOfLocations = {"2C:F4:32:49:DB:50" :{ "name" : "2C:F4:32:49:DB:50" , "Location": "32.103968,35.209522"}
                  ,"2C:F4:32:49:E4:A7" :{ "name" : "2C:F4:32:49:E4:A7" , "Location": "32.104937,35.211618"}
                  ,"98:F4:AB:B2:60:2A" :{ "name" : "98:F4:AB:B2:60:2A" , "Location": "32.105562,35.211091"}
                  ,"2C:F4:32:49:DB:57" :{ "name" : "2C:F4:32:49:DB:57" , "Location": "32.104008,35.207790"}
                  ,"98:F4:AB:B2:61:AD" :{ "name" : "98:F4:AB:B2:61:AD" , "Location": "32.105103,35.206729"}}

#for sensor in usersFrom.keys():    
#    location = usersFrom[sensor]["Location"]
#    usersFrom[sensor].pop("Location")
#    listOfLocations[sensor] = {"location":location}
#    print(location)

while(True):
    dictionaryForPlot ={}
    db = firebase.database()
    usersFrom = dict(db.child("Users").get().val())
    #usersFrom[sensor].pop("Location")
    #print(usersFrom)
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
        temp_location = tuple(float(x) for x in listOfLocations[sensor]["Location"].split(","))
        print(temp_location)
        single_point = kml.newpoint(name=listOfLocations[sensor]["name"], description = str(listOfLocations[sensor]["numofprobes"])+ " probe requests caught", coords=[temp_location[::-1]])
    kml.save( "DATA/"+current_time +" Points.kml")
        
    time.sleep(60*60)

