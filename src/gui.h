static Janet cfun_GuiLabel(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiLabel(bounds, text);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiButton(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiButton(bounds, text);                                           // Button control, returns true when clicked
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiLabelButton(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    bool result = GuiLabelButton(bounds, text);
    return janet_wrap_boolean(result);
}

typedef struct {
    bool active;
} GuiActive;

static int gui_active_get(void *ptr, Janet key, Janet *out) {
    GuiActive *gui_active = (GuiActive *)ptr;
    if (!janet_checktype(key, JANET_KEYWORD)) {
        return 0;
    }
    const char *key_str = janet_unwrap_keyword(key);
    if (!strcmp(key_str, "active")) {
        *out = janet_wrap_boolean(gui_active->active);
        return 1;
    }
    return 0;
}

static const JanetAbstractType GuiActive_type = {
    "GuiActive",
    NULL,
    NULL,
    gui_active_get,
JANET_ATEND_GET};

static Janet cfun_GuiActive(int argc, Janet *argv) {
    janet_fixarity(argc, 1);
    bool initial_value = janet_getboolean(argv, 0);
    GuiActive *gui_active = (GuiActive *)janet_abstract(&GuiActive_type, sizeof(GuiActive));
    gui_active->active = initial_value;
    return janet_wrap_abstract(gui_active);
}

static Janet cfun_GuiToggle(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT)) {
        janet_panic("Expected a GuiActive as the third argument.");
    }
    GuiActive *gui_active = janet_getabstract(argv, 2, &GuiActive_type);
    bool result = GuiToggle(bounds, text, &gui_active->active);
    return janet_wrap_boolean(result);
}

static Janet cfun_GuiCheckBox(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Rectangle bounds = jaylib_getrect(argv, 0);
    const char *text = jaylib_getcstring(argv, 1);
    if (!janet_checktype(argv[2], JANET_ABSTRACT)) {
        janet_panic("Expected a GuiActive as the third argument.");
    }
    GuiActive *gui_active = janet_getabstract(argv, 2, &GuiActive_type);
    bool result = GuiCheckBox(bounds, text, &gui_active->active);
    return janet_wrap_boolean(result);
}

static JanetReg gui_cfuns[] = {
    {"gui-label", cfun_GuiLabel, NULL},
    {"gui-button", cfun_GuiButton, NULL},
    {"gui-label-button", cfun_GuiLabelButton, NULL},
    {"gui-active", cfun_GuiActive, NULL},
    {"gui-toggle", cfun_GuiToggle, NULL},
    {"gui-checkbox", cfun_GuiCheckBox},
    {NULL, NULL, NULL}
};
