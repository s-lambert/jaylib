#include <stdio.h>

typedef struct
{
    bool value;
} GuiBoolean;

static int gui_boolean_get(void *ptr, Janet key, Janet *out)
{
    GuiBoolean *gui_boolean = (GuiBoolean *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD))
    {
        return 0;
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!janet_cstrcmp(key_str, "value"))
    {
        *out = janet_wrap_boolean(gui_boolean->value);
        return 1;
    }
    return 0;
}

static const JanetAbstractType GuiBoolean_type = {
    "GuiBoolean",
    NULL,
    NULL,
    gui_boolean_get,
    JANET_ATEND_GET};

static Janet cfun_GuiBoolean(int argc, Janet *argv)
{
    janet_fixarity(argc, 1);
    bool initial_value = janet_getboolean(argv, 0);
    GuiBoolean *gui_boolean = (GuiBoolean *)janet_abstract(&GuiBoolean_type, sizeof(GuiBoolean));
    gui_boolean->value = initial_value;
    return janet_wrap_abstract(gui_boolean);
}

typedef struct
{
    int value;
} GuiInteger;

static int gui_integer_get(void *ptr, Janet key, Janet *out)
{
    GuiInteger *gui_integer = (GuiInteger *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD))
    {
        return 0;
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!janet_cstrcmp(key_str, "value"))
    {
        *out = janet_wrap_integer(gui_integer->value);
        return 1;
    }
    return 0;
}

static void gui_integer_put(void *ptr, Janet key, Janet value)
{
    GuiInteger *gui_integer = (GuiInteger *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD))
    {
        janet_panic("expected keyword");
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!janet_cstrcmp(key_str, "value"))
    {
        if (!janet_checktype(value, JANET_NUMBER))
        {
            janet_panic("expected number value");
        }
        gui_integer->value = janet_unwrap_number(value);
    }
}

static const JanetAbstractType GuiInteger_type = {
    "GuiInteger",
    NULL,
    NULL,
    gui_integer_get,
    gui_integer_put,
    JANET_ATEND_PUT};

static Janet cfun_GuiInteger(int argc, Janet *argv)
{
    janet_fixarity(argc, 1);
    int initial_value = janet_getinteger(argv, 0);
    GuiInteger *gui_integer = (GuiInteger *)janet_abstract(&GuiInteger_type, sizeof(GuiInteger));
    gui_integer->value = initial_value;
    return janet_wrap_abstract(gui_integer);
}

typedef struct
{
    float value;
} GuiFloat;

static int gui_float_get(void *ptr, Janet key, Janet *out)
{
    GuiFloat *gui_float = (GuiFloat *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD))
    {
        return 0;
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!janet_cstrcmp(key_str, "value"))
    {
        *out = janet_wrap_number(gui_float->value);
        return 1;
    }
    return 0;
}

static void gui_float_put(void *ptr, Janet key, Janet value)
{
    GuiFloat *gui_float = (GuiFloat *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD))
    {
        janet_panic("expected keyword");
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!janet_cstrcmp(key_str, "value"))
    {
        if (!janet_checktype(value, JANET_NUMBER))
        {
            janet_panic("expected number value");
        }
        gui_float->value = (float)janet_unwrap_number(value);
    }
}

static const JanetAbstractType GuiFloat_type = {
    "GuiFloat",
    NULL,
    NULL,
    gui_float_get,
    gui_float_put,
    JANET_ATEND_PUT};

static Janet cfun_GuiFloat(int argc, Janet *argv)
{
    janet_fixarity(argc, 1);
    float initial_value = (float)janet_getnumber(argv, 0);
    GuiFloat *gui_float = (GuiFloat *)janet_abstract(&GuiFloat_type, sizeof(GuiFloat));
    gui_float->value = initial_value;
    return janet_wrap_abstract(gui_float);
}

static Janet cfun_GuiLock(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 0);
    GuiLock();
    return janet_wrap_nil();
}

static Janet cfun_GuiUnlock(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 0);
    GuiUnlock();
    return janet_wrap_nil();
}

