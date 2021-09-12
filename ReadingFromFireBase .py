### This script is created for reading inforamtion from firebase database and convert it to csv file, ###
### the script can also create KML file for google earth with the information of each sensor connected to the database. ###

# Dependencies that need to be installed first

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

# Configuration to the database Firebase
config = {
  "apiKey": "Api key of the database",
  "authDomain": "Enter your authentication code",
  "databaseURL": "Enter the database url",
  "storageBucket": "Enter storage bucket code"
}


# Creating firebase variables
firebase = Firebase(config)
db = firebase.database()
usersFrom = dict(db.child("Users").get().val()) # the address of the information in the database

# List of the locations of the sensors
listOfLocations = {"2C:F4:32:49:DB:50" :{ "name" : "2C:F4:32:49:DB:50" , "Location": "32.103968,35.209522"}
                  ,"2C:F4:32:49:E4:A7" :{ "name" : "2C:F4:32:49:E4:A7" , "Location": "32.104937,35.211618"}
                  ,"98:F4:AB:B2:60:2A" :{ "name" : "98:F4:AB:B2:60:2A" , "Location": "32.105562,35.211091"}
                  ,"2C:F4:32:49:DB:57" :{ "name" : "2C:F4:32:49:DB:57" , "Location": "32.104008,35.207790"}
                  ,"98:F4:AB:B2:61:AD" :{ "name" : "98:F4:AB:B2:61:AD" , "Location": "32.105103,35.206729"}}

while(True):
  
    dictionaryForPlot ={} # dictonary for the information gatherd from the firebase
    db = firebase.database() # reading the database
    usersFrom = dict(db.child("Users").get().val())
    db.child("Users").remove() # clear the database from old information
    
    # Save the current time for the file name
    now = datetime.now() 
    current_time = (now.strftime("%m-%d-%y-%H-%M-%S"))
    
    # Creating simple KML file
    kml = simplekml.Kml(open=1) 
    
    # For each sensor read the content from the database and create dataframe for it
    for sensor in usersFrom.keys():
        # Saving to dataframe the information from the dict
        pd.DataFrame.from_dict(usersFrom[sensor], orient ='index').to_csv("DATA/probeRequest"+str(current_time)+".csv",mode='a')
        
        # Save the number of probe requests for each sensor
        listOfLocations[sensor]["numofprobes"] = len(usersFrom[sensor].values()) 


        # Creating kml file 
        temp_location = tuple(float(x) for x in listOfLocations[sensor]["Location"].split(","))
        single_point = kml.newpoint(name=listOfLocations[sensor]["name"], description = str(listOfLocations[sensor]["numofprobes"])+ " probe requests caught", coords=[temp_location[::-1]])
    
    # Save the kml file after reading all the sensors
    kml.save( "DATA/"+current_time +" Points.kml") 
        
    # Crate new csv at each time defiend here
    time.sleep(60*60)

