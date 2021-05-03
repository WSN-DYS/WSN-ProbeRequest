#!/usr/bin/env python
# coding: utf-8

# npm install -g firebase-tools // Run in cmd in windows

# In[ ]:


#pip install firebase


# In[ ]:


#pip install gcloud


# In[ ]:


#pip install python_jwt


# In[ ]:


#pip install sseclient


# In[ ]:


#pip install pycryptodome


# In[ ]:


#pip install requests-toolbelt


# In[39]:


from firebase import Firebase
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from datetime import datetime
import time
import csv
import pandas as pd
import os


# In[72]:


#configuration to the database Firebase
config = {
  "apiKey": "AIzaSyA4fHNEMAnGEEJV87kFC0GHzMMRc0TatuA",
  "authDomain": "proberequest-finalproject.firebaseapp.com",
  "databaseURL": "https://proberequest-finalproject-default-rtdb.firebaseio.com",
  "storageBucket": "proberequest-finalproject.appspot.com"
}



firebase = Firebase(config)

dictionaryForPlot ={}
db = firebase.database()
usersFrom = dict(db.child("Users").get().val())
n =0
while(True):
    
    db.child("Users").remove()
    now = datetime.now()
    current_time = (now.strftime("%m-%d-%y-%H-%M-%S"))
    
    for sensor in usersFrom.keys():
        pd.DataFrame.from_dict(usersFrom[sensor], orient ='index').to_csv("DATA/probeRequest"+str(current_time)+".csv",mode='a')
    
    time.sleep(2*60*60)#wait 2 hours and only then update the csv
        


# In[ ]:




