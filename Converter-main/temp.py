import os 

#elenco unità di misura
unita_misura_temperatura = ["Farenheit", "Celsius"]
unita_misura_lunghezza = ["Meters", "Yards"]


#classe madre unità di misura
class UnitMeausure():
    def __init__(self, tipologia, value):
        self.tipologia = tipologia
        self.value = int(value)

#classi figlie
class lunghezza(UnitMeausure):
    
    def do_conversion(self, final):
        if final == self.tipologia:
            return self.value
        if self.tipologia == "Meters":
            return self.meters_convert(final)
        if self.tipologia == "Yards":
            return self.yards_convert(final)
    
    def meters_convert(self,final):
       
        if final == "Yards":
            return self.value * 1.094

    def yards_convert(self,final):
        if final == "Meters":
            return self.value / 1.094

class temperatura(UnitMeausure):
    
    #metodo della conversione
    def do_conversion(self, final):
        if final == self.tipologia:
            return self.value
        if self.tipologia == "Farheneit":
            return self.far_convert(final)
        if self.tipologia == "Celsius":
            return self.celsius_converter(final)
    
    def far_convert(self,final):
  
        #converte da farheneti a celsius
        if final == "Celsius":
            return (self.value * 9/5) + 32
    
    def celsius_converter(self, final):

        #converti da celsius a farheneit
        if final == "Farheneit":
            return (self.value * 9/5) + 32
    
    
   
def main():
    
    while True:
        
        print(unita_misura_lunghezza)
        print(unita_misura_temperatura)
        
        while True:
            user_input = input("seleziona l'unità di misura primaria: ") 
            valore = input("Inserire il valore della misurazione: ")
            
            if user_input not in unita_misura_lunghezza + unita_misura_temperatura:
                print("tipologia unità di misura non presente! ")
                continue
            
            try:
                valore = int(valore)
            except Exception:
                print("valore inserito non numerico")
                continue
            
            break
        
        print("In cosa lo vuoi convertire?")
        
        if user_input in unita_misura_lunghezza:
            unita = lunghezza(user_input, valore)
            unita_possibili = unita_misura_lunghezza.copy()
            unita_possibili.remove(user_input)
            print(unita_possibili)
            
            
            
        elif user_input in unita_misura_temperatura:
            unita = temperatura(user_input, valore)
            unita_possibili = unita_misura_temperatura.copy()
            unita_possibili.remove(user_input)
            print(unita_possibili)
        
        
        while True: 
                finale = input("digita l'unità di misura: ")
                if finale not in unita_possibili:
                    print("unità di misura non in elenco, ripeti")
                    continue
                print(unita.do_conversion(finale))
                break 
       
        decision = input("Continuare? s/n")
        if decision == "s":
            os.system("cls")
            continue
        
        #end program
        break
    
        
        
#starting point
if __name__ == "__main__":
    print("*****************************")
    print("CONVERTITORE UNITA' DI MISURA")
    print("*****************************")
    main()
