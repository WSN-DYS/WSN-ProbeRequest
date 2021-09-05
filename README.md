# Probe requests sniffer

## introduction
The following project enables the collection of Probe requests from wireless devices with a WiFi component by using ESP8266 components and the ARDUINO platform.

## Why Collect Probe Request?
The probe requests package contains a lot of useful information for all kinds of uses:
We can perform quantitative analysis for example at public transport stations or at disaster events in order to collect to identify and assess a given number of people and their location, and even track a particular user's route.
In addition, we can use the information for business analysis, for example, to study the frequency of shoppers visiting the store and more.

## Project tools:
- ESP8266 - A component that has a WiFi chip with which the requests are collected.
<img src="https://user-images.githubusercontent.com/57191216/130618793-ce1d04dd-81c3-4684-9329-c0ed2e24ed27.png" width="100" height="120">
- Arduino platform - a tool that enables chip programming.
<img src="https://user-images.githubusercontent.com/57191216/130618791-7dc216e2-175b-415e-aec3-a65f4a176ca1.png" width="90" height="80">
- Firebase - A database for storing the information collected.
<img src="https://user-images.githubusercontent.com/57191216/130619865-687359f9-ae0e-40fa-b37b-c52c041a6b52.png" width="130" height="60">
- Python - A script for exporting information from the database to CSV.
<img src="https://user-images.githubusercontent.com/57191216/130619342-bff1c2fe-770d-47c3-90d9-b072e5b0184d.png" width="130" height="80">

## Features:
1. Channel prioritization - In order to optimize the search on busy channels, we built an algorithm that prioritizes and gives more scanning time to channels that caught more requests in the previous run.
2. Use SHA1 encryption to encrypt the MAC addresses of requests in order to maintain user privacy.
3. Export a KML file that allows the components to be displayed on a Google earth map, in addition to displaying the number of requests for each sensor.
4. Export all the information collected in the database to a CSV file at a frequency determined by the user.
