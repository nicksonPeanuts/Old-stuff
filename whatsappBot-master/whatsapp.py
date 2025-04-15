from selenium import webdriver
from os import system

#apertura chrome 
chrome_browser = webdriver.Chrome('D:\\programmi\\Chromium\\ChromeDriverNew\\chromedriver.exe')
chrome_browser.get('https://web.whatsapp.com/')

input("Scrivi qualcosa appena sei entrato")
input('Login Effettuato')

#nuovo commento

while True:
    #input
    name = input('Name : ')
    messaggio = input("Text: ")
    contatore = int(input("times: "))

    user = chrome_browser.find_element_by_xpath('//span[@title = "{}"]'.format(name))
    user.click()


    for number in range(contatore):

        #trova e scrive Hey nella barra dei messaggi
        message = chrome_browser.find_element_by_xpath('/html/body/div[1]/div/div/div[4]/div/footer/div[1]/div[2]/div/div[2]')
        message.send_keys(messaggio)

        #invia il messaggio
        button = chrome_browser.find_element_by_xpath('/html/body/div[1]/div/div/div[4]/div/footer/div[1]/div[3]/button')
        button.click()
        system('cls')
