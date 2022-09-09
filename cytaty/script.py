import urllib.request
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
import csv
import time

file = open("../postaci.csv", "r");
type(file)
csvreader = csv.reader(file)

postaci = []

for row in csvreader:
    postaci.append(row[0].replace(" ", "_"))

file.close()
print(postaci)


file = open("cytaty.csv", "w")
driver = webdriver.Chrome(service=Service('./chromedriver/chromedriver'))

for postac in postaci:
    driver.get("https://leagueoflegends.fandom.com/pl/wiki/" + postac + "/cytaty")
    time.sleep(1)
    cytaty = driver.find_elements(By.TAG_NAME, "i")

    for cytat in cytaty:  
        file.write(cytat.get_attribute("innerHTML") + "$" + postac + "$\n")
        # print(cytat.text + "," + postac + ",\n")
