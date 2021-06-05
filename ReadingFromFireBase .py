# this script download the data from firebase to the computer once every hour.
from firebase import Firebase
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from datetime import datetime
import time
import csv
import pandas as pd
import os

config = {
  "apiKey": "AIzaSyA4fHNEMAnGEEJV87kFC0GHzMMRc0TatuA",
  "authDomain": "proberequest-finalproject.firebaseapp.com",
  "databaseURL": "https://proberequest-finalproject-default-rtdb.firebaseio.com",
  "storageBucket": "proberequest-finalproject.appspot.com"
}


firebase = Firebase(config)
while(True):
    dictionaryForPlot ={}
    db = firebase.database()
    usersFrom = dict(db.child("Users").get().val())
    db.child("Users").remove()
    now = datetime.now()
    current_time = (now.strftime("%m-%d-%y-%H-%M-%S"))
    
    for sensor in usersFrom.keys():
        pd.DataFrame.from_dict(usersFrom[sensor], orient ='index').to_csv("DATA/probeRequest"+str(current_time)+".csv",mode='a')
    time.sleep(60*60)
        
