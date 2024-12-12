import flet as ft
import serial
import threading

# Configuración del puerto serial
ser = serial.Serial('COM11', 115200, timeout=1)  # Añadir timeout para evitar bloqueos

# Función para leer datos del puerto serial
def read_serial(leds, text_display, pause_button, reiniciar_button, led_indicator, start_button, status_led):
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Received: {data}")  # Mensaje de depuración
            text_display.value = data
            text_display.update()
            if data == "REINICIAR":
                for led in leds:
                    led.bgcolor = ft.colors.BLACK
                    led.update()
            elif data.startswith("P") and data[1:].isdigit():
                index = int(data[1]) - 1
                if 0 <= index < len(leds):
                    leds[index].bgcolor = ft.colors.GREEN
                    leds[index].update()
            elif data == "PAUSAO":
                pause_button.text = "CONTINUAR"
                led_indicator.bgcolor = ft.colors.RED
                pause_button.update()
                led_indicator.update()
            elif data == "CONTINUAO":
                pause_button.text = "PAUSAR"
                led_indicator.bgcolor = ft.colors.GREEN
                pause_button.update()
                led_indicator.update()
            elif data == "VAL":
                print("VAL received")  # Mensaje de depuración
                status_led.bgcolor = ft.colors.GREEN
                status_led.content = ft.Text("VALIDO", color=ft.colors.WHITE, size=60)  # Ajustar el tamaño del texto
                status_led.update()
            elif data == "DEF":
                print("DEF received")  # Mensaje de depuración
                status_led.bgcolor = ft.colors.RED
                status_led.content = ft.Text("DEFECTUOSO", color=ft.colors.WHITE, size=60)  # Ajustar el tamaño del texto
                status_led.update()
            
            # Verificar si todos los LEDs están encendidos
            all_on = all(led.bgcolor == ft.colors.GREEN for led in leds)
            reiniciar_button.disabled = not all_on
            reiniciar_button.update()

# Función para enviar datos al puerto serial
def send_serial(e, text_input):
    ser.write(text_input.value.encode('utf-8'))
    ser.flush()  # Asegurarse de que los datos se envían inmediatamente
    text_input.value = ""
    text_input.update()

def start_button_click(e, start_button, start_led):
    ser.write("START".encode('utf-8'))
    ser.flush()  # Asegurarse de que los datos se envían inmediatamente
    start_button.disabled = True
    start_button.update()
    start_led.bgcolor = ft.colors.GREEN
    start_led.update()

# Función para enviar la palabra "REINICIAR" al puerto serial y apagar los LEDs
def send_reiniciar(e, leds):
    if all(led.bgcolor == ft.colors.GREEN for led in leds):
        ser.write("REINICIAR".encode('utf-8'))
        ser.flush()  # Asegurarse de que los datos se envían inmediatamente
        for led in leds:
            led.bgcolor = ft.colors.BLACK
            led.update()

# Función para manejar el botón de pausa/continuar
def toggle_pause(e, pause_button, led_indicator):
    if pause_button.text == "PAUSAR":
        ser.write("PAUSAR".encode('utf-8'))
        ser.flush()  # Asegurarse de que los datos se envían inmediatamente
        pause_button.text = "CONTINUAR"
        led_indicator.bgcolor = ft.colors.RED
    else:
        ser.write("CONTINUAR".encode('utf-8'))
        ser.flush()  # Asegurarse de que los datos se envían inmediatamente
        pause_button.text = "PAUSAR"
        led_indicator.bgcolor = ft.colors.GREEN
    pause_button.update()
    led_indicator.update()

# Función para enviar "VAL" al puerto serial
def send_val(e):
    print("Sending VAL")  # Mensaje de depuración
    ser.write("VAL".encode('utf-8'))
    ser.flush()  # Asegurarse de que los datos se envían inmediatamente

# Función para enviar "DEF" al puerto serial
def send_def(e):
    print("Sending DEF")  # Mensaje de depuración
    ser.write("DEF".encode('utf-8'))
    ser.flush()  # Asegurarse de que los datos se envían inmediatamente

# Crear la interfaz
def main(page):
    page.window_width = 1920
    page.window_height = 1080
    page.vertical_alignment = ft.MainAxisAlignment.CENTER

    leds = [ft.Container(width=100, height=100, bgcolor=ft.colors.BLACK) for _ in range(8)]  # Doble de tamaño
    
    led_grid_1 = ft.Column([
        ft.Row([leds[0], leds[1]], spacing=10),
        ft.Row([leds[2], leds[3]], spacing=10),
        ft.Text("Malla 1")
    ], spacing=10)

    led_grid_2 = ft.Column([
        ft.Row([leds[4], leds[5]], spacing=10),
        ft.Row([leds[6], leds[7]], spacing=10),
        ft.Text("Malla 2")
    ], spacing=10)


    text_display = ft.Text(value="", size=20)
    text_input = ft.TextField(label="Enviar al serial", on_submit=lambda e: send_serial(e, text_input))
    reiniciar_button = ft.ElevatedButton(text="REINICIAR", on_click=lambda e: send_reiniciar(e, leds), disabled=True, width=200, height=100)
    pause_button = ft.ElevatedButton(text="PAUSAR", bgcolor=ft.colors.YELLOW, color=ft.colors.BLACK, on_click=lambda e: toggle_pause(e, pause_button, led_indicator), width=200, height=100)
    led_indicator = ft.Container(width=200, height=100, bgcolor=ft.colors.GREEN)  # Ajustar el ancho del indicador
    start_led = ft.Container(width=200, height=100, bgcolor=ft.colors.BLACK)
    start_button = ft.ElevatedButton(text="START", on_click=lambda e: start_button_click(e, start_button, start_led), width=200, height=100)
    
    # Nuevo LED de estado
    status_led = ft.Container(width=600, height=300, bgcolor=ft.colors.WHITE, content=ft.Text("", size=60))  # Triplicar el tamaño

    # Contenedor para el botón START y su LED
    start_container = ft.Container(
        content=ft.Row([start_led, start_button], alignment=ft.MainAxisAlignment.CENTER),
        alignment=ft.alignment.center
    )
    
    # Contenedor para el indicador de estado
    status_container = ft.Container(
        content=status_led,
        alignment=ft.alignment.center
    )
    
    # Organizar los elementos en la página
    page.add(
        status_container,  # Añadir el contenedor del indicador de estado aquí, centrado y en la parte superior
        ft.Row([
            ft.Column([
                led_grid_1,
                led_grid_2
            ], alignment=ft.MainAxisAlignment.START),
            ft.Column([
                text_display,
                text_input,
            ], alignment=ft.MainAxisAlignment.CENTER)
        ], alignment=ft.MainAxisAlignment.SPACE_BETWEEN),
        ft.Row([
            start_container,
            reiniciar_button,
            pause_button,
            led_indicator
        ], alignment=ft.MainAxisAlignment.CENTER)
    )
    
    threading.Thread(target=read_serial, args=(leds, text_display, pause_button, reiniciar_button, led_indicator, start_button, status_led), daemon=True).start()

# Ejecutar la aplicación Flet
ft.app(target=main)