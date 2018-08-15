print("loading dependencies")
from datetime import datetime
startTime = datetime.now()
import getpass
import os
from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.chrome.options import Options  
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException
import time
from seleniumrequests import Chrome
from seleniumrequests.request import RequestMixin

print("loading chrome")
chrome_options = Options()  
chrome_options.add_argument("--headless")  
chrome_options.add_argument("--no-startup-window")  
chrome_options.add_argument("--bwsi")  
chrome_options.add_argument("--disable-gpu")  
chrome_options.add_argument("--no-sandbox")  
chrome_options.add_argument("--disable-sync")  
chrome_options.add_argument("--disable-background-networking")  
chrome_options.add_argument("--no-first-run")  
chrome_options.add_argument("--aggressive-tab-discard")  
chrome_options.add_argument("--user-agent=Mozilla/4.0 (Windows; MSIE 6.0; Windows NT 5.2)")
chrome_options.add_argument('--user-data-dir=/tmp/user-data')
chrome_options.add_argument('--data-path=/tmp/data-path')
chrome_options.add_argument('--homedir=/tmp')
chrome_options.add_argument('--disk-cache-dir=/tmp/cache-dir')

prefs={"profile.managed_default_content_settings.images": 2, 'disk-cache-size': 4096 }
chrome_options.add_experimental_option("prefs",prefs)

delay = 60 
chrome_options.binary_location = "/usr/bin/chromium-browser"
driver = Chrome(executable_path=os.path.abspath("/usr/lib/chromium-browser/chromedriver"), chrome_options=chrome_options)  
print("logging into outlook")
driver.get("https://outlook.office.com/owa/?ae=Folder&t=IPF.Appointment") 

try:
    driver.find_element_by_name("loginfmt").send_keys("andrew.tayman@fireeye.com")
    driver.find_element_by_id("idSIButton9").click()
    print("entered username, waiting for password prompt")
    

    try:
        myElem = WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'branding')))
        print("password prompt loaded")
    except TimeoutException:
        print("Loading password prompt took too much time!")
        driver.close();
        exit(1)

    passwd = getpass.getpass()
    driver.find_element_by_id("passwordInput").send_keys(passwd)
    driver.find_element_by_id("submitButton").click()
    print("entered password, waiting for 2FA token")
    try:
        myElem = WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'idSIButton9')))
        driver.find_element_by_id("idSIButton9").click()
        print("asking to remember credentials for next time")
    except TimeoutException:
        print("Loading 2FA page took too much time!")
        driver.close();
        exit(1)

    print("2FA accepted, loading office landing page")


except NoSuchElementException:
    print("already logged in")





try:
    myElem = WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.CLASS_NAME, 'cntnttp')))
    print("calendar loaded, dumping entries")
except TimeoutException:
    print(driver.page_source)
    print("Loading calendar took too much time!")
    driver.close();
    exit(1)
    # todo: this should probably be more conditional - if the page defaults to the inbox execute the below to navigate to 
    # the calendar
    # try:
    #     myElem = WebDriverWait(driver, delay).until(EC.presence_of_element_located((By.ID, 'lnkNavCal')))
    #     print("loading calendar")
    #     driver.find_element_by_id("lnkNavCal").click()    
    # except TimeoutException:
    #     print("Loading landing page took too much time!")
    #     print(driver.page_source)
    #     driver.close();
    #     exit(1)

elements = driver.find_elements_by_css_selector("td.v a")
for element in elements:
    print(element.get_attribute('title'))

total_execution_time = datetime.now() - startTime
print(f"total execution time: {total_execution_time}")
