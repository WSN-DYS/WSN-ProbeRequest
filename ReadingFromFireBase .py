
## all those modules need to be installed before using this script:

# npm install -g firebase-tools // Run in cmd in windows
#pip install firebase
#pip install gcloud
#pip install python_jwt
#pip install sseclient
#pip install pycryptodome
#pip install requests-toolbelt

from firebase import Firebase
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from datetime import datetime
import time
import csv
import pandas as pd
import os

#configuration to the database Firebase all the information to down load from our databae is here
config = {
  "apiKey": "AIzaSyA4fHNEMAnGEEJV87kFC0GHzMMRc0TatuA",
  "authDomain": "proberequest-finalproject.firebaseapp.com",
  "databaseURL": "https://proberequest-finalproject-default-rtdb.firebaseio.com",
  "storageBucket": "proberequest-finalproject.appspot.com"
}



firebase = Firebase(config)

db = firebase.database()

#while loop that can run in the background and collect the data,
#in each period of time the user want 
while(True):
	
	# creating a dictionary for the values from the database
	usersFrom ={}
	usersFrom = dict(db.child("Users").get().val()) 

	# remove all the old data to make spave in the database
	db.child("Users").remove() 

	# get the current time
	now = datetime.now() 
	current_time = (now.strftime("%m-%d-%y-%H-%M-%S"))

	# creating a rew in csv file for each probe request collected
	#the file saved in created folder named DATA in the same folder this script is running from
	for sensor in usersFrom.keys():
		pd.DataFrame.from_dict(usersFrom[sensor], orient ='index').to_csv("DATA/probeRequest"+str(current_time)+".csv",mode='a')

	# a timer for exporting the data currently set to one day
	time.sleep(24*60*60)
