introduction
The following project enables the collection of Probe requests from wireless devices with a WiFi component by using ESP8266 components and the ARDUINO platform

Why Collect Probe Request?
The probe requests package contains a lot of useful information for all kinds of uses:
We can perform quantitative analysis for example at public transport stations or at disaster events in order to collect to identify and assess a given number of people and their location, and even track a particular user's route.
In addition, we can use the information for business analysis, for example, to study the frequency of shoppers visiting the store and more.

Project tools:
ESP8266 - A component that has a WiFi chip with which the requests are collected
Arduino platform - a tool that enables chip programming.
Firebase - A database for storing the information collected.
Python - A script for exporting information from the database to CSV

Features
1. Channel prioritization - In order to optimize the search on busy channels, we built an algorithm that prioritizes and gives more scanning time to channels that caught more requests in the previous run.
2. Use SHA1 encryption to encrypt the MAC addresses of requests in order to maintain user privacy.
3. Export a KML file that allows the components to be displayed on a Google earth map, in addition to displaying the number of requests for each sensor.
4. Export all the information collected in the database to a CSV file at a frequency determined by the user
