#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>

typedef struct {
    bool exit;
} UsbVoltageState;

void render_voltage_on_screen(Canvas* canvas, void* ctx) {
    (void)ctx; // Игнорируем неиспользуемый параметр

    float voltage = furi_hal_power_get_usb_voltage();

    char voltage_str[32];
    snprintf(voltage_str, sizeof(voltage_str), "USB Voltage: %.2lf V", (double)voltage);

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 10, 30, voltage_str);
    canvas_draw_str(canvas, 10, 50, "Press back to exit");
}

// Обработчик ввода
static void usb_voltage_input_handler(InputEvent* event, void* ctx) {
    UsbVoltageState* state = ctx;
    if(event->type == InputTypePress && event->key == InputKeyBack) {
        state->exit = true;
    }
}

int32_t usb_voltage_app(void* p) {
    (void)p;
    UsbVoltageState state = {.exit = false};

    ViewPort* view_port = view_port_alloc();
    Gui* gui = furi_record_open("gui");

    // Установка функции отрисовки на экране
    view_port_draw_callback_set(view_port, render_voltage_on_screen, &state);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // Установка обработчика ввода
    view_port_input_callback_set(view_port, usb_voltage_input_handler, &state);

    // Главный цикл приложения
    while(!state.exit) {
        view_port_update(view_port);
        furi_delay_ms(100);
    }

    // Очистка ресурсов
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close("gui");

    return 0;
}
