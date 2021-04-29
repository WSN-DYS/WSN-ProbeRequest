#!/usr/bin/env python
# coding: utf-8

# npm install -g firebase-tools // Run in cmd in windows

# In[ ]:


pip install firebase


# In[ ]:


pip install gcloud


# In[ ]:


pip install python_jwt


# In[ ]:


pip install sseclient


# In[ ]:


pip install pycryptodome


# In[ ]:


pip install requests-toolbelt


# In[45]:


from firebase import Firebase
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
from datetime import datetime
import time


# In[46]:


#configuration to the database Firebase
config = {
  "apiKey": "AIzaSyA4fHNEMAnGEEJV87kFC0GHzMMRc0TatuA",
  "authDomain": "proberequest-finalproject.firebaseapp.com",
  "databaseURL": "https://proberequest-finalproject-default-rtdb.firebaseio.com",
  "storageBucket": "proberequest-finalproject.appspot.com"
}

firebase = Firebase(config)


# In[47]:


n = 0
while n < 2:
    #create dictionary from the information in firebase
    dictionaryForPlot ={}
    db = firebase.database()
    users = db.child("Users").get().val()
    for MacAddress, MacAndRssi in users.items():
        dictionaryForPlot[MacAndRssi["address"]] = MacAndRssi["rssi"] 

    #Using plot for the final graph.
    figure(figsize=(14, 8), dpi=80)
    y = dictionaryForPlot.values()
    x = [ i for i in range(len(dictionaryForPlot))]
    plt.title("Probe requests graph")
    plt.ylabel("Rssi")
    plt.scatter(x, y)

    #Create massege with the final information of the scan, number of devies and current hour.
    now = datetime.now()
    current_time = now.strftime("%H:%M:%S")
    print("The numebr of devices to this hour:")
    print(str(len(dictionaryForPlot)) + " devices, hour: " +str(now)+"\n")
    
    #print all the mac addresses of the devices that found.
    print("The Mac addresses are:")
    for Mac in dictionaryForPlot.keys():
        print(Mac)
    print()
    #wait 10 seconds and print again all the information.
    n+=10
    time.sleep(10)


# In[44]:



figure(figsize=(14, 8), dpi=80)
y = list(dictionaryForPlot.values())
x = [ i for i in range(len(dictionaryForPlot))]
Mac = list(dictionaryForPlot.keys())
plt.title("Probe requests graph")
plt.ylabel("Rssi")
for i in range(len(x)):
   plt.text(x[i]-1,y[i]+0.7,Mac[i],fontdict=dict(color="black",size=7))
plt.scatter(x, y)


# In[ ]:




