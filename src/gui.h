#include <stdio.h>

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
    if (!strcmp(key_str, "value"))
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

static JanetReg gui_cfuns[] = {
    {"gui-lock", cfun_GuiLock, NULL},
    {"gui-unlock", cfun_GuiUnlock, NULL},

    {"gui-label", cfun_GuiLabel, NULL},
    {"gui-button", cfun_GuiButton, NULL},
    {"gui-label-button", cfun_GuiLabelButton, NULL},
    {"gui-boolean", cfun_GuiBoolean, NULL},
    {"gui-integer", cfun_GuiInteger, NULL},
    {"gui-toggle", cfun_GuiToggle, NULL},
    {"gui-toggle-group", cfun_GuiToggleGroup, NULL},
    {"gui-toggle-slider", cfun_GuiToggleSlider, NULL},
    {"gui-check-box", cfun_GuiCheckBox, NULL},
    {"gui-combo-box", cfun_GuiComboBox, NULL},

    {"gui-dropdown-box", cfun_GuiDropdownBox, NULL},
    {"gui-spinner", cfun_GuiSpinner, NULL},
    {"gui-value-box", cfun_GuiValueBox, NULL},
    {"gui-text-box", cfun_GuiTextBox, NULL},
    {NULL, NULL, NULL}};
