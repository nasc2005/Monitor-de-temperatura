from PyQt5 import  uic,QtWidgets
import requests
import time
import threading

def atualiza_dados():
    while 1:
        time.sleep(1)
       
       # Faz requisição para a field de Temperatura no ThingSpeak"
        resposta = requests.get('https://api.thingspeak.com/channels/2723410/fields/1.json?api_key=A1DS9OEBAOCUX5OV&results=2')
        dados = resposta.json()
        # Faz requisição para a field de Umidade no ThingSpeak"
        resposta2 = requests.get('https://api.thingspeak.com/channels/2723410/fields/2.json?api_key=A1DS9OEBAOCUX5OV&results=2')
        dados2 = resposta2.json()
        
        # Extrai o valor do primeiro item em "feeds" na chave "field1"
        primeiro_field1 = dados['feeds'][1]['field1']

        # Extrai o valor do primeiro item em "feeds" na chave "field2"
        primeiro_field2 = dados2['feeds'][1]['field2']

        # Limita a string para 4 caracteres
        primeiro_field1_formatado = primeiro_field1[:4]

        # Limita a string para 4 caracteres
        primeiro_field2_formatado = primeiro_field2[:4]

        # Exibe dados na Tela
        tela.label_6.setText(primeiro_field1_formatado + " °C")
        tela.label_7.setText(primeiro_field2_formatado + "%")

# cria interface grafica
app=QtWidgets.QApplication([])
# Chama arquivo de estilo da tela
tela=uic.loadUi("tela_monitor.ui")
# Chama função infinita assincronamente
threading.Thread(target=atualiza_dados).start()
# Exibe tela
tela.show()
app.exec()




    


