(use ../build/jaylib)

(init-window 800 600 "Test Game")
(set-target-fps 60)

(var toggle_active (gui-boolean false))
(var toggle_group_active (gui-integer 1))
(var toggle_slider_active (gui-integer 1))
(var check_active (gui-boolean false))
(var combo_active (gui-integer 1))
(var dropdown_active (gui-integer 1))
(var dropdown_open false)
(var loading_spinner (gui-integer 0))
(var editing_number false)
(var number_input (gui-integer 5))
(var editing_text false)
(var text_input @"text")

(while (not (window-should-close))
  (begin-drawing)
  (clear-background [0 0 0])

  # Lock the UI while the dropdown is open
  (if dropdown_open (gui-lock))

  (gui-label [0 0 75 25] "LABEL")
  (if (gui-button [0 25 75 25] "BUTTON")
    (print "clicked button"))
  (if (gui-label-button [0 50 75 25] "LABEL_BUTTON")
    (print "clicked label button"))
  (gui-toggle [0 75 75 25] "TOGGLE" toggle_active)
  (gui-toggle-group [0 100 75 25] "1;2;3" toggle_group_active)
  (gui-toggle-slider [0 125 75 25] "1;2;3" toggle_slider_active)
  (gui-check-box [0 150 25 25] "CHECKBOX" check_active)
  (gui-combo-box [0 175 75 25] "1;2;3" combo_active)
  (if (gui-value-box [0 225 75 25] "NUMBER" number_input 0 10 editing_number)
    (set editing_number (not editing_number)))
  (gui-spinner [125 0 75 25] "SPINNER" loading_spinner 0 100 false)

  (if (gui-text-box [0 250 75 25] text_input 10 editing_text)
    (set editing_text (not editing_text)))

  # Unlock and then render the dropdown so it's still interactable
  (gui-unlock)

  (if (gui-dropdown-box [0 200 75 25] "1;2;3" dropdown_active dropdown_open)
    (set dropdown_open (not dropdown_open)))


  (end-drawing))

(close-window)
