env = DefaultEnvironment()

# Создание FAP файла для приложения
fap = env.BuildFap(
    target="usb_voltage",            # Название FAP файла
    app=["usb_voltage.c"],           # Исходный код приложения
)

# Зависимость от исходного кода
Depends(fap, "usb_voltage.c")