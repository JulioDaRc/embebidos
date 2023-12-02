import serial
import paho.mqtt.client as mqtt
import numpy as np

cliente_mqtt = mqtt.Client()
cliente_mqtt.connect("192.168.137.106", 1883) 

#sensor1
with open('/home/julio/Desktop/frec1.txt', 'r') as archivo:
 datos = archivo.read()           
AmbulanciaFrecTXT1 = np.array([float(valor) for valor in datos.split()]) 
with open('/home/julio/Desktop/frec2.txt', 'r') as archivo:
 datos = archivo.read()           
AmbulanciaFrecTXT2 = np.array([float(valor) for valor in datos.split()])
with open('/home/julio/Desktop/frec3.txt', 'r') as archivo:
 datos = archivo.read()           
AmbulanciaFrecTXT3 = np.array([float(valor) for valor in datos.split()])


#sensor2
with open('/home/julio/Desktop/frec1Sen2.txt', 'r') as archivo:
 datos = archivo.read()           
FrecSen2TXT1 = np.array([float(valor) for valor in datos.split()]) 
with open('/home/julio/Desktop/frec2Sen2.txt', 'r') as archivo:
 datos = archivo.read()           
FrecSen2TXT2 = np.array([float(valor) for valor in datos.split()])
with open('/home/julio/Desktop/frec3Sen2.txt', 'r') as archivo:
 datos = archivo.read()           
FrecSen2TXT3 = np.array([float(valor) for valor in datos.split()])


def distancia_euclidiana(FrecTiemReal, txt1,txt2,txt3):
  de1=np.linalg.norm(FrecTiemReal - txt1)
  de2=np.linalg.norm(FrecTiemReal - txt2)
  de3=np.linalg.norm(FrecTiemReal - txt3)
  Promedio=(de1+de2+de3)/3

  return Promedio

def fourier(amplitudes):
  fourier_transform = np.fft.fft(amplitudes)
  magnitudesSen = np.abs(fourier_transform)
  np.set_printoptions(suppress=True, precision=4)
  return magnitudesSen




SemaACT1=False
SemaACT2=False
puerto_serial = serial.Serial('/dev/ttyACM0', 9600)  # Reemplaza '/dev/ttyUSB0' con tu puerto USB

AmplSen1 = np.array([]) 
AmplSen2 = np.array([]) 
AmplSen3 = np.array([]) 
AmplSen4 = np.array([]) 
while True:
    try:
      line = puerto_serial.readline().decode('utf-8').rstrip()
      sensor_values = line.split(",")
      print(sensor_values)        

      if len(sensor_values) == 4:
        AmplSen1 = np.append(AmplSen1, sensor_values[0])
        AmplSen2 = np.append(AmplSen2, sensor_values[1])
        AmplSen3 = np.append(AmplSen3, sensor_values[2])
        AmplSen4 = np.append(AmplSen4, sensor_values[3])

      if len(AmplSen1)==250 and len(AmplSen4)==250:
        FrecSen1=fourier(AmplSen1)
        FrecSen2=fourier(AmplSen2)
        FrecSen3=fourier(AmplSen3)
        FrecSen4=fourier(AmplSen4)
        DiEucSen1=distancia_euclidiana(FrecSen1, AmbulanciaFrecTXT1,AmbulanciaFrecTXT2,AmbulanciaFrecTXT3)
        DiEucSen2=distancia_euclidiana(FrecSen2, FrecSen2TXT1,FrecSen2TXT2,FrecSen2TXT3)
        #DiEucSen3=distancia_euclidiana(FrecSen3, AmbulanciaFrecTXT1,AmbulanciaFrecTXT2,AmbulanciaFrecTXT3)
        #DiEucSen4=distancia_euclidiana(FrecSen4, AmbulanciaFrecTXT1,AmbulanciaFrecTXT2,AmbulanciaFrecTXT3)
        print("promedio Dist sen1: ",DiEucSen1)
        print("promedio Dist sen2: ",DiEucSen2)
        if SemaACT1==False and SemaACT2==False:
          if 370 < DiEucSen1 <= 458:          
              SemaACT1=True
              cliente_mqtt.publish("SecuenciaSem", "off")
              cliente_mqtt.publish("Sen1", "Emergency")
              cliente_mqtt.publish("Sen2", " ")                    
              cliente_mqtt.publish("Sen3", " ")
              cliente_mqtt.publish("Sen4", " ")
          else:
            cliente_mqtt.publish("SecuenciaSem", "on")        
            SemaACT1=False 
            cliente_mqtt.publish("Sen1", " ")
            cliente_mqtt.publish("Sen2", " ")                    
            cliente_mqtt.publish("Sen3", " ")
            cliente_mqtt.publish("Sen4", " ")

 
        if SemaACT1==True and not(370 < DiEucSen1 <= 458):
          SemaACT1=False 
          cliente_mqtt.publish("SecuenciaSem", "on")
        

        if SemaACT1==False and SemaACT2==False:
          if 3800 < DiEucSen2 <= 6420:
              SemaACT2=True
              cliente_mqtt.publish("SecuenciaSem", "off")
              cliente_mqtt.publish("Sen1", " ")
              cliente_mqtt.publish("Sen2", "Emergency")                    
              cliente_mqtt.publish("Sen3", " ")
              cliente_mqtt.publish("Sen4", " ")
          else:
            cliente_mqtt.publish("SecuenciaSem", "on")
            cliente_mqtt.publish("Sen1", " ")
            cliente_mqtt.publish("Sen2", " ") 
            SemaACT2=False                   
            cliente_mqtt.publish("Sen3", " ")
            cliente_mqtt.publish("Sen4", " ")
        if SemaACT2==True and not(3800 < DiEucSen2 <= 6420):
          SemaACT2=False
          cliente_mqtt.publish("SecuenciaSem", "on")
          cliente_mqtt.publish("Sen2", " ")    



                    
          

        AmplSen1 = np.array([]) 
        AmplSen2 = np.array([]) 
        AmplSen3 = np.array([]) 
        AmplSen4 = np.array([]) 
    except ValueError:
      print("Error en la conversión de datos. Datos recibidos: ", line)



          
