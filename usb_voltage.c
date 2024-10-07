#include <string.h>
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
    char comment[32];

    // Определяем значение напряжения и соответствующий комментарий
    if(voltage > 4.5f && voltage < 5.5f) {
        snprintf(voltage_str, sizeof(voltage_str), "USB Voltage: %.2lf V", (double)voltage);
        snprintf(comment, sizeof(comment), "Power bank OK."); // Сокращенный комментарий
    } else if(voltage < 4.5f) {
        snprintf(voltage_str, sizeof(voltage_str), "USB Voltage: %.2lf V", (double)voltage);
        snprintf(comment, sizeof(comment), "Low voltage!"); // Сокращенный комментарий
    } else {
        snprintf(voltage_str, sizeof(voltage_str), "USB Voltage: %.2lf V", (double)voltage);
        snprintf(comment, sizeof(comment), "High voltage!"); // Сокращенный комментарий
    }

    // Очистка канваса и отрисовка текста
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);

    // Отображаем строки
    canvas_draw_str(canvas, 5, 10, voltage_str);
    canvas_draw_str(canvas, 5, 30, comment);
    canvas_draw_str(canvas, 5, 50, "Press back to exit");
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
        furi_delay_ms(100); // Обновление каждые 100 мс
    }

    // Очистка ресурсов
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close("gui");

    return 0;
}