static Janet cfun_GuiLabel(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiLabel(bounds, text);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiButton(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiButton(bounds, text); // Button control, returns true when clicked
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiLabelButton(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiLabelButton(bounds, text);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiToggle(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiBoolean as the third argument.");
    }
    GuiBoolean *gui_boolean = janet_getabstract(argv, 2, &GuiBoolean_type);
    bool result = GuiToggle(bounds, text, &gui_boolean->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiToggleGroup(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    bool result = GuiToggleGroup(bounds, text, &gui_integer->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiToggleSlider(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    bool result = GuiToggleSlider(bounds, text, &gui_integer->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiCheckBox(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiBoolean as the third argument.");
    }
    GuiBoolean *gui_boolean = janet_getabstract(argv, 2, &GuiBoolean_type);
    bool result = GuiCheckBox(bounds, text, &gui_boolean->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiComboBox(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    bool result = GuiComboBox(bounds, text, &gui_integer->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiDropdownBox(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 4);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    bool edit_mode = janet_getboolean(argv, 3);
    bool result = GuiDropdownBox(bounds, text, &gui_integer->value, edit_mode);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiSpinner(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected a GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    int min_value = janet_getinteger(argv, 3);
    int max_value = janet_getinteger(argv, 4);
    bool edit_mode = janet_getboolean(argv, 5);
    bool result = GuiSpinner(bounds, text, &gui_integer->value, min_value, max_value, edit_mode);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiValueBox(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the third argument.");
    }
    GuiInteger *gui_integer = janet_getabstract(argv, 2, &GuiInteger_type);
    int min_value = janet_getinteger(argv, 3);
    int max_value = janet_getinteger(argv, 4);
    bool edit_mode = janet_getboolean(argv, 5);
    bool result = GuiValueBox(bounds, text, &gui_integer->value, min_value, max_value, edit_mode);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiTextBox(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 4);
    Rectangle bounds = jaylib_getrect(argv, 0);
    char *text = (char *)jaylib_getcstring(argv, 1);
    int text_size = janet_getinteger(argv, 2);
    bool edit_mode = janet_getboolean(argv, 3);
    bool result = GuiTextBox(bounds, text, text_size, edit_mode);

    // Write back text changes to the Janet buffer.
    JanetBuffer *buf = janet_unwrap_buffer(argv[1]);
    janet_buffer_setcount(buf, 0);
    janet_buffer_push_cstring(buf, text);

    return janet_wrap_boolean(result);
}

static Janet cfun_GuiSlider(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *left_text = jaylib_getcstring(argv, 1);
    const char *right_text = jaylib_getcstring(argv, 2);
    if (!janet_checktype(argv[3], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiFloat as the third argument.");
    }
    GuiFloat *gui_float = janet_getabstract(argv, 3, &GuiFloat_type);
    float min_value = (float)janet_getnumber(argv, 4);
    float max_value = (float)janet_getnumber(argv, 5);
    bool result = GuiSlider(bounds, left_text, right_text, &gui_float->value, min_value, max_value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiSliderBar(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *left_text = jaylib_getcstring(argv, 1);
    const char *right_text = jaylib_getcstring(argv, 2);
    if (!janet_checktype(argv[3], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiFloat as the third argument.");
    }
    GuiFloat *gui_float = janet_getabstract(argv, 3, &GuiFloat_type);
    float min_value = (float)janet_getnumber(argv, 4);
    float max_value = (float)janet_getnumber(argv, 5);
    bool result = GuiSliderBar(bounds, left_text, right_text, &gui_float->value, min_value, max_value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiProgressBar(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *left_text = jaylib_getcstring(argv, 1);
    const char *right_text = jaylib_getcstring(argv, 2);
    if (!janet_checktype(argv[3], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiFloat as the third argument.");
    }
    GuiFloat *gui_float = janet_getabstract(argv, 3, &GuiFloat_type);
    float min_value = (float)janet_getnumber(argv, 4);
    float max_value = (float)janet_getnumber(argv, 5);
    bool result = GuiProgressBar(bounds, left_text, right_text, &gui_float->value, min_value, max_value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiStatusBar(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiStatusBar(bounds, text);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiDummyRec(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiDummyRec(bounds, text);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiGrid(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 5);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    float spacing = (float)janet_getnumber(argv, 2);
    int subdivs = janet_getinteger(argv, 3);
    Vector2 mouse_cell = jaylib_getvec2(argv, 4);
    bool result = GuiGrid(bounds, text, spacing, subdivs, &mouse_cell);

    // Write back mouse_cell values to the Janet array.
    JanetArray *array = janet_getarray(argv, 4);
    array->data[0] = janet_wrap_number(mouse_cell.x);
    array->data[1] = janet_wrap_number(mouse_cell.y);

    return janet_wrap_boolean(result);
}

static Janet cfun_GuiListView(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 4);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the third argument.");
    }
    GuiInteger *scroll_index = janet_getabstract(argv, 2, &GuiInteger_type);
    if (!janet_checktype(argv[2], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the fourth argument.");
    }
    GuiInteger *active = janet_getabstract(argv, 3, &GuiInteger_type);
    bool result = GuiListView(bounds, text, &scroll_index->value, &active->value);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiListViewEx(int32_t argc, Janet *argv)
{
    janet_fixarity(argc, 6);
    Rectangle bounds = jaylib_getrect(argv, 0);
    JanetView idx = janet_getindexed(argv, 1);
    const char **text = janet_scalloc(sizeof(const char **), (size_t)(idx.len));
    for (unsigned i = 0; i < idx.len; i++)
    {
        text[i] = jaylib_getcstring(idx.items, i);
    }
    const int count = janet_getinteger(argv, 2);
    if (!janet_checktype(argv[3], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the fourth argument.");
    }
    GuiInteger *scroll_index = janet_getabstract(argv, 3, &GuiInteger_type);
    if (!janet_checktype(argv[4], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the fifth argument.");
    }
    GuiInteger *active = janet_getabstract(argv, 4, &GuiInteger_type);
    if (!janet_checktype(argv[5], JANET_ABSTRACT))
    {
        janet_panic("Expected GuiInteger as the sixth argument.");
    }
    GuiInteger *focus = janet_getabstract(argv, 5, &GuiInteger_type);
    bool result = GuiListViewEx(bounds, text, count, &scroll_index->value, &active->value, &focus->value);
    return janet_wrap_boolean(result);
}

static JanetReg gui_cfuns[] = {
    {"gui-boolean", cfun_GuiBoolean, NULL},
    {"gui-integer", cfun_GuiInteger, NULL},
    {"gui-float", cfun_GuiFloat, NULL},

    {"gui-lock", cfun_GuiLock, NULL},
    {"gui-unlock", cfun_GuiUnlock, NULL},

    // Basic controls
    {"gui-label", cfun_GuiLabel, NULL},
    {"gui-button", cfun_GuiButton, NULL},
    {"gui-label-button", cfun_GuiLabelButton, NULL},
    {"gui-toggle", cfun_GuiToggle, NULL},
    {"gui-toggle-group", cfun_GuiToggleGroup, NULL},
    {"gui-toggle-slider", cfun_GuiToggleSlider, NULL},
    {"gui-check-box", cfun_GuiCheckBox, NULL},
    {"gui-combo-box", cfun_GuiComboBox, NULL},

    {"gui-dropdown-box", cfun_GuiDropdownBox, NULL},
    {"gui-spinner", cfun_GuiSpinner, NULL},
    {"gui-value-box", cfun_GuiValueBox, NULL},
    {"gui-text-box", cfun_GuiTextBox, NULL},

    {"gui-slider", cfun_GuiSlider, NULL},
    {"gui-slider-bar", cfun_GuiSliderBar, NULL},
    {"gui-progress-bar", cfun_GuiProgressBar, NULL},
    {"gui-status-bar", cfun_GuiStatusBar, NULL},
    {"gui-dummy-rec", cfun_GuiDummyRec, NULL},
    {"gui-grid", cfun_GuiGrid, NULL},

    // Advanced controls
    {"gui-list-view", cfun_GuiListView, NULL},
    {"gui-list-view-ex", cfun_GuiListViewEx, NULL},
    {NULL, NULL, NULL}};
