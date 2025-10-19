import speech_recognition as sr
import requests

ESP32_URL = "http://192.168.137.144/set"  

recognizer = sr.Recognizer()
mic = sr.Microphone()

print(" Voice to LCD started. Speak into the mic...")

while True:
    with mic as source:
        recognizer.adjust_for_ambient_noise(source)
        print(" Listening...")
        audio = recognizer.listen(source)

    try:
        text = recognizer.recognize_google(audio)
        print(f" You said: {text}")

        # Send text to ESP32
        r = requests.get(ESP32_URL, params={"msg": text})
        print(f" Sent to ESP32. Status code: {r.status_code}")

    except sr.UnknownValueError:
        print(" Could not understand audio")
    except Exception as e:
        print(f" Error sending to ESP32: {e}")

